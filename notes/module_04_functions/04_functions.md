# Module 04: Functions — Deep Dive

> *"Functions are the atoms of program structure. Mastering them means mastering how C++ thinks."*

---

## 4.1 Function Anatomy

```cpp
// ANATOMY OF A C++ FUNCTION:
//
//  return_type  function_name  (  parameter_list  )  body
//       ↓              ↓                ↓               ↓
       int          add         (int a, int b)   { return a + b; }

// More complex example:
const std::string& get_name(const Person& p, bool full_name = true) {
    return full_name ? p.full_name : p.first_name;
}
```

### The Call Stack — What Happens When You Call a Function

```
┌─────────────────────────────────────────────────────────────────┐
│                    CALL STACK DIAGRAM                            │
│                                                                   │
│  int main() {                                                    │
│      int r = add(3, 4);  ←── CALL                              │
│      ...                                                         │
│  }                                                               │
│                                                                   │
│  STACK (grows down):                                             │
│                                                                   │
│  Before call:        During call:        After return:           │
│  ┌──────────┐        ┌──────────┐        ┌──────────┐           │
│  │  main()  │        │  add()   │        │  main()  │           │
│  │  frame   │        │  a = 3   │        │  frame   │           │
│  │          │        │  b = 4   │        │  r = 7   │           │
│  └──────────┘        │  return  │        └──────────┘           │
│                       │  addr   │                                │
│                       ├──────────┤                               │
│                       │  main()  │                               │
│                       │  frame   │                               │
│                       └──────────┘                               │
│                                                                   │
│  Steps:                                                          │
│  1. Push arguments (3, 4) onto stack (or put in registers)      │
│  2. Push return address (next instruction in main)              │
│  3. Jump to add() entry point                                    │
│  4. add() creates its stack frame                               │
│  5. add() computes, puts result in register (rax/eax)           │
│  6. add() pops its frame, jumps to return address               │
│  7. main() reads result from register                           │
└─────────────────────────────────────────────────────────────────┘
```

---

## 4.2 Parameter Passing Modes

This is one of the most important topics in C++.

```
┌─────────────────────────────────────────────────────────────────────┐
│              PARAMETER PASSING MODES COMPARISON                      │
│                                                                       │
│  void by_value    (int x)       — Copy made, original unchanged     │
│  void by_ref      (int& x)      — No copy, original can change      │
│  void by_const_ref(const int& x)— No copy, read-only                │
│  void by_pointer  (int* x)      — Pointer to original, can be null  │
│  void by_move     (int&& x)     — Move semantics (C++11)            │
└─────────────────────────────────────────────────────────────────────┘
```

### Pass by Value

```cpp
void double_it(int x) {
    x *= 2;  // Only modifies the LOCAL COPY
}

int main() {
    int n = 5;
    double_it(n);
    std::cout << n;  // Still 5! Original unchanged
}

// When to use:
// - For small, cheap-to-copy types (int, double, bool, char)
// - When you want a local copy to modify
// - When the type is cheap to copy (e.g., std::string_view)
```

### Pass by Reference

```cpp
void double_it(int& x) {
    x *= 2;  // Modifies the ORIGINAL
}

int main() {
    int n = 5;
    double_it(n);
    std::cout << n;  // 10! Original was modified
}

// Swap example (classic reference use case):
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int x = 1, y = 2;
swap(x, y);  // x=2, y=1
```

### Pass by Const Reference (Most Common for Large Objects!)

```cpp
// BAD: Copying a large string is EXPENSIVE (heap allocation!)
void print_name(std::string name) {
    std::cout << name;
}

// GOOD: No copy, but read-only — this is the STANDARD pattern
void print_name(const std::string& name) {
    std::cout << name;
    // name = "other"; // ERROR: can't modify
}

// Use const& for any type larger than a pointer (8 bytes):
void process(const std::vector<int>& data);     // No copy of vector
void render(const std::string& text);           // No copy of string
void transform(const Matrix4x4& m);            // No copy of 4x4 matrix
```

### Pass by Pointer

```cpp
// Use pointers when:
// 1. The argument can be nullptr (optional parameter)
// 2. You need to change WHERE a pointer points
// 3. Interfacing with C code

void process(int* data, int size) {
    if (data == nullptr) return;  // Handle null case
    for (int i = 0; i < size; i++) {
        data[i] *= 2;
    }
}

int arr[] = {1, 2, 3, 4, 5};
process(arr, 5);  // arr elements are modified
process(nullptr, 0);  // Safe: null check
```

### Pass by Rvalue Reference (Move Semantics — C++11)

