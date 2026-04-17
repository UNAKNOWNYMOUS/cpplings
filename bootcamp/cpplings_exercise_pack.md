# Cpplings Exercise Pack Notes

## How to use this pack

These are written in a **Rustlings-style** spirit:
- each exercise should be small and single-purpose
- most should start in a broken or incomplete state
- the learner should fix code, not just read explanations
- tests should check one main concept
- the answer key comes at the end

Recommended folder scheme:

```text
cpplings/
  exercises/
    00_under_the_hood/
    01_types/
    02_functions/
    03_pointers/
    04_memory/
    05_classes/
    06_polymorphism/
    07_operators/
    08_templates/
    09_stl/
    10_move/
    11_raii/
    12_threads/
    13_modern/
    14_ub/
    15_capstones/
```

---

## 00 — Under the Hood

### Exercise 00.1 — `linker_error`
**Goal:** Understand declaration vs definition.

**Prompt:** This should compile but fail to link. Explain why, then fix it.

```cpp
// main.cpp
void greet();

int main() {
    greet();
}
```

**Learner task:** Add the missing definition in the correct place.

---

### Exercise 00.2 — `memory_segments`
**Goal:** Identify storage regions.

**Prompt:** Label where each object lives: text, data, BSS, stack, or heap.

```cpp
int global_init = 42;
int global_uninit;

void foo() {
    int local = 7;
    int* p = new int(9);
    delete p;
}
```

**Learner task:** Fill comments or a small markdown answer file.

---

## 01 — Types and Variables

### Exercise 01.1 — `signed_unsigned_surprise`
**Goal:** Understand integer promotions.

```cpp
#include <iostream>

int main() {
    unsigned int a = 5;
    int b = -3;

    std::cout << (a + b) << "\n";
    std::cout << (a > b) << "\n";
}
```

**Learner task:** Predict the output, then rewrite the code so the comparison is intuitive.

---

### Exercise 01.2 — `padding_cleanup`
**Goal:** Reduce struct size by reordering fields.

```cpp
struct Wasteful {
    bool flag;
    double value;
    int count;
    char code;
};
```

**Learner task:** Rearrange members to minimize size without changing meaning.

---

## 02 — Functions and Call Stack

### Exercise 02.1 — `no_return_type_overload`
**Goal:** Learn why return type alone cannot overload.

```cpp
int get();
double get();

int main() {
    return 0;
}
```

**Learner task:** Make the design valid while keeping both behaviors available.

---

### Exercise 02.2 — `default_args_vs_overload`
**Goal:** Spot ambiguity.

```cpp
#include <string>

void connect(const std::string& host, int port = 80);
void connect(const std::string& host);

int main() {
    connect("example.com");
}
```

**Learner task:** Remove the ambiguity with the smallest clean change.

---

## 03 — Pointers and References

### Exercise 03.1 — `dangling_local`
**Goal:** Recognize dangling pointers.

```cpp
int* bad_pointer() {
    int x = 42;
    return &x;
}
```

**Learner task:** Rewrite this in two safe ways:
1. returning by value
2. using dynamic allocation ownership correctly

---

### Exercise 03.2 — `swap_two_ways`
**Goal:** Compare pointer and reference APIs.

```cpp
void swap_ptr(int* a, int* b) {
    // TODO
}

void swap_ref(int& a, int& b) {
    // TODO
}
```

**Learner task:** Implement both, then say which API is better here and why.

---

### Exercise 03.3 — `const_pointer_reading`
**Goal:** Read pointer declarations right-to-left.

**Learner task:** Explain the meaning of each.

```cpp
const int* p1;
int* const p2 = nullptr;
const int* const p3 = nullptr;
```

---

## 04 — Dynamic Memory

### Exercise 04.1 — `delete_mismatch`
**Goal:** Match `new`/`delete` correctly.

```cpp
int main() {
    int* arr = new int[5];
    delete arr;
}
```

**Learner task:** Fix the bug and explain why it matters.

---

