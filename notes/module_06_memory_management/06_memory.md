# Module 06: Memory Management — Stack, Heap, RAII & Smart Pointers

> *"In C++, you have complete control over memory. This is both the greatest power and the greatest responsibility."*

---

## 6.1 Raw Memory Operations

### `new` and `delete`

```cpp
// Allocate a single object on the heap:
int* p = new int(42);     // Allocates 4 bytes, initializes to 42
double* d = new double;   // Allocates 8 bytes, uninitialized!
std::cout << *p;          // 42
delete p;                  // Free the memory
p = nullptr;               // Avoid dangling pointer

// Allocate an array on the heap:
int* arr = new int[10];   // Allocate 10 ints (40 bytes)
arr[0] = 1;
arr[9] = 9;
delete[] arr;             // MUST use delete[] for arrays!
arr = nullptr;
// delete arr;             // WRONG! delete without [] → UB!

// Value-initialize (zero-initialize):
int* zero_arr = new int[10]();  // All elements = 0
delete[] zero_arr;
```

### Common Memory Bugs

```
┌─────────────────────────────────────────────────────────────────────┐
│               COMMON MEMORY BUGS                                     │
│                                                                       │
│  1. MEMORY LEAK: new without delete                                  │
│     int* p = new int(5);                                            │
│     // forgot delete p;  → 4 bytes leaked forever!                  │
│                                                                       │
│  2. DANGLING POINTER: using after delete                             │
│     int* p = new int(5);                                            │
│     delete p;                                                        │
│     *p = 10;  // UB! Memory may have been reused                   │
│                                                                       │
│  3. DOUBLE FREE: deleting twice                                      │
│     int* p = new int(5);                                            │
│     delete p;                                                        │
│     delete p;  // UB! Corrupts heap metadata                        │
│                                                                       │
│  4. BUFFER OVERFLOW: writing past array bounds                       │
│     int* arr = new int[5];                                          │
│     arr[5] = 10;  // UB! Writes past end of array                  │
│                                                                       │
│  5. MISMATCH: delete vs delete[]                                     │
│     int* arr = new int[5];                                          │
│     delete arr;  // WRONG! Should be delete[]                       │
│                                                                       │
│  6. UNINITIALIZED MEMORY: reading garbage                           │
│     int* p = new int;                                               │
│     std::cout << *p;  // UB! Could be any value                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 6.2 RAII — Resource Acquisition Is Initialization

**RAII is the most important C++ idiom.** It solves memory management through object lifetimes.

**The principle:**
- Acquire resources in the constructor
- Release resources in the destructor
- Resources are automatically released when the object goes out of scope

```cpp
// WITHOUT RAII (fragile):
void load_and_process(const std::string& filename) {
    FILE* f = fopen(filename.c_str(), "r");
    
    if (!f) return;  // OK, nothing to free
    
    if (some_condition()) {
        // Forgot to fclose(f)! LEAK!
        return;
    }
    
    process(f);
    
    if (another_condition()) {
        throw std::runtime_error("failed");  // LEAK! Exception skips fclose
    }
    
    fclose(f);  // Only reached if no early returns or exceptions
}

// WITH RAII (safe):
class FileHandle {
    FILE* file;
public:
    FileHandle(const char* path, const char* mode) {
        file = fopen(path, mode);
        if (!file) throw std::runtime_error("Cannot open file");
    }
    ~FileHandle() {
        if (file) fclose(file);  // ALWAYS called, no matter what!
    }
    FILE* get() { return file; }
    
    // Disable copy (file handles shouldn't be copied)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
};

void load_and_process(const std::string& filename) {
    FileHandle f(filename.c_str(), "r");  // Acquired in constructor
    
    if (some_condition()) return;   // ~FileHandle() called: file closed!
    
    process(f.get());
    
    if (another_condition()) throw std::runtime_error("failed"); // File closed!
    
    // File automatically closed when f goes out of scope
}
```

---

## 6.3 Smart Pointers — RAII for Dynamic Memory

C++11 introduced smart pointers that automatically manage heap memory.

```
┌──────────────────────────────────────────────────────────────────────┐
│                    SMART POINTER COMPARISON                           │
│                                                                        │
│  unique_ptr<T>  │ One owner only   │ Move-only │ Zero overhead       │
│  shared_ptr<T>  │ Many owners      │ Copyable  │ Reference counting  │
│  weak_ptr<T>    │ Observes shared  │ Copyable  │ Doesn't own         │
└──────────────────────────────────────────────────────────────────────┘
```

### `std::unique_ptr` — Exclusive Ownership

```cpp
#include <memory>

// Create (prefer make_unique over raw new):
auto p = std::make_unique<int>(42);        // Better than: new int(42)
auto arr = std::make_unique<int[]>(10);    // Array version