```cpp
// Covered fully in Module 09, but introduced here:
void consume(std::string&& s) {
    // s is an rvalue reference — we can "steal" its resources
    std::string local = std::move(s);  // No copy! s's buffer moved
    // s is now in a "valid but unspecified" state
}

std::string temp = "Hello";
consume(std::move(temp));  // Explicit move
consume("World");           // String literal is rvalue — works too
```

### The Golden Rules for Parameter Passing

```
┌─────────────────────────────────────────────────────────────────┐
│            PARAMETER PASSING DECISION GUIDE                      │
│                                                                   │
│  Type size?          Input only?    Output?    Decision          │
│  ─────────────────────────────────────────────────────          │
│  Small (≤ 8 bytes)   Yes            No         Pass by value    │
│  Large (> 8 bytes)   Yes            No         const ref        │
│  Any                 No (modify)    Yes        ref              │
│  Any                 Optional null  Maybe      pointer          │
│  Movable (string,    Transfer       Yes        rvalue ref &&    │
│   vector, etc.)      ownership                                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## 4.3 Return Values

```cpp
// Return by value (most common — use NRVO/RVO for optimization)
std::string make_greeting(const std::string& name) {
    return "Hello, " + name + "!";  // Compiler may elide the copy (NRVO)
}

// Return by reference (CAREFUL! Don't return reference to local!)
int& get_element(std::vector<int>& v, int i) {
    return v[i];  // OK: v outlives the function call
}

// DANGER: Never return reference to local variable!
int& dangerous() {
    int local = 42;
    return local;  // DANGLING REFERENCE! UB! local dies here!
}

// Multiple return values: use std::pair, std::tuple, or struct
std::pair<int, int> min_max(const std::vector<int>& v) {
    int mn = *std::min_element(v.begin(), v.end());
    int mx = *std::max_element(v.begin(), v.end());
    return {mn, mx};  // Aggregate initialization
}

auto [mn, mx] = min_max({3,1,4,1,5,9,2,6});  // C++17 structured binding

// Modern: std::optional for "might fail" returns (C++17)
#include <optional>
std::optional<double> safe_sqrt(double x) {
    if (x < 0) return std::nullopt;  // No value
    return std::sqrt(x);
}

if (auto result = safe_sqrt(4.0)) {
    std::cout << *result;  // 2.0
}
```

### Return Value Optimization (RVO/NRVO)

```cpp
// The compiler is ALLOWED (and required in C++17 for some cases)
// to elide copies/moves when returning local objects

std::vector<int> make_vector() {
    std::vector<int> v = {1, 2, 3, 4, 5};  // Created directly in caller's storage
    return v;  // No copy! The vector is constructed IN PLACE at the call site
}

// C++17: Guaranteed copy elision (GCE)
// RValue returned directly is GUARANTEED to not be copied
auto vec = make_vector();  // Zero copies! Guaranteed.
```

---

## 4.4 Default Arguments

```cpp
// Default arguments must be at the END
void print(std::string msg, int times = 1, char sep = '\n') {
    for (int i = 0; i < times; i++) {
        std::cout << msg << sep;
    }
}

print("Hello");           // times=1, sep='\n'
print("Hello", 3);        // times=3, sep='\n'
print("Hello", 3, ' ');   // times=3, sep=' '
// print("Hello", , ' '); // ERROR: can't skip arguments

// Default arguments in declarations (headers):
// Can specify in declaration OR definition, but NOT BOTH
// (ODR rule)
void foo(int x, int y = 10);  // Declaration
void foo(int x, int y) {      // Definition (no default here!)
    ...
}
```

---

## 4.5 Function Overloading

Multiple functions with the **same name** but **different parameter lists**.

```cpp
// Overloaded functions — same name, different signatures
int    add(int a, int b)       { return a + b; }
double add(double a, double b) { return a + b; }
int    add(int a, int b, int c){ return a + b + c; }

// The compiler picks the RIGHT version based on argument types:
add(1, 2);        // Calls int version
add(1.0, 2.0);    // Calls double version
add(1, 2, 3);     // Calls 3-argument version
add(1, 2.0);      // Ambiguous! (int+double → no exact match)
                  // Could convert either way → error!
```

### Overload Resolution Rules

```
┌─────────────────────────────────────────────────────────────────┐
│              OVERLOAD RESOLUTION PRIORITY                        │
│  (Compiler tries in this order until it finds a match)          │
│                                                                   │
│  1. EXACT MATCH: No conversions needed (best!)                  │
│  2. Trivial conversions: T* → const T*, T[] → T*, etc.         │
│  3. Numeric promotions: char→int, float→double                  │
│  4. Standard conversions: int→double, double→int (lossy)        │
│  5. User-defined conversions (constructor/cast operator)        │
│  6. Ellipsis (...) match (worst)                                │
│                                                                   │
│  If multiple functions match at the same priority → AMBIGUOUS   │
└─────────────────────────────────────────────────────────────────┘
```

---

## 4.6 Function Templates (Brief Introduction)

```cpp
// A template function works for any type T
template<typename T>
T add(T a, T b) {
    return a + b;
}