### Exercise 04.2 — `exception_leak`
**Goal:** Replace manual memory with RAII.

```cpp
#include <stdexcept>

void process(int n) {
    int* data = new int[n];

    if (n > 100) {
        throw std::runtime_error("too big");
    }

    delete[] data;
}
```

**Learner task:** Rewrite using modern C++ so cleanup is always correct.

---

## 05 — Classes and Special Members

### Exercise 05.1 — `rule_of_five_string`
**Goal:** Implement a tiny owning string.

```cpp
class String {
    char* data_;
    std::size_t size_;

public:
    String();
    String(const char* s);
    ~String();

    // TODO: copy ctor
    // TODO: copy assignment
    // TODO: move ctor
    // TODO: move assignment
};
```

**Learner task:** Implement the Big Five correctly.

---

### Exercise 05.2 — `initializer_list_preferred`
**Goal:** Prefer member initializer lists.

```cpp
#include <string>
#include <vector>

class Widget {
    int x;
    std::string name;
    std::vector<int> values;

public:
    Widget(int x_, std::string name_) {
        x = x_;
        name = name_;
    }
};
```

**Learner task:** Rewrite constructor idiomatically.

---

## 06 — Inheritance and Polymorphism

### Exercise 06.1 — `virtual_destructor`
**Goal:** Prevent partial destruction through a base pointer.

```cpp
#include <iostream>

struct Base {
    ~Base() { std::cout << "~Base\n"; }
};

struct Derived : Base {
    int* data = new int[10];
    ~Derived() {
        delete[] data;
        std::cout << "~Derived\n";
    }
};

int main() {
    Base* p = new Derived();
    delete p;
}
```

**Learner task:** Fix the class design.

---

### Exercise 06.2 — `object_slicing`
**Goal:** See why polymorphism needs references or pointers.

```cpp
#include <iostream>

struct Base {
    virtual void speak() const { std::cout << "Base\n"; }
};

struct Derived : Base {
    void speak() const override { std::cout << "Derived\n"; }
};

void say(Base b) {
    b.speak();
}

int main() {
    Derived d;
    say(d);
}
```

**Learner task:** Make it print `Derived` without changing the class hierarchy.

---

## 07 — Operator Overloading

### Exercise 07.1 — `money_plus`
**Goal:** Make `a + b` and `5 + money` both work.

```cpp
struct Money {
    int cents;
    explicit Money(int c) : cents(c) {}
};

// TODO: implement operator+
```

**Learner task:** Support:

```cpp
Money a(100);
Money b(250);
a + b;
Money(50) + a;
```

Bonus: support `2 * money` and `money * 2`.

---

### Exercise 07.2 — `matrix_subscript`
**Goal:** Support `m[i][j]` syntax.

```cpp
class Matrix {
    int data_[2][2]{{1,2},{3,4}};
public:
    // TODO
};
```

**Learner task:** Implement `operator[]` for const and non-const access.

---

## 08 — Templates

### Exercise 08.1 — `template_clamp`
**Goal:** Write a generic clamp.

```cpp
template <typename T>
const T& clamp(const T& value, const T& low, const T& high) {
    // TODO
}
```

**Learner task:** Make it work for `int`, `double`, and `std::string`.

---

### Exercise 08.2 — `template_in_header`
**Goal:** Understand why templates are usually defined in headers.

**Prompt:** You have:

```cpp
// math.h
template <typename T>
T square(T x);
```

```cpp
// math.cpp
template <typename T>
T square(T x) { return x * x; }
```

```cpp
// main.cpp
#include "math.h"
#include <iostream>

int main() {
    std::cout << square(5) << "\n";
}
```

**Learner task:** Fix the project in the most common idiomatic way.

---

## 09 — STL

### Exercise 09.1 — `erase_remove`
**Goal:** Remove all `5`s correctly from a vector.

```cpp
#include <algorithm>
#include <vector>

int main() {
    std::vector<int> v{1,5,2,5,3,5,4};
    std::remove(v.begin(), v.end(), 5);
}
```

