# Module 10: Concurrency & Multithreading

> *"Concurrency is hard. C++ gives you the tools to do it right — but you must use them carefully."*

---

## 10.1 `std::thread` — Creating Threads

```cpp
#include <thread>
#include <iostream>

// Basic thread creation:
void worker(int id) {
    std::cout << "Thread " << id << " running\n";
}

std::thread t1(worker, 1);         // Start thread with function + args
std::thread t2([](){ std::cout << "Lambda thread\n"; });  // Lambda

t1.join();   // Wait for t1 to finish (MUST call join or detach!)
t2.join();   // Wait for t2 to finish
// If you don't join or detach: std::terminate() called → crash!

// Detach: let thread run independently (fire-and-forget)
std::thread t3(worker, 3);
t3.detach();  // t3 runs on its own; you can no longer join it

// Hardware concurrency:
unsigned int n = std::thread::hardware_concurrency();  // # of CPU cores

// Thread ID:
std::thread::id main_id = std::this_thread::get_id();
std::cout << "Main thread: " << main_id << "\n";
```

---

## 10.2 Data Races and Undefined Behavior

```cpp
// DATA RACE: Two threads access the same memory, at least one writes
// Result: UNDEFINED BEHAVIOR (unpredictable output, crashes, corruption)

int counter = 0;

void increment_unsafe() {
    for (int i = 0; i < 100000; ++i) {
        counter++;  // NOT THREAD-SAFE!
        // counter++ is actually 3 operations:
        // 1. Read counter
        // 2. Increment value
        // 3. Write back
        // Another thread can interleave between ANY of these steps!
    }
}

std::thread t1(increment_unsafe);
std::thread t2(increment_unsafe);
t1.join();
t2.join();
std::cout << counter;  // NOT necessarily 200000! Could be anything!
```

---

## 10.3 `std::mutex` — Mutual Exclusion

```cpp
#include <mutex>

std::mutex mtx;
int counter = 0;

void increment_safe() {
    for (int i = 0; i < 100000; ++i) {
        mtx.lock();   // Acquire the lock (blocks if another thread holds it)
        counter++;    // Critical section — only ONE thread at a time
        mtx.unlock(); // Release the lock
    }
}

// BETTER: use lock_guard (RAII — automatically unlocks!)
void increment_raii() {
    for (int i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> guard(mtx);
        // Lock acquired; automatically released when guard goes out of scope
        counter++;
    }
}

// EVEN BETTER: unique_lock (more flexible — can unlock/relock manually)
void increment_unique() {
    for (int i = 0; i < 100000; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        counter++;
        lock.unlock();  // Can manually unlock before scope ends
        // ... do non-critical work ...
        lock.lock();    // Can relock
    }
}

// C++17: std::scoped_lock (locks MULTIPLE mutexes safely)
std::mutex m1, m2;
void safe_dual_lock() {
    std::scoped_lock lock(m1, m2);  // Locks both without deadlock risk!
    // Uses deadlock-prevention algorithm
}
```

---

## 10.4 `std::atomic` — Lock-Free Operations

```cpp
#include <atomic>

// Atomic operations: thread-safe without locking
// Suitable for simple types (int, bool, pointer, etc.)

std::atomic<int> counter{0};

void increment_atomic() {
    for (int i = 0; i < 100000; ++i) {
        counter++;          // Atomic increment — thread-safe!
        counter.fetch_add(1);  // Explicit atomic add (same as ++)
    }
}

// Compare-and-swap (CAS): the fundamental atomic operation
std::atomic<int> val{0};
int expected = 0;
int desired = 1;
bool success = val.compare_exchange_strong(expected, desired);
// If val == expected: val = desired, returns true
// If val != expected: expected = val, returns false

// Atomic load and store:
val.store(42);          // Atomic store
int v = val.load();     // Atomic load
int old = val.exchange(10);  // Atomically set to 10, return old value

// Memory ordering (advanced):
counter.store(1, std::memory_order_relaxed);  // Just atomicity, no ordering
counter.load(std::memory_order_acquire);       // Acquire semantics
counter.store(1, std::memory_order_release);   // Release semantics
counter.store(1, std::memory_order_seq_cst);   // Default: sequential consistency

// Atomic flag: simplest atomic type (guaranteed lock-free)
std::atomic_flag flag = ATOMIC_FLAG_INIT;
flag.test_and_set();  // Atomically set to true, return previous value
flag.clear();         // Atomically set to false
```