// Use like a regular pointer:
std::cout << *p << "\n";    // 42
*p = 100;

// OWNERSHIP: unique_ptr CANNOT be copied, only MOVED
std::unique_ptr<int> q = p;              // ERROR: cannot copy unique_ptr
std::unique_ptr<int> r = std::move(p);   // OK: move ownership
// p is now nullptr; r owns the int

// Automatic cleanup:
{
    auto ptr = std::make_unique<std::string>("hello");
    std::cout << *ptr << "\n";
}   // ptr goes out of scope → "hello" string deleted automatically!

// Returning from functions (safe!):
std::unique_ptr<Widget> create_widget() {
    auto w = std::make_unique<Widget>(args);
    setup(w);
    return w;  // Ownership transferred to caller
}

// Polymorphism:
std::unique_ptr<Animal> animal = std::make_unique<Dog>("Rex", 3, "Lab");
animal->speak();  // Dog::speak() via virtual dispatch

// Custom deleter:
auto closer = [](FILE* f) { fclose(f); };
std::unique_ptr<FILE, decltype(closer)> file(fopen("file.txt", "r"), closer);
```

### `std::shared_ptr` — Shared Ownership

```cpp
// Multiple owners — uses reference counting
auto p1 = std::make_shared<int>(42);
auto p2 = p1;   // Both p1 and p2 own the int
auto p3 = p2;   // Now 3 owners

std::cout << p1.use_count() << "\n";  // 3

p1.reset();   // p1 releases its ownership
std::cout << p2.use_count() << "\n";  // 2

p2.reset();
// use_count = 1 (only p3)

p3.reset();
// use_count = 0 → int is deleted!

// Shared ownership of a resource:
class Database {
    // ...
};

class UserService {
    std::shared_ptr<Database> db;  // UserService SHARES the database
public:
    UserService(std::shared_ptr<Database> d) : db{std::move(d)} {}
};

class OrderService {
    std::shared_ptr<Database> db;
public:
    OrderService(std::shared_ptr<Database> d) : db{std::move(d)} {}
};

auto db = std::make_shared<Database>();
UserService users(db);
OrderService orders(db);
// Both services share the database; it's deleted when both are destroyed
```

### `shared_ptr` Internal Structure

```
┌──────────────────────────────────────────────────────────────────────┐
│                  shared_ptr INTERNALS                                  │
│                                                                        │
│  auto p = make_shared<int>(42);                                       │
│                                                                        │
│  p:                                                                    │
│  ┌────────────────┐                                                   │
│  │  ptr ──────────┼──→  Control Block + Data                         │
│  │  ctrl_blk ─────┼──→  ┌──────────────────────┐                    │
│  └────────────────┘      │  strong_count = 1     │                    │
│                          │  weak_count   = 0     │                    │
│                          │  deleter              │                    │
│                          │  allocator            │                    │
│                          │  [actual data: 42]    │ (with make_shared) │
│                          └──────────────────────┘                    │
│                                                                        │
│  p2 = p;                                                              │
│  ┌────────────────┐                                                   │
│  │  ptr ──────────┼──→  Same data                                    │
│  │  ctrl_blk ─────┼──→  ┌──────────────────────┐                    │
│  └────────────────┘      │  strong_count = 2 ← incremented           │
│                          └──────────────────────┘                    │
│                                                                        │
│  When strong_count reaches 0: data is deleted                        │
│  When weak_count also reaches 0: control block is deleted            │
└──────────────────────────────────────────────────────────────────────┘
```

### `std::weak_ptr` — Non-Owning Observer

```cpp
// weak_ptr doesn't affect reference count
// Used to OBSERVE a shared_ptr without owning

std::shared_ptr<int> sp = std::make_shared<int>(42);
std::weak_ptr<int> wp = sp;  // Observe but don't own

std::cout << sp.use_count() << "\n";  // 1 (weak doesn't count!)

// To use a weak_ptr, must lock it first:
if (auto locked = wp.lock()) {  // lock() returns a shared_ptr
    // If sp still alive: locked is valid shared_ptr
    std::cout << *locked << "\n";  // 42
} else {
    // sp was destroyed before we could lock
    std::cout << "Object expired!\n";
}

sp.reset();  // Destroy the shared_ptr
if (auto locked = wp.lock()) {
    // Never reached
} else {
    std::cout << "Expired!\n";  // This runs
}

// WHEN TO USE weak_ptr:
// 1. Breaking cyclic references (the classic use case)
struct Node {
    int value;
    std::shared_ptr<Node> next;    // Owns next
    std::weak_ptr<Node> prev;      // Observes prev (no cycle!)
};

