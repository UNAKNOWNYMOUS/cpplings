# Module 12: Performance, Best Practices & Modern C++ Patterns

> *"C++ lets you be as fast as C while being as expressive as modern high-level languages — when you know what you're doing."*

---

## 12.1 Understanding Performance

```
┌──────────────────────────────────────────────────────────────────────┐
│              THE HARDWARE REALITY (2024 approximate)                  │
│                                                                        │
│  Operation            Approximate Cost                                │
│  ─────────────────────────────────────                               │
│  Register access      ~0.3 ns                                        │
│  L1 cache hit         ~1 ns                                          │
│  L2 cache hit         ~4 ns                                          │
│  L3 cache hit         ~10 ns                                         │
│  RAM access           ~60–100 ns  ← 100x slower than L1!            │
│  SSD read             ~100 µs     ← 100,000x slower than RAM        │
│  Network (same dc)    ~500 µs     ← 5x slower than SSD              │
│                                                                        │
│  KEY INSIGHT: Cache-friendly code is fast code.                      │
│  Memory access patterns matter MORE than algorithm complexity        │
│  for typical dataset sizes.                                          │
│                                                                        │
│  vector<T> beats list<T> for most workloads because:                │
│  - Contiguous memory → cache-friendly                                │
│  - Predictable access pattern → hardware prefetcher works           │
│  Even though list has O(1) insert vs vector's O(n)!                 │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 12.2 Data-Oriented Design

```cpp
// OBJECT-ORIENTED (cache-unfriendly for batch processing):
struct Entity {
    float position[3];  // 12 bytes
    float velocity[3];  // 12 bytes
    float health;       // 4 bytes
    float damage;       // 4 bytes
    int id;             // 4 bytes
    bool alive;         // 1 byte
    // Padding...       // 3 bytes
    // Total: 40 bytes per entity
};

std::vector<Entity> entities(100000);

// To update positions, we load ALL 40 bytes of each entity
// but only USE 24 bytes (position + velocity)!
// Memory bandwidth wasted, cache full of unused data
void update_positions(float dt) {
    for (auto& e : entities) {
        if (e.alive) {
            e.position[0] += e.velocity[0] * dt;  // Only need pos+vel
            e.position[1] += e.velocity[1] * dt;  // But loading whole Entity
            e.position[2] += e.velocity[2] * dt;
        }
    }
}

// DATA-ORIENTED (Structure of Arrays — cache-friendly):
struct EntitySystem {
    std::vector<float> pos_x, pos_y, pos_z;     // All X positions together
    std::vector<float> vel_x, vel_y, vel_z;     // All velocities together
    std::vector<float> health;
    std::vector<float> damage;
    std::vector<bool> alive;

    void update_positions(float dt) {
        // Now we only load EXACTLY the data we need
        // pos_x and vel_x are contiguous → cache line holds many values
        for (size_t i = 0; i < pos_x.size(); ++i) {
            if (alive[i]) {
                pos_x[i] += vel_x[i] * dt;  // Sequential memory access!
                pos_y[i] += vel_y[i] * dt;
                pos_z[i] += vel_z[i] * dt;
            }
        }
    }
};
// Result: Often 2-10x faster due to cache efficiency!
```

---

## 12.3 Compiler Optimizations

```cpp
// HELP the compiler optimize by:

// 1. Mark functions inline (or put them in headers for LTO)
inline int square(int x) { return x * x; }

// 2. Use const wherever possible — enables more optimizations
const int MAX = 100;
void process(const std::vector<int>& data) { /* compiler knows data won't change */ }

// 3. Avoid aliasing — tell compiler pointers don't overlap:
void add_arrays(float* __restrict__ a, const float* __restrict__ b, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] += b[i];  // Compiler can vectorize (SIMD) with __restrict__
    }
}

// 4. Use [[likely]] / [[unlikely]] (C++20):
if [[likely]] (value > 0) {
    fast_path();
} else [[unlikely]] {
    slow_error_path();
}

// 5. Branch-free arithmetic (avoid branch misprediction):
// Slow (branchy):
int abs_slow(int x) { return x < 0 ? -x : x; }

// Fast (branch-free, one instruction on modern CPUs):
int abs_fast(int x) {
    int mask = x >> 31;   // -1 if negative, 0 if non-negative
    return (x + mask) ^ mask;
}

// 6. Reserve vector capacity:
std::vector<int> v;
v.reserve(1000);  // One allocation vs. many reallocations
for (int i = 0; i < 1000; ++i) v.push_back(i);
```

---

## 12.4 Profiling — Measure Before Optimizing

```bash
# RULE #1: Never optimize without profiling first!
# "Premature optimization is the root of all evil." — Knuth