// Compiler generates code for each type used:
add(1, 2);       // Generates: int add(int a, int b)
add(1.0, 2.0);   // Generates: double add(double a, double b)
add<float>(1.0f, 2.0f);  // Explicit instantiation

// Full coverage in Module 07!
```

---

## 4.7 Inline Functions

```cpp
// 'inline' SUGGESTS the compiler to insert function body at call site
// (avoids function call overhead for tiny functions)

inline int square(int x) {
    return x * x;
}

// In practice:
// - Compiler decides whether to actually inline (ignores hint if too complex)
// - 'inline' functions CAN be defined in headers (ODR exception)
// - Modern compilers inline without the keyword anyway (-O2 and above)

// The real use of 'inline': allow multiple definitions across TUs
// inline int global_counter = 0;  // C++17: inline variables!
```

---

## 4.8 Variadic Functions

### Old C-style (Avoid!)

```cpp
#include <cstdarg>

// Old-style: no type safety!
int sum(int count, ...) {
    va_list args;
    va_start(args, count);
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }
    va_end(args);
    return total;
}

sum(3, 1, 2, 3);  // sum = 6
// Problem: passing wrong types is UB!
```

### Modern C++ Variadic Templates (Preferred!)

```cpp
// Type-safe, compile-time, zero-overhead
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // C++17 fold expression!
}

sum(1, 2, 3);          // 6 (int)
sum(1.0, 2.0, 3.0);   // 6.0 (double)
sum(1, 2.0, 3);        // 6.0 (mixed — implicit conversions)

// Printing any number of arguments:
template<typename T, typename... Rest>
void print_all(T first, Rest... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
        print_all(rest...);  // Recursive!
    } else {
        std::cout << "\n";
    }
}

print_all(1, "hello", 3.14, true);  // "1, hello, 3.14, 1"
```

---

## 4.9 Lambda Functions (C++11)

Lambdas are **anonymous functions** defined inline. They're one of the most important modern C++ features.

```cpp
// ANATOMY:
// [capture] (parameters) -> return_type { body }

// Simplest lambda (no capture, no params):
auto say_hello = []() {
    std::cout << "Hello!\n";
};
say_hello();  // Call it

// With parameters:
auto add = [](int a, int b) -> int {
    return a + b;
};
std::cout << add(3, 4);  // 7

// Return type deduced (almost always can omit ->):
auto multiply = [](int a, int b) { return a * b; };
```

### Lambda Captures — The Critical Concept

```cpp
int x = 10;
int y = 20;

// Capture by VALUE (copy):
auto f1 = [x, y]() {
    // x and y are COPIES made when lambda is defined
    std::cout << x + y;  // 30
};
x = 100;  // Doesn't affect the copy inside f1
f1();     // Still prints 30

// Capture by REFERENCE:
auto f2 = [&x, &y]() {
    std::cout << x + y;  // Uses ACTUAL x and y
};
x = 100;  // Now f2 will use x=100
f2();     // Prints 120

// Capture ALL by value:
auto f3 = [=]() { return x + y; };

// Capture ALL by reference:
auto f4 = [&]() { return x + y; };

// Mixed:
auto f5 = [=, &x]() { /* x by ref, everything else by value */ };
auto f6 = [&, x]() { /* x by value, everything else by ref */ };

// Capture this (for class methods):
class Counter {
    int count = 0;
public:
    auto make_incrementer() {
        return [this]() { ++count; };  // Captures the Counter object
        // C++17: [*this] to capture by value
    }
};
```

### Mutable Lambdas

```cpp
int n = 0;
// By default, captured-by-value vars are const in the lambda body
// auto f = [n]() { n++; };  // ERROR: n is const copy

// 'mutable' makes the copies non-const:
auto f = [n]() mutable {
    n++;  // Modifies the COPY, not the original
    std::cout << n;
};
f();  // 1
f();  // 2
std::cout << n;  // Still 0! Original unchanged
```

### Lambdas with Standard Algorithms

```cpp
#include <algorithm>
#include <vector>

std::vector<int> v = {5, 2, 8, 1, 9, 3};

// Sort descending:
std::sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;  // Custom comparator
});
// v = {9, 8, 5, 3, 2, 1}

// Find first element > 4:
auto it = std::find_if(v.begin(), v.end(), [](int x) {
    return x > 4;
});

// Transform (double every element):
std::transform(v.begin(), v.end(), v.begin(), [](int x) {
    return x * 2;
});