---

## 10.5 Condition Variables

```cpp
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool data_ready = false;
int shared_data = 0;

// Producer thread:
void producer() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        shared_data = 42;
        data_ready = true;
    }
    cv.notify_one();  // Wake up ONE waiting thread
    // cv.notify_all(); // Wake up ALL waiting threads
}

// Consumer thread:
void consumer() {
    std::unique_lock<std::mutex> lock(mtx);
    
    // Wait until data_ready is true
    cv.wait(lock, []{ return data_ready; });
    // 'wait' atomically: 1. releases lock, 2. waits for notify, 3. reacquires lock
    // The predicate prevents spurious wakeups
    
    std::cout << "Got data: " << shared_data << "\n";
}

// Producer-Consumer with a bounded queue:
template<typename T>
class BoundedQueue {
    std::queue<T> q;
    std::mutex mtx;
    std::condition_variable not_full;
    std::condition_variable not_empty;
    size_t max_size;

public:
    BoundedQueue(size_t n) : max_size{n} {}

    void push(T item) {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [this]{ return q.size() < max_size; });
        q.push(std::move(item));
        not_empty.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [this]{ return !q.empty(); });
        T item = std::move(q.front());
        q.pop();
        not_full.notify_one();
        return item;
    }
};
```

---

## 10.6 `std::future` and `std::promise`

```cpp
#include <future>

// future: receive a value from another thread
// promise: send a value to another thread

std::promise<int> promise;
std::future<int> future = promise.get_future();

// Thread 1 (producer):
std::thread t([&promise]() {
    // ... do some computation ...
    promise.set_value(42);  // Send result
    // promise.set_exception(std::make_exception_ptr(std::runtime_error("error")));
});

// Thread 2 (consumer):
int result = future.get();  // BLOCKS until value is available
std::cout << result;  // 42
t.join();
```

---

## 10.7 `std::async` — High-Level Async

```cpp
// std::async: simplest way to run something asynchronously
// Returns a future for the result

auto future = std::async(std::launch::async, []() {
    // This runs in a separate thread
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 42;
});

// Do other work while async runs...
std::cout << "Doing other work\n";

// Get the result (blocks if not ready):
int result = future.get();  // Waits if necessary
std::cout << "Result: " << result << "\n";

// Multiple async tasks:
std::vector<std::future<int>> futures;
for (int i = 0; i < 8; ++i) {
    futures.push_back(std::async(std::launch::async, [i]() {
        return expensive_computation(i);
    }));
}

// Collect results:
int total = 0;
for (auto& f : futures) {
    total += f.get();
}

// Launch policies:
std::launch::async   // Always run in new thread
std::launch::deferred // Run lazily (in calling thread when get() called)
std::launch::async | std::launch::deferred  // Let implementation decide (default)
```

---

## 10.8 Thread-Local Storage

```cpp
// Thread-local variables: each thread has its own copy
thread_local int thread_id = 0;
thread_local std::vector<int> per_thread_data;

void worker(int id) {
    thread_id = id;  // Only affects THIS thread's copy
    per_thread_data.push_back(id);
    // Other threads have their own thread_id and per_thread_data
}

std::thread t1(worker, 1);
std::thread t2(worker, 2);
// thread_id in t1 = 1, thread_id in t2 = 2, main thread's thread_id = 0
t1.join(); t2.join();
```

---

## 10.9 Deadlock and How to Avoid It

