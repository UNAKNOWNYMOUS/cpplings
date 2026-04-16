# Module 09: Move Semantics & Perfect Forwarding

> *"Move semantics transformed C++ from a language with performance footguns into one where efficiency is the default."*

---

## 9.1 The Problem Move Semantics Solves

```cpp
// BEFORE C++11: EXPENSIVE copies everywhere

std::vector<int> create_big_vector() {
    std::vector<int> result;
    result.reserve(1000000);
    for (int i = 0; i < 1000000; ++i) result.push_back(i);
    return result;  // COPIES 1 million ints?!
}

std::vector<int> v = create_big_vector();  // Deep copy — 4MB copied!

// AFTER C++11: Move semantics — STEAL the buffer instead of copying
// create_big_vector() returns an rvalue (temporary)
// Vector's MOVE constructor runs: steal the internal buffer
// Result: O(1) "copy" instead of O(n) copy!
```

---

## 9.2 lvalues and rvalues

Understanding value categories is foundational to move semantics.

```
┌──────────────────────────────────────────────────────────────────────┐
│                    VALUE CATEGORIES                                    │
│                                                                        │
│  LVALUE: Has a name; has an address; can appear on left of =         │
│  Examples: variables, references, function return of lvalue ref      │
│                                                                        │
│    int x = 5;      // x is an lvalue                                 │
│    std::string s;  // s is an lvalue                                 │
│    int arr[5];     // arr[2] is an lvalue                            │
│                                                                        │
│  RVALUE: Temporary; no name; cannot appear on left of =              │
│  Examples: literals, temporaries, function return of non-ref         │
│                                                                        │
│    5             // rvalue (literal)                                  │
│    x + y         // rvalue (temporary result)                        │
│    std::string("hi")  // rvalue (temporary object)                   │
│    create_big_vector() // rvalue (returned temporary)                │
│                                                                        │
│  KEY INSIGHT:                                                          │
│  lvalues: "I have a name, I'll be used again"                        │
│  rvalues: "I'm temporary, you can STEAL my guts"                     │
└──────────────────────────────────────────────────────────────────────┘
```

```cpp
int x = 5;
int& lref = x;       // lvalue reference — binds to lvalue
// int& bad = 5;     // ERROR: can't bind lvalue ref to rvalue!
const int& cref = 5; // OK: const lvalue ref CAN bind to rvalue

int&& rref = 5;      // rvalue reference — binds to rvalue
int&& rref2 = x + 1; // OK: x+1 is a temporary (rvalue)
// int&& bad = x;    // ERROR: can't bind rvalue ref to lvalue!

// std::move: CAST lvalue to rvalue (doesn't actually move!)
int&& rref3 = std::move(x);  // Now we can steal from x
// After move: x is in a valid but unspecified state!
```

---

## 9.3 Move Constructor and Move Assignment

```cpp
class Buffer {
    int* data;
    size_t size;

public:
    // Constructor
    Buffer(size_t n) : data{new int[n]}, size{n} {
        std::cout << "Constructed\n";
    }

    // Destructor
    ~Buffer() {
        delete[] data;
        std::cout << "Destroyed\n";
    }

    // Copy constructor — EXPENSIVE: allocates new memory, copies all data
    Buffer(const Buffer& other) : size{other.size} {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        std::cout << "Copied\n";
    }

    // Copy assignment
    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        std::cout << "Copy assigned\n";
        return *this;
    }

    // MOVE constructor — CHEAP: steal the pointer!
    Buffer(Buffer&& other) noexcept        // noexcept is important for optimization!
        : data{other.data}, size{other.size} {
        other.data = nullptr;  // "Null out" the source — leave in valid state
        other.size = 0;
        std::cout << "Moved\n";
    }

    // MOVE assignment — CHEAP: swap resources
    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;          // Free current resource
        data = other.data;      // Steal other's resource
        size = other.size;
        other.data = nullptr;   // Null out source
        other.size = 0;
        std::cout << "Move assigned\n";
        return *this;
    }
};

Buffer b1(1000000);            // Constructed
Buffer b2 = b1;                // Copied (expensive)
Buffer b3 = std::move(b1);     // Moved (cheap!) — b1 is now empty
Buffer b4 = Buffer(500000);    // Moved (compiler elides this or moves)
```