# GCC/Clang: compile with profiling
g++ -O2 -pg program.cpp -o program   # Enable profiling
./program                             # Run to generate gmon.out
gprof program gmon.out > profile.txt  # View profile
cat profile.txt                       # See where time is spent

# perf (Linux): low-overhead profiling
perf stat ./program          # Basic stats (cache misses, branches, etc.)
perf record ./program        # Record detailed profile
perf report                  # Interactive viewer

# Valgrind callgrind: instruction-level profiling
valgrind --tool=callgrind ./program
callgrind_annotate callgrind.out.*

# Address Sanitizer for correctness:
g++ -fsanitize=address -g program.cpp
```

---

## 12.5 Smart Use of `constexpr`

```cpp
// constexpr: compute at COMPILE TIME → zero runtime cost!

constexpr double PI = 3.14159265358979;

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

static_assert(factorial(10) == 3628800);  // Verified at compile time!

// constexpr class:
class Rational {
    int num, den;
public:
    constexpr Rational(int n, int d) : num{n}, den{d} {}
    constexpr double value() const { return static_cast<double>(num) / den; }
    constexpr Rational operator+(const Rational& other) const {
        return {num * other.den + other.num * den, den * other.den};
    }
};

constexpr Rational half{1, 2};
constexpr Rational third{1, 3};
constexpr Rational five_sixths = half + third;  // Computed at compile time!

// consteval (C++20): MUST be evaluated at compile time:
consteval int must_be_compile_time(int n) {
    return n * n;
}

must_be_compile_time(5);  // OK: 25 at compile time
// int x = 5; must_be_compile_time(x);  // ERROR: x not constexpr!

// constinit (C++20): compile-time initialization, runtime mutation:
constinit int global = factorial(5);  // Initialized at compile time
// global can be modified at runtime (unlike constexpr global)
```

---

## 12.6 Key Design Patterns in Modern C++

### CRTP — Curiously Recurring Template Pattern

```cpp
// Enable static (compile-time) polymorphism — zero virtual overhead!
template<typename Derived>
class Printable {
public:
    void print() const {
        static_cast<const Derived*>(this)->print_impl();
    }
};

class Dog : public Printable<Dog> {
public:
    void print_impl() const { std::cout << "Dog\n"; }
};

class Cat : public Printable<Cat> {
public:
    void print_impl() const { std::cout << "Cat\n"; }
};

// Static dispatch — no vtable, no virtual overhead!
template<typename T>
void print_animal(const Printable<T>& animal) {
    animal.print();  // Resolved at compile time
}

print_animal(Dog{});  // Calls Dog::print_impl directly
print_animal(Cat{});  // Calls Cat::print_impl directly
```

### Policy-Based Design

```cpp
// Compose behavior at compile time through policy classes
template<typename SortPolicy, typename ContainerPolicy>
class Database {
    // Uses policies to customize behavior
public:
    void sort_data(auto& data) {
        SortPolicy::sort(data);
    }
    auto create_container() {
        return ContainerPolicy::create();
    }
};

struct QuickSortPolicy {
    static void sort(auto& data) { std::sort(data.begin(), data.end()); }
};

struct StableSortPolicy {
    static void sort(auto& data) { std::stable_sort(data.begin(), data.end()); }
};

struct VectorPolicy {
    static auto create() { return std::vector<int>{}; }
};

Database<QuickSortPolicy, VectorPolicy> fast_db;
Database<StableSortPolicy, VectorPolicy> stable_db;
```

### Type Erasure

```cpp
// std::function: stores any callable (with type erasure)
std::function<int(int, int)> op;
op = [](int a, int b) { return a + b; };
op = std::plus<int>{};  // Function object
op(3, 4);  // 7

// Cost: heap allocation + virtual dispatch
// Alternative: use templates for performance-critical paths

// Custom type erasure (zero overhead when used correctly):
class Drawable {
    struct Concept {
        virtual void draw() const = 0;
        virtual ~Concept() = default;
    };
    template<typename T>
    struct Model : Concept {
        T value;
        Model(T v) : value{std::move(v)} {}
        void draw() const override { value.draw(); }
    };
    std::unique_ptr<Concept> self;
public:
    template<typename T>
    Drawable(T t) : self{std::make_unique<Model<T>>(std::move(t))} {}
    void draw() const { self->draw(); }
};
```

---

## 12.7 The Spaceship Operator `<=>` (C++20)

```cpp
#include <compare>