**Learner task:** Actually erase the removed values.

---

### Exercise 09.2 — `count_if_over_5`
**Goal:** Use algorithms instead of hand-written loops.

**Learner task:** Replace the loop with a one-liner using STL.

```cpp
#include <vector>

int main() {
    std::vector<int> v{1,7,9,2,4,6};
    int count = 0;

    for (int x : v) {
        if (x > 5) {
            ++count;
        }
    }
}
```

---

### Exercise 09.3 — `reserve_or_reallocate`
**Goal:** Understand `size` vs `capacity`.

**Learner task:** Explain when `reserve(1000)` helps and when it does not.

---

## 10 — Move Semantics

### Exercise 10.1 — `remove_bad_move_return`
**Goal:** Avoid blocking NRVO.

```cpp
#include <string>

std::string make_message() {
    std::string s = "hello";
    return std::move(s);
}
```

**Learner task:** Fix the return statement and explain why.

---

### Exercise 10.2 — `classify_value_categories`
**Goal:** Classify expressions.

Given:

```cpp
int x = 5;
int* p = &x;
```

**Learner task:** Classify each as lvalue, prvalue, or xvalue:
- `x`
- `x + 1`
- `*p`
- `++x`
- `x++`
- `std::move(x)`

---

## 11 — Smart Pointers and RAII

### Exercise 11.1 — `shared_ptr_double_delete`
**Goal:** Avoid creating multiple control blocks.

```cpp
#include <memory>

int main() {
    auto sp1 = std::shared_ptr<int>(new int(5));
    auto sp2 = std::shared_ptr<int>(sp1.get());
}
```

**Learner task:** Fix the ownership bug.

---

### Exercise 11.2 — `build_lock_guard`
**Goal:** Implement a tiny RAII wrapper.

```cpp
#include <mutex>

class LockGuard {
    std::mutex& m_;
public:
    explicit LockGuard(std::mutex& m) : m_(m) {
        // TODO lock
    }

    ~LockGuard() {
        // TODO unlock
    }

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};
```

**Learner task:** Finish the class.

---

### Exercise 11.3 — `break_cycle`
**Goal:** Use `weak_ptr` when appropriate.

```cpp
#include <memory>

struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;
};
```

**Learner task:** Fix the cycle risk.

---

## 12 — Concurrency

### Exercise 12.1 — `data_race_counter`
**Goal:** Fix a race condition.

```cpp
#include <thread>
#include <vector>

int counter = 0;

void work() {
    for (int i = 0; i < 100000; ++i) {
        ++counter;
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(work);
    }
    for (auto& t : threads) {
        t.join();
    }
}
```

**Learner task:** Fix it once with a mutex and once with an atomic.

---

### Exercise 12.2 — `join_or_terminate`
**Goal:** Remember thread lifetime rules.

```cpp
#include <thread>

int main() {
    std::thread t([]{});
}
```

**Learner task:** Explain what happens and fix it.

---

## 13 — Modern C++

### Exercise 13.1 — `lambda_capture_bug`
**Goal:** Notice dangling reference capture.

```cpp
#include <functional>

std::function<int()> make_func() {
    int x = 42;
    return [&]() { return x; };
}
```

**Learner task:** Fix the lifetime bug.

---

### Exercise 13.2 — `string_view_lifetime`
**Goal:** Respect non-owning views.

```cpp
#include <string>
#include <string_view>

std::string_view bad() {
    std::string s = "hello";
    return s;
}
```

**Learner task:** Fix it in two different ways.

---

### Exercise 13.3 — `if_constexpr_or_error`
**Goal:** Use `if constexpr` to avoid invalid branches.

```cpp
#include <string>
#include <type_traits>

template <typename T>
auto stringify(T value) {
    if (std::is_same_v<T, bool>) {
        return value ? std::string("true") : std::string("false");
    } else {
        return std::to_string(value);
    }
}
```

**Learner task:** Make this work for `bool`, `int`, and `double` without invalid code paths being compiled.