---

## 9.4 When Moves Happen Automatically

```cpp
// C++ automatically uses move semantics when:

// 1. Returning a local variable (NRVO / move):
std::vector<int> make_vector() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    return v;  // MOVE (or NRVO — Named Return Value Optimization)
}

// 2. Passing a temporary:
void process(std::string s) { /* ... */ }
process(std::string("hello"));  // MOVE into s

// 3. std::move (explicit):
std::string s = "hello";
std::string s2 = std::move(s);  // Explicit move
// s is now valid but empty ("")

// 4. Inserting into containers:
std::vector<std::string> v;
std::string s3 = "world";
v.push_back(std::move(s3));  // MOVE into vector — s3 now empty
v.emplace_back("direct");    // Construct directly — no move needed!
```

---

## 9.5 `noexcept` and Move Semantics

```cpp
// WHY noexcept matters for moves:

// std::vector reallocation:
// - If move constructor is noexcept: uses move (fast)
// - If move constructor might throw: uses copy (safe but slow)

class Safe {
public:
    Safe(Safe&& other) noexcept { /* move */ }  // noexcept → vector uses moves
};

class Unsafe {
public:
    Unsafe(Unsafe&& other) { /* might throw! */ }  // No noexcept → vector copies!
};

std::vector<Safe> safe_vec;
safe_vec.push_back(Safe{});   // move used during reallocation — FAST

std::vector<Unsafe> unsafe_vec;
unsafe_vec.push_back(Unsafe{});  // COPIES during reallocation — slow!

// Rule: ALWAYS mark move constructors/assignments as noexcept!
// Exception: when they genuinely might throw (rare)
```

---

## 9.6 Perfect Forwarding

```cpp
// Problem: Preserving value category when forwarding arguments

template<typename T>
void process(T& value) {
    std::cout << "lvalue\n";
}
template<typename T>
void process(T&& value) {
    std::cout << "rvalue\n";
}

// NAIVE wrapper — LOSES value category information!
template<typename T>
void naive_wrapper(T value) {
    process(value);  // ALWAYS calls lvalue version! 'value' is always named!
}

// CORRECT: perfect forwarding with universal references + std::forward
template<typename T>
void perfect_wrapper(T&& arg) {  // 'T&&' with deduced T is a UNIVERSAL REFERENCE
    process(std::forward<T>(arg));  // Forwards as lvalue if arg was lvalue,
                                    // as rvalue if arg was rvalue
}

int x = 5;
perfect_wrapper(x);    // arg is lvalue → forwards as lvalue
perfect_wrapper(10);   // arg is rvalue → forwards as rvalue
```

### Universal References vs Rvalue References

```
┌──────────────────────────────────────────────────────────────────────┐
│            UNIVERSAL REFERENCE vs RVALUE REFERENCE                   │
│                                                                        │
│  RVALUE REFERENCE (T is NOT deduced):                                │
│  void foo(Widget&& x);     // Only binds to Widget rvalues           │
│  Widget&& ref = ...;       // Only binds to Widget rvalues           │
│                                                                        │
│  UNIVERSAL REFERENCE (T IS deduced, written as T&&):                 │
│  template<typename T>                                                 │
│  void foo(T&& x);          // Binds to BOTH lvalues AND rvalues!     │
│                                                                        │
│  HOW IT WORKS (reference collapsing):                                │
│  If T is int&   → T&& = int& && = int&   (lvalue ref)              │
│  If T is int    → T&& = int&&            (rvalue ref)               │
│                                                                        │
│  So 'T&&' where T is deduced can be EITHER kind!                    │
│  std::forward<T>(arg) restores the original value category.         │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 9.7 `std::forward` vs `std::move`

```cpp
// std::move: ALWAYS cast to rvalue reference (unconditionally)
// Use when: You KNOW you want to move, and you're done with the object

std::string s = "hello";
std::string s2 = std::move(s);  // s is now empty; s2 owns the string