struct Point {
    int x, y;

    // Generate ALL comparison operators automatically!
    auto operator<=>(const Point&) const = default;
    // This generates: ==, !=, <, >, <=, >= all at once!
};

Point a{1, 2}, b{1, 3};
a == b;  // false
a < b;   // true (compares x first, then y)
a != b;  // true

// Custom comparison:
struct Version {
    int major, minor, patch;

    auto operator<=>(const Version& other) const {
        if (auto cmp = major <=> other.major; cmp != 0) return cmp;
        if (auto cmp = minor <=> other.minor; cmp != 0) return cmp;
        return patch <=> other.patch;
    }
    bool operator==(const Version&) const = default;
};

Version v1{1, 2, 3}, v2{1, 3, 0};
v1 < v2;   // true
v1 == v2;  // false
```

---

## 12.8 Coroutines (C++20)

```cpp
#include <coroutine>
#include <generator>  // C++23, or use custom implementation

// Coroutines: functions that can be suspended and resumed
// Enable cooperative multitasking, generators, async I/O

// Generator: lazily produce values
std::generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;  // Suspend here, return value to caller
        auto c = a + b;
        a = b;
        b = c;
    }
}

// Usage:
for (int fib : fibonacci() | std::views::take(10)) {
    std::cout << fib << " ";  // 0 1 1 2 3 5 8 13 21 34
}

// Async with coroutines:
Task<int> async_operation() {
    auto result = co_await some_async_function();  // Suspend, resume when ready
    co_return result * 2;
}
```

---

## 12.9 The `<filesystem>` Library

```cpp
#include <filesystem>
namespace fs = std::filesystem;

// Path manipulation:
fs::path p = "/home/user/documents";
p / "file.txt";           // Append: /home/user/documents/file.txt
p.filename();             // "documents"
p.parent_path();          // "/home/user"
p.extension();            // "" (no extension here)
p.stem();                 // "documents" (without extension)

// File operations:
fs::exists("/tmp/file.txt");          // Check if exists
fs::is_regular_file("/tmp/file.txt"); // Is it a file?
fs::is_directory("/tmp");             // Is it a directory?
fs::file_size("/tmp/file.txt");       // Size in bytes
fs::last_write_time("/tmp/file.txt"); // Last modification time

fs::copy("/src/file.txt", "/dst/file.txt");     // Copy
fs::rename("/old/path", "/new/path");            // Move/rename
fs::remove("/tmp/file.txt");                     // Delete file
fs::remove_all("/tmp/dir");                      // Delete directory recursively
fs::create_directories("/a/b/c/d");              // Create nested dirs

// Iterate over directory:
for (const auto& entry : fs::directory_iterator("/tmp")) {
    std::cout << entry.path() << "\n";
    if (entry.is_regular_file()) {
        std::cout << "  Size: " << entry.file_size() << "\n";
    }
}