// Count elements satisfying a condition:
int count = std::count_if(v.begin(), v.end(), [](int x) {
    return x % 2 == 0;  // Count evens
});
```

### Generic Lambdas (C++14)

```cpp
// 'auto' parameters make the lambda a template!
auto identity = [](auto x) { return x; };

identity(42);       // Works with int
identity(3.14);     // Works with double
identity("hello");  // Works with const char*

// C++20: Explicit template parameters in lambdas
auto typed = []<typename T>(T x) -> T {
    return x * 2;
};
```

---

## 4.10 `std::function` — Storing Callables

```cpp
#include <functional>

// std::function can store ANY callable:
// - Regular functions
// - Lambdas
// - Functors (objects with operator())
// - Member functions (with std::bind)

std::function<int(int, int)> operation;

operation = [](int a, int b) { return a + b; };
std::cout << operation(3, 4);  // 7

operation = [](int a, int b) { return a * b; };
std::cout << operation(3, 4);  // 12

// Passing callbacks:
void apply(const std::vector<int>& v, std::function<void(int)> callback) {
    for (int x : v) callback(x);
}

apply({1, 2, 3}, [](int x) { std::cout << x * x << " "; });
// Output: 1 4 9

// PERFORMANCE NOTE: std::function has overhead (type erasure, heap allocation)
// For performance-critical code, prefer templates or auto parameters
```

---

## 4.11 Function Pointers

```cpp
// Function pointer syntax (confusing, use 'using' alias!)
int (*fp)(int, int);  // fp is a pointer to function taking 2 ints, returning int

// Better: type alias
using BinaryOp = int(*)(int, int);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

BinaryOp op = add;
std::cout << op(3, 4);  // 7

op = mul;
std::cout << op(3, 4);  // 12

// Array of function pointers:
BinaryOp ops[] = {add, mul};
for (auto f : ops) {
    std::cout << f(3, 4) << " ";  // 7 12
}
```

---

## 4.12 `noexcept` — Exception Specification

```cpp
// noexcept: declares that a function will NEVER throw an exception
// Allows compiler optimizations (no exception table needed)

void safe_function() noexcept {
    // I guarantee: no exception will escape this function
    int x = 5 + 3;
}

// noexcept with condition:
template<typename T>
void swap(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                std::is_nothrow_move_assignable_v<T>) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

// If a noexcept function DOES throw:
// std::terminate() is called immediately (not exception propagation!)

// Rules:
// - Destructors are noexcept by default (never throw from destructors!)
// - Move operations should be noexcept when possible
// - Mark leaf functions noexcept when you know they won't throw
```

---

## 4.13 Recursion

```cpp
// A function that calls itself

// Factorial:
long long factorial(int n) {
    if (n <= 1) return 1;       // BASE CASE (prevents infinite recursion)
    return n * factorial(n - 1); // RECURSIVE CASE
}

// Fibonacci:
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
// WARNING: Exponential time complexity! Use memoization or iteration.

// Tail recursion (can be optimized by compiler):
long long factorial_tail(int n, long long acc = 1) {
    if (n <= 1) return acc;
    return factorial_tail(n - 1, n * acc);  // Tail call: last operation is the call
}
// With -O2, this compiles to a loop (no stack growth)

// STACK OVERFLOW: Recursion too deep fills the stack!
// Stack is usually 1-8 MB; each frame uses some space
// factorial(1000000) would overflow!
```

---

## 4.14 `constexpr` Functions

```cpp
// constexpr functions can be evaluated at compile time:
constexpr int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

constexpr int p = power(2, 10);  // Computed at compile time: 1024
int arr[power(2, 8)];            // Array size computed at compile time: 256

// Recursive constexpr (C++14+):
constexpr int fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);
}

constexpr int f10 = fib(10);  // 55, computed at compile time

// constexpr functions can also run at runtime:
int n = get_runtime_value();
int result = power(2, n);  // Runtime computation
```

---

## 📝 Module 04 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| Pass by value | Copies; use for small/cheap types |
| Pass by const ref | No copy, read-only; use for large objects |
| Pass by ref | No copy, mutable; use when you need to modify |
| Return values | Prefer value return; use optional for "might fail" |
| Overloading | Same name, different params; compiler picks by type |
| Lambdas | Anonymous functions; capture by `[=]` value or `[&]` ref |
| std::function | Type-erased callable; convenient but has overhead |
| noexcept | Mark non-throwing functions; enables optimizations |
| constexpr | Evaluate at compile time when possible |
| Recursion | Always need base case; watch for stack overflow |

---

## ➡️ Next: [Module 05: Object-Oriented Programming](../module_05_oop/05_oop.md)