```
┌──────────────────────────────────────────────────────────────────────┐
│                    DEADLOCK                                            │
│                                                                        │
│  Thread 1:                    Thread 2:                               │
│  lock(mutex_A)                lock(mutex_B)                          │
│  // ...                       // ...                                  │
│  lock(mutex_B) ← WAITS        lock(mutex_A) ← WAITS                 │
│  // Waiting for B             // Waiting for A                        │
│                                                                        │
│  Neither thread can proceed → DEADLOCK!                               │
│                                                                        │
│  SOLUTIONS:                                                            │
│  1. Always lock mutexes in the SAME ORDER everywhere                 │
│  2. Use std::scoped_lock (locks all at once, deadlock-free)          │
│  3. Use std::lock + std::lock_guard with adopt_lock                  │
│  4. Use try_lock + timeout patterns                                   │
└──────────────────────────────────────────────────────────────────_____┘
```

```cpp
// DEADLOCK-PRONE:
void bad() {
    std::lock_guard<std::mutex> g1(m1);  // Thread 1 locks m1
    std::lock_guard<std::mutex> g2(m2);  // Thread 1 tries m2...
    // Thread 2 might have locked m2, trying to lock m1 → deadlock!
}

// SAFE: scoped_lock (locks both simultaneously):
void good() {
    std::scoped_lock lock(m1, m2);  // Locks both atomically — no deadlock!
    // ...
}

// SAFE: std::lock + adopt_lock:
void also_good() {
    std::lock(m1, m2);  // Lock both at once (no deadlock)
    std::lock_guard<std::mutex> g1(m1, std::adopt_lock);  // Adopt ownership
    std::lock_guard<std::mutex> g2(m2, std::adopt_lock);
    // ...
}
```

---

## 10.10 `std::shared_mutex` — Reader-Writer Lock

```cpp
#include <shared_mutex>

std::shared_mutex rw_mutex;
std::unordered_map<int, std::string> cache;

// Multiple readers can hold shared_lock simultaneously:
std::string read_value(int key) {
    std::shared_lock<std::shared_mutex> lock(rw_mutex);
    // Many threads can read at the same time
    return cache.at(key);
}

// Only ONE writer can hold unique_lock at a time (exclusive):
void write_value(int key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(rw_mutex);
    // While writing: ALL readers and writers are blocked
    cache[key] = value;
}

// Use when: Many reads, few writes
// The shared_mutex allows concurrent reads but exclusive writes
```

---

## 10.11 `std::call_once` and Singletons

```cpp
#include <mutex>  // for std::once_flag, std::call_once

std::once_flag flag;
std::unique_ptr<Database> db;

void initialize_database() {
    db = std::make_unique<Database>("connection_string");
}

Database& get_database() {
    std::call_once(flag, initialize_database);  // Called EXACTLY once, thread-safe
    return *db;
}

// Thread-safe singleton (C++11 and later):
class Singleton {
    Singleton() {}  // Private constructor
public:
    static Singleton& instance() {
        static Singleton inst;  // Initialized exactly once, thread-safe (C++11!)
        return inst;            // Static local initialization is thread-safe!
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};
```

---

## 📝 Module 10 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| `std::thread` | Create threads; always join() or detach() |
| Data race | Two threads access same memory, one writes → UB |
| `std::mutex` | Exclusive lock; always use RAII wrappers |
| `lock_guard` | RAII lock; simplest, non-movable |
| `unique_lock` | RAII lock; flexible, movable |
| `scoped_lock` | Lock multiple mutexes safely (C++17) |
| `std::atomic` | Lock-free thread-safe operations for simple types |
| Condition variable | Efficient waiting for a condition |
| `std::future/promise` | Pass values between threads |
| `std::async` | High-level async execution |
| `thread_local` | Per-thread storage |
| `shared_mutex` | Reader-writer lock; concurrent reads |
| Deadlock | Avoid with consistent lock ordering or `scoped_lock` |

---

## ➡️ Next: [Module 11: Error Handling & Exceptions](../module_11_error_handling/11_errors.md)