---

### Exercise 13.4 — `visit_variant`
**Goal:** Use `std::variant` safely.

```cpp
#include <iostream>
#include <variant>

int main() {
    std::variant<int, double> v = 3.5;
    // TODO print the stored value without guessing the active type
}
```

---

## 14 — Undefined Behavior

### Exercise 14.1 — `use_after_free`
**Goal:** Recognize UB immediately.

```cpp
int main() {
    int* p = new int(5);
    delete p;
    *p = 10;
}
```

**Learner task:** Explain why this is UB and rewrite safely.

---

### Exercise 14.2 — `out_of_bounds`
**Goal:** Stop indexing past the end.

```cpp
#include <array>

int main() {
    std::array<int, 5> a{1,2,3,4,5};
    return a[5];
}
```

**Learner task:** Fix the bug and say why sanitizers help here.

---

### Exercise 14.3 — `signed_overflow`
**Goal:** Understand that signed overflow is UB.

```cpp
#include <climits>

int main() {
    int x = INT_MAX;
    return x + 1;
}
```

**Learner task:** Rewrite safely.

---

## 15 — Capstones

### Exercise 15.1 — `mini_string`
**Goal:** Build a small owning string type.

**Required features:**
- default ctor
- `const char*` ctor
- destructor
- copy ctor
- copy assignment
- move ctor
- move assignment
- `size()`
- `c_str()`
- `operator[]`

**Stretch:** `operator+=`, `operator+`, stream output.

---

### Exercise 15.2 — `mini_vector`
**Goal:** Build a tiny vector.

**Required features:**
- dynamic storage
- `size()` / `capacity()`
- `push_back`
- `reserve`
- `operator[]`
- destructor
- copy/move support

**Stretch:** iterators, `pop_back`, `emplace_back`.

---

### Exercise 15.3 — `thread_safe_queue`
**Goal:** Build a producer-consumer queue.

**Required features:**
- internal `std::queue<T>`
- `std::mutex`
- `std::condition_variable`
- `push`
- blocking `pop`

**Stretch:** `try_pop`, shutdown flag, move-only support.

---

# Answer Key

## 00.1 — `linker_error`
A declaration promises a symbol exists; a definition provides the symbol. This compiles because `greet()` is declared, but it fails to link unless you add:

```cpp
void greet() {}
```

in some compiled translation unit.

## 00.2 — `memory_segments`
- `global_init` → data segment
- `global_uninit` → BSS
- `foo` machine code → text segment
- `local` → stack
- `p` itself → stack
- `*p` target object → heap

## 01.1 — `signed_unsigned_surprise`
Typical output on 32-bit `unsigned int` arithmetic:

```text
2
0
```

Why: `b` converts to unsigned before arithmetic/comparison.
A cleaner fix is to avoid mixed signed/unsigned math, for example:

```cpp
int a = 5;
int b = -3;
std::cout << (a + b) << "\n";
std::cout << (a > b) << "\n";
```

## 01.2 — `padding_cleanup`
Better layout:

```cpp
struct Better {
    double value;
    int count;
    bool flag;
    char code;
};
```

This minimizes padding by placing widest members first.

## 02.1 — `no_return_type_overload`
Return type alone cannot distinguish overloads. Fix by changing parameter lists or names:

```cpp
int get_int();
double get_double();
```

## 02.2 — `default_args_vs_overload`
The call is ambiguous because `connect("example.com")` matches both declarations. Remove one overload, usually the one with fewer parameters:

```cpp
void connect(const std::string& host, int port = 80);
```

## 03.1 — `dangling_local`
Safe return-by-value:

```cpp
int good_value() {
    int x = 42;
    return x;
}
```

Safe owning heap version:

```cpp
#include <memory>

std::unique_ptr<int> good_ptr() {
    return std::make_unique<int>(42);
}
```

## 03.2 — `swap_two_ways`
```cpp
void swap_ptr(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_ref(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
```