// std::forward<T>: conditionally cast to rvalue
// Use when: Forwarding an argument in a template function
// Preserves the original value category

template<typename T>
void wrapper(T&& arg) {
    inner_function(std::forward<T>(arg));
    // If T=int& (lvalue was passed): forward as lvalue
    // If T=int  (rvalue was passed): forward as rvalue
}

// COMMON MISTAKE: using move in a forwarding context:
template<typename T>
void bad_wrapper(T&& arg) {
    inner_function(std::move(arg));  // ALWAYS moves, even lvalues! BAD!
}

int x = 5;
bad_wrapper(x);  // MOVES x! x is now in moved-from state! Bug!

// COMMON MISTAKE: using forward outside a template:
std::string s3 = "hello";
std::string s4 = std::forward<std::string>(s3);  // Works but use move instead!
```

---

## 9.8 The Emplace Family

```cpp
// emplace: construct directly in container — avoids move/copy!

std::vector<std::pair<int, std::string>> v;

// OLD: Create pair, then copy/move into vector (1-2 operations)
v.push_back(std::make_pair(1, "hello"));  // Create pair + move

// NEW: Construct directly in vector memory (1 operation — no copy/move!)
v.emplace_back(1, "hello");  // Arguments forwarded to pair constructor

// For all containers:
std::map<int, std::string> m;
m.emplace(1, "one");      // Directly constructs pair in map
m.try_emplace(2, "two");  // Only insert if key doesn't exist (C++17)

// emplace vs push_back:
struct Point {
    int x, y;
    Point(int x, int y) : x{x}, y{y} {}
};

std::vector<Point> points;
points.push_back(Point{1, 2});  // Create Point + move into vector
points.emplace_back(1, 2);      // Construct Point DIRECTLY in vector — faster!
```

---

## 9.9 Return Value Optimization (RVO and NRVO)

```cpp
// Compilers often ELIDE (skip) copies/moves entirely!

// RVO (Return Value Optimization): Unnamed temporary
std::string make_string_rvo() {
    return std::string("hello");  // RVO: constructed directly in caller's space!
}

// NRVO (Named RVO): Named local variable
std::string make_string_nrvo() {
    std::string result = "hello";
    result += " world";
    return result;  // NRVO: often constructed directly in caller's space!
}

// Don't return std::move() — it PREVENTS NRVO!
std::string bad_return() {
    std::string s = "hello";
    return std::move(s);  // WRONG! Prevents NRVO! Just return s directly.
}

// Rule: Return local variables directly, without std::move
// The compiler will apply NRVO when possible, and move when not
```

---

## 9.10 Moved-From State

```cpp
// After moving from an object, it is in a "valid but unspecified state"
// You can only:
// 1. Destroy it
// 2. Assign to it (reset it)
// You CANNOT use its previous value!

std::string s = "hello";
std::string s2 = std::move(s);

// s is now valid but unspecified:
std::cout << s;  // Might print "", might print anything. Don't rely on it!
s = "new value";  // OK: assign new value
std::cout << s;   // "new value" — safe now

// Smart pointers after move:
auto p = std::make_unique<int>(42);
auto p2 = std::move(p);
// p is now nullptr (unique_ptr guarantees this)
if (!p) std::cout << "p is null\n";  // Safe to check
```

---

## 📝 Module 09 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| lvalue vs rvalue | lvalues have names/addresses; rvalues are temporaries |
| `T&&` | Rvalue reference; can bind to temporaries |
| Move constructor/assignment | Steal resources; mark `noexcept` |
| `std::move` | Cast to rvalue; doesn't actually move |
| `std::forward<T>` | Preserves value category; use in template forwarding |
| Universal reference | `T&&` where T is deduced; binds to both |
| `noexcept` | Critical for vector reallocation to use moves |
| `emplace_back` | Construct directly in container; prefer over push_back |
| RVO/NRVO | Compiler elides copies; don't return `std::move(local)` |
| Moved-from state | Valid but unspecified; assign before reuse |

---

## ➡️ Next: [Module 10: Concurrency & Multithreading](../module_10_concurrency/10_concurrency.md)