// Without weak_ptr: shared_ptr cycle → memory never freed!
// With weak_ptr: cycle broken, memory correctly freed
```

### The Cycle Problem

```
┌──────────────────────────────────────────────────────────────────────┐
│           SHARED_PTR CYCLE (MEMORY LEAK!)                             │
│                                                                        │
│  struct Node { shared_ptr<Node> next; };                              │
│                                                                        │
│  auto a = make_shared<Node>();  // count=1                            │
│  auto b = make_shared<Node>();  // count=1                            │
│  a->next = b;  // b's count=2                                        │
│  b->next = a;  // a's count=2                                        │
│                                                                        │
│  a and b go out of scope:                                             │
│  a destroyed → b's count: 2→1 (not 0! still referenced by b->next→a)│
│  b destroyed → a's count: 2→1 (not 0! still referenced by a->next→b)│
│                                                                        │
│  RESULT: Both objects have count=1 but no external references.       │
│  They keep each other alive FOREVER → MEMORY LEAK!                   │
│                                                                        │
│  FIX: Make one of them a weak_ptr:                                   │
│  struct Node { shared_ptr<Node> next; weak_ptr<Node> prev; };        │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 6.4 Arrays — C-Style vs Modern

```cpp
// C-STYLE ARRAY (on stack):
int arr[5] = {1, 2, 3, 4, 5};
arr[0] = 10;
// sizeof(arr) = 20 (5 * 4 bytes)
// arr DECAYS to a pointer when passed to a function!

void bad(int* arr, int size) {  // size lost! can't tell how long arr is
    // sizeof(arr) = 8 (pointer size on 64-bit)!!! Not 20!
}

// MODERN: std::array (fixed size, no decay, knows its size):
#include <array>
std::array<int, 5> a = {1, 2, 3, 4, 5};
a[0] = 10;
std::cout << a.size() << "\n";  // 5 (always known)
// Passed by reference — no decay:
void good(const std::array<int, 5>& arr) {
    std::cout << arr.size() << "\n";  // 5
}

// MODERN: std::vector (dynamic size, heap-allocated):
#include <vector>
std::vector<int> v = {1, 2, 3, 4, 5};
v.push_back(6);        // Grows as needed
v.pop_back();          // Remove last
std::cout << v.size(); // 5
std::cout << v.capacity(); // Actual allocated (≥ size)
```

### How `std::vector` Grows

```
┌──────────────────────────────────────────────────────────────────────┐
│                 VECTOR GROWTH STRATEGY                                │
│                                                                        │
│  v starts:  capacity=0, size=0                                        │
│  push_back: capacity=1, size=1   [1]                                 │
│  push_back: capacity=2, size=2   [1][2]                              │
│  push_back: capacity=4, size=3   [1][2][3][_]                        │
│  push_back: capacity=4, size=4   [1][2][3][4]                        │
│  push_back: capacity=8, size=5   [1][2][3][4][5][_][_][_]           │
│                                                                        │
│  Doubling strategy: amortized O(1) push_back                         │
│  Each element pushed_back costs O(1) amortized                       │
│                                                                        │
│  When capacity exceeded:                                               │
│  1. Allocate new larger buffer (2x typical)                          │
│  2. Move all elements to new buffer                                   │
│  3. Free old buffer                                                   │
│  4. Update internal pointers                                          │
│                                                                        │
│  RESERVE to avoid reallocations:                                      │
│  v.reserve(100);  // Pre-allocate for 100 elements                   │
│  // Now push_back 100 times → ZERO reallocations                     │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 6.5 Placement New and Memory Pools

```cpp
#include <new>

// Placement new: construct an object at a specific memory address
char buffer[sizeof(int)];  // Stack-allocated raw memory
int* p = new(buffer) int(42);  // Construct int IN the buffer

// Must manually call destructor (not delete!):
p->~int();  // For primitive types: no-op, but required by standard

// Real use: memory pools, custom allocators
class MemoryPool {
    char* pool;
    size_t offset = 0;
    size_t size;

public:
    MemoryPool(size_t sz) : pool{new char[sz]}, size{sz} {}
    ~MemoryPool() { delete[] pool; }

    template<typename T, typename... Args>
    T* allocate(Args&&... args) {
        void* mem = pool + offset;
        offset += sizeof(T);
        return new(mem) T(std::forward<Args>(args)...);
    }
};
```

---

## 6.6 `std::allocator` and Custom Allocators

```cpp
#include <memory>

// std::allocator: default allocator for STL containers
std::allocator<int> alloc;

// Allocate raw memory (no constructor called):
int* p = alloc.allocate(5);  // 5 * sizeof(int) bytes

// Construct objects:
std::allocator_traits<std::allocator<int>>::construct(alloc, p, 42);

// Destroy objects:
std::allocator_traits<std::allocator<int>>::destroy(alloc, p);

// Deallocate:
alloc.deallocate(p, 5);

// Custom allocator for performance:
template<typename T>
class PoolAllocator {
    // ... pool implementation
public:
    using value_type = T;
    T* allocate(size_t n) { /* from pool */ }
    void deallocate(T* p, size_t n) { /* return to pool */ }
};