References are cleaner here because null is not meaningful and the call site is simpler.

## 03.3 — `const_pointer_reading`
- `const int* p1;` → pointer to const int
- `int* const p2 = nullptr;` → const pointer to int
- `const int* const p3 = nullptr;` → const pointer to const int

## 04.1 — `delete_mismatch`
Use `delete[]`:

```cpp
int* arr = new int[5];
delete[] arr;
```

`new[]` and `delete[]` must match.

## 04.2 — `exception_leak`
Best fix:

```cpp
#include <stdexcept>
#include <vector>

void process(int n) {
    std::vector<int> data(n);
    if (n > 100) {
        throw std::runtime_error("too big");
    }
}
```

RAII guarantees cleanup on exceptions.

## 05.1 — `rule_of_five_string`
One correct sketch:

```cpp
#include <cstring>
#include <utility>

class String {
    char* data_{};
    std::size_t size_{};

public:
    String() : data_(new char[1]{'\0'}), size_(0) {}

    String(const char* s) : size_(std::strlen(s)) {
        data_ = new char[size_ + 1];
        std::memcpy(data_, s, size_ + 1);
    }

    ~String() { delete[] data_; }

    String(const String& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        std::memcpy(data_, other.data_, size_ + 1);
    }

    String& operator=(const String& other) {
        if (this != &other) {
            char* new_data = new char[other.size_ + 1];
            std::memcpy(new_data, other.data_, other.size_ + 1);
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
        }
        return *this;
    }

    String(String&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = new char[1]{'\0'};
        other.size_ = 0;
    }

    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = new char[1]{'\0'};
            other.size_ = 0;
        }
        return *this;
    }
};
```

## 05.2 — `initializer_list_preferred`
```cpp
Widget(int x_, std::string name_)
    : x(x_), name(std::move(name_)), values() {}
```

## 06.1 — `virtual_destructor`
Make the base destructor virtual:

```cpp
struct Base {
    virtual ~Base() { std::cout << "~Base\n"; }
};
```

Then deleting through `Base*` correctly runs `Derived::~Derived()` first.

## 06.2 — `object_slicing`
Pass by reference or pointer, not by value:

```cpp
void say(const Base& b) {
    b.speak();
}
```

Now `say(d)` prints `Derived`.

## 07.1 — `money_plus`
A clean answer:

```cpp
struct Money {
    int cents;
    explicit Money(int c) : cents(c) {}
};

Money operator+(Money a, Money b) {
    return Money(a.cents + b.cents);
}
```

For `2 * money` and `money * 2`:

```cpp
Money operator*(Money m, int n) { return Money(m.cents * n); }
Money operator*(int n, Money m) { return Money(m.cents * n); }
```

## 07.2 — `matrix_subscript`
```cpp
class Matrix {
    int data_[2][2]{{1,2},{3,4}};
public:
    int* operator[](std::size_t row) { return data_[row]; }
    const int* operator[](std::size_t row) const { return data_[row]; }
};
```

## 08.1 — `template_clamp`
```cpp
template <typename T>
const T& clamp(const T& value, const T& low, const T& high) {
    return value < low ? low : (value > high ? high : value);
}
```

## 08.2 — `template_in_header`
Move the definition into the header:

```cpp
// math.h
template <typename T>
T square(T x) {
    return x * x;
}
```

Templates usually need full definitions visible at the point of instantiation.

## 09.1 — `erase_remove`
```cpp
v.erase(std::remove(v.begin(), v.end(), 5), v.end());
```

## 09.2 — `count_if_over_5`
```cpp
#include <algorithm>

int count = std::count_if(v.begin(), v.end(), [](int x) { return x > 5; });
```

## 09.3 — `reserve_or_reallocate`
`reserve(1000)` increases capacity so repeated `push_back` operations up to 1000 elements avoid reallocations. It does **not** change size, and it does **not** help if you are not growing the vector.

## 10.1 — `remove_bad_move_return`
Use:

```cpp
return s;
```