// Recursive iteration:
for (const auto& entry : fs::recursive_directory_iterator("/home")) {
    if (entry.path().extension() == ".cpp") {
        std::cout << "Found C++ file: " << entry.path() << "\n";
    }
}
```

---

## 12.10 Best Practices Summary

```
╔══════════════════════════════════════════════════════════════════════╗
║               MODERN C++ BEST PRACTICES CHEAT SHEET                 ║
╠══════════════════════════════════════════════════════════════════════╣
║                                                                       ║
║  MEMORY & RESOURCES                                                  ║
║  ✓ Use RAII for all resources (memory, files, mutexes, etc.)        ║
║  ✓ Prefer unique_ptr > shared_ptr > raw pointer                     ║
║  ✓ Never use raw new/delete — use make_unique/make_shared           ║
║  ✓ Follow Rule of Zero (let compiler generate special members)      ║
║                                                                       ║
║  TYPES & VARIABLES                                                   ║
║  ✓ Use auto for type deduction (reduces boilerplate, avoids errors) ║
║  ✓ Prefer const by default; use constexpr when possible            ║
║  ✓ Use strong types (enum class, named types) over primitives       ║
║  ✓ Use std::optional instead of sentinel values or nullptr          ║
║                                                                       ║
║  FUNCTIONS & INTERFACES                                              ║
║  ✓ Pass by const& for large objects you don't own                  ║
║  ✓ Pass by value when you need a copy (move-optimized)             ║
║  ✓ Return by value (NRVO applies); avoid out parameters            ║
║  ✓ Mark move constructors/ops noexcept                              ║
║  ✓ Use override on all virtual function overrides                   ║
║                                                                       ║
║  CONTAINERS & ALGORITHMS                                             ║
║  ✓ Default to std::vector; justify other containers                ║
║  ✓ Use standard algorithms over hand-written loops                  ║
║  ✓ Use emplace_back over push_back when constructing in-place       ║
║  ✓ Reserve vector when size is known in advance                     ║
║                                                                       ║
║  TEMPLATES & GENERIC CODE                                            ║
║  ✓ Use concepts (C++20) to constrain templates                      ║
║  ✓ Use constexpr if for compile-time branching                      ║
║  ✓ Prefer function templates to overloads when behavior is same     ║
║                                                                       ║
║  ERROR HANDLING                                                      ║
║  ✓ Use exceptions for exceptional errors                             ║
║  ✓ Use std::expected/optional for expected failures                 ║
║  ✓ Never let exceptions escape destructors                          ║
║                                                                       ║
║  CONCURRENCY                                                         ║
║  ✓ Always protect shared data with mutex or atomic                  ║
║  ✓ Prefer std::async or thread pools over raw threads               ║
║  ✓ Use RAII locks (lock_guard, unique_lock, scoped_lock)            ║
║                                                                       ║
║  STYLE                                                               ║
║  ✓ Compile with -Wall -Wextra -Wpedantic (zero warnings policy)     ║
║  ✓ Use sanitizers in debug builds                                    ║
║  ✓ Write tests alongside code                                        ║
║  ✓ Profile before optimizing — measure, don't guess                 ║
╚══════════════════════════════════════════════════════════════════════╝
```

---

## 12.11 C++ Version Feature Timeline

```
┌──────────────────────────────────────────────────────────────────────┐
│            C++ FEATURE TIMELINE                                        │
│                                                                        │
│  C++11: The Big Leap                                                  │
│  auto, range-for, lambdas, move semantics, rvalue refs               │
│  smart pointers, nullptr, constexpr, thread library                  │
│  variadic templates, initializer lists, std::array                   │
│                                                                        │
│  C++14: Refinements                                                   │
│  Generic lambdas, auto return type, constexpr functions              │
│  make_unique, shared_timed_mutex, std::exchange                      │
│                                                                        │
│  C++17: Substantial Additions                                         │
│  Structured bindings, if constexpr, fold expressions                 │
│  std::optional, std::variant, std::any                               │
│  std::filesystem, std::string_view, parallel algorithms              │
│  Class template argument deduction (CTAD)                            │
│                                                                        │
│  C++20: Revolutionary                                                 │
│  Concepts, Ranges, Coroutines, Modules                               │
│  std::format, std::span, consteval, constinit                        │
│  Three-way comparison (spaceship), jthread, std::stop_token          │
│                                                                        │
│  C++23: More Improvements                                             │
│  std::expected, std::generator, std::print                           │
│  std::mdspan, more ranges, flat_map, flat_set                        │
│  import std; (standard library modules)                              │
│                                                                        │
│  C++26 (upcoming): std::execution, reflection, ...                   │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 📝 Module 12 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| Cache efficiency | Data layout > algorithm complexity for typical sizes |
| Data-oriented design | SoA over AoS for batch processing |
| `constexpr` | Move computation to compile time; free at runtime |
| CRTP | Static polymorphism; no vtable overhead |
| Spaceship `<=>` | Generate all comparisons at once (C++20) |
| Coroutines | Cooperative multitasking, generators, async I/O |
| `<filesystem>` | Platform-independent file/directory operations |
| Profile first | Never optimize blind; measure with perf/valgrind |
| Sanitizers | Run with ASan+UBSan in debug; catch bugs early |
| Modern best practices | const everywhere; RAII always; algorithms over loops |

---

## 🏆 Congratulations — You've Completed the Course!

You now understand C++ from the ground up:

1. **Foundations** — compilation, linking, memory model
2. **Core Types** — integers, floats, pointers, references
3. **Control Flow** — loops, conditions, expressions
4. **Functions** — overloading, default args, lambdas
5. **OOP** — classes, inheritance, polymorphism, RAII
6. **Memory** — RAII, smart pointers, unique/shared/weak
7. **Templates** — generic programming, concepts, TMP
8. **STL** — containers, algorithms, ranges
9. **Move Semantics** — lvalues, rvalues, perfect forwarding
10. **Concurrency** — threads, mutexes, atomics, async
11. **Error Handling** — exceptions, expected, noexcept
12. **Performance** — cache, data-oriented, constexpr, patterns

---

## ➡️ Continue to: [Exercises & Projects](../exercises/exercises.md)