std::vector<int, PoolAllocator<int>> fast_vec;
```

---

## 6.7 Memory Detection Tools

```bash
# Valgrind: find leaks, invalid access, UB
valgrind --leak-check=full --track-origins=yes ./program

# AddressSanitizer (ASan): compile-time instrumentation (faster than valgrind)
g++ -fsanitize=address -fno-omit-frame-pointer -g program.cpp -o program
./program

# MemorySanitizer (MSan): uninitialized memory reads
g++ -fsanitize=memory -g program.cpp -o program

# UBSanitizer (UBSan): undefined behavior detection
g++ -fsanitize=undefined -g program.cpp -o program

# ThreadSanitizer (TSan): data race detection
g++ -fsanitize=thread -g program.cpp -o program

# Use ALL sanitizers in debug builds:
g++ -fsanitize=address,undefined -g -O1 program.cpp -o program
```

---

## 6.8 `std::span` (C++20) — Non-Owning View of Contiguous Data

```cpp
#include <span>

// std::span is a lightweight VIEW over contiguous memory
// No ownership, no copying — just a pointer + size

void process(std::span<int> data) {
    for (int x : data) {
        std::cout << x << " ";
    }
}

int arr[] = {1, 2, 3, 4, 5};
std::vector<int> v = {1, 2, 3, 4, 5};
std::array<int, 5> a = {1, 2, 3, 4, 5};

process(arr);    // Works with C arrays
process(v);      // Works with vector
process(a);      // Works with array
process({arr, 3}); // Works with first 3 elements

// Much better than: process(int* arr, int size)!
```

---

## 6.9 Stack vs Heap: When to Choose Each

```
┌─────────────────────────────────────────────────────────────────────┐
│              STACK vs HEAP DECISION GUIDE                            │
│                                                                       │
│  USE STACK WHEN:                USE HEAP (smart pointer) WHEN:       │
│  ─────────────────────────      ────────────────────────────────     │
│  ✓ Size known at compile time   ✓ Size known only at runtime        │
│  ✓ Object lifetime tied to      ✓ Object lifetime transcends a      │
│    current function scope         function call                      │
│  ✓ Small object (<1MB)          ✓ Large object (>MB)               │
│  ✓ No need to share ownership   ✓ Multiple owners needed           │
│  ✓ Performance critical         ✓ Polymorphism needed              │
│                                                                       │
│  RULE: Prefer stack. Use heap only when needed.                      │
│  NEVER use raw new/delete. Always use RAII or smart pointers.       │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 6.10 `std::string` and String Management

```cpp
#include <string>
#include <string_view>

// std::string: heap-allocated, mutable, owns memory
std::string s = "Hello, World!";
s += " Goodbye!";
s.size();       // 22
s.length();     // 22 (same as size)
s.empty();      // false
s.clear();      // empty now
s.append("new content");
s.substr(0, 5);   // "Hello"
s.find("World");  // position of "World"
s.replace(7, 5, "C++");  // Replace "World" with "C++"

// C++17: std::string_view — lightweight, non-owning view of a string
// No heap allocation, no copy, safe to pass around
std::string_view sv = "Hello";  // Points to string literal
std::string_view sv2 = s;       // Points to s (doesn't own)
// Never store string_view when the original string might be destroyed!

void print_name(std::string_view name) {  // Better than const std::string&
    std::cout << name;                    // Works with string, char*, literals
}

print_name("Alice");          // String literal — no copy
print_name(std::string("Bob")); // std::string — no copy
print_name("Charlie");         // C string — no copy

// Small String Optimization (SSO):
// std::string typically uses a small buffer (15-22 chars on 64-bit)
// If string <= SSO limit, stored INLINE in the string object (no heap!)
std::string short_str = "Hi";   // On stack — no heap allocation!
std::string long_str = "This is a very long string that exceeds the SSO";  // Heap
```

---

## 📝 Module 06 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| RAII | Tie resource lifetime to object lifetime; always the right approach |
| `unique_ptr` | One owner; move-only; zero overhead vs raw pointer |
| `shared_ptr` | Multiple owners; reference counted; some overhead |
| `weak_ptr` | Non-owning observer; breaks cycles |
| Memory bugs | Leak, dangling, double-free, overflow — sanitizers catch these |
| `make_unique` / `make_shared` | Always prefer over raw `new` |
| `std::vector` | Preferred dynamic array; doubling growth strategy |
| `std::span` | Non-owning view over contiguous data |
| `string_view` | Non-owning string view; better than `const string&` in many cases |
| Placement new | Construct in existing memory; for custom allocators |

---

## ➡️ Next: [Module 07: Templates & Generic Programming](../module_07_templates/07_templates.md)