Do not `std::move` a local return object here because it can inhibit NRVO/copy elision.

## 10.2 — `classify_value_categories`
- `x` → lvalue
- `x + 1` → prvalue
- `*p` → lvalue
- `++x` → lvalue
- `x++` → prvalue
- `std::move(x)` → xvalue

## 11.1 — `shared_ptr_double_delete`
Correct fix:

```cpp
auto sp1 = std::make_shared<int>(5);
auto sp2 = sp1;
```

The broken version creates two unrelated control blocks for the same raw pointer.

## 11.2 — `build_lock_guard`
```cpp
class LockGuard {
    std::mutex& m_;
public:
    explicit LockGuard(std::mutex& m) : m_(m) { m_.lock(); }
    ~LockGuard() { m_.unlock(); }

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};
```

## 11.3 — `break_cycle`
Use `weak_ptr` for one direction:

```cpp
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;
};
```

## 12.1 — `data_race_counter`
Mutex version:

```cpp
#include <mutex>

int counter = 0;
std::mutex m;

void work() {
    for (int i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lock(m);
        ++counter;
    }
}
```

Atomic version:

```cpp
#include <atomic>

std::atomic<int> counter = 0;

void work() {
    for (int i = 0; i < 100000; ++i) {
        ++counter;
    }
}
```

## 12.2 — `join_or_terminate`
A `std::thread` destroyed while still joinable causes `std::terminate()`. Fix by either:

```cpp
t.join();
```

or, less commonly:

```cpp
t.detach();
```

## 13.1 — `lambda_capture_bug`
Capture by value:

```cpp
std::function<int()> make_func() {
    int x = 42;
    return [x]() { return x; };
}
```

## 13.2 — `string_view_lifetime`
Safe fix returning owning string:

```cpp
std::string good() {
    std::string s = "hello";
    return s;
}
```

Safe fix returning a view to static storage:

```cpp
std::string_view good_view() {
    return "hello";
}
```

## 13.3 — `if_constexpr_or_error`
```cpp
#include <string>
#include <type_traits>

template <typename T>
auto stringify(T value) {
    if constexpr (std::is_same_v<T, bool>) {
        return value ? std::string("true") : std::string("false");
    } else {
        return std::to_string(value);
    }
}
```

The key fix is `if constexpr`, not ordinary `if`.

## 13.4 — `visit_variant`
```cpp
std::visit([](const auto& x) {
    std::cout << x << "\n";
}, v);
```

## 14.1 — `use_after_free`
It is UB because the object lifetime ended at `delete p;`. Safe rewrite:

```cpp
int main() {
    int value = 5;
    value = 10;
}
```

Or, if dynamic lifetime is needed, use `std::unique_ptr<int>`.

## 14.2 — `out_of_bounds`
Valid indices are `0..4`. Fix by using `a[4]` or a checked access pattern like `a.at(4)` if using `std::vector`. Sanitizers help catch out-of-bounds accesses during testing.

## 14.3 — `signed_overflow`
Safer alternatives:
- check before incrementing
- use a wider type
- use unsigned only if wraparound is intentionally part of the design

Example:

```cpp
#include <climits>

int main() {
    int x = INT_MAX;
    if (x < INT_MAX) {
        return x + 1;
    }
    return x;
}
```

## 15.1 — `mini_string`
The important mastery points are:
- deep copy, not shallow copy
- moved-from object must remain valid
- destructor must always match allocation
- avoid leaks on copy assignment

## 15.2 — `mini_vector`
The important mastery points are:
- track `size_` and `capacity_` separately
- reallocate on growth
- move elements during reallocation when possible
- preserve strong exception safety where practical

## 15.3 — `thread_safe_queue`
The important mastery points are:
- always hold the mutex when touching the queue
- use `condition_variable::wait` with a predicate
- notify after `push`
- think carefully about shutdown and spurious wakeups

---

If you want the next step, I can turn this into an actual **cpplings directory layout** with one markdown/spec file per exercise plus suggested test cases.
