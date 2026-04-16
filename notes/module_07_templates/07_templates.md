# Module 07: Templates & Generic Programming

> *"Templates are C++'s way of writing code that works for any type — without sacrificing performance."*

---

## 7.1 Why Templates?

Without templates, you'd write the same logic repeatedly for each type:

```cpp
// Without templates — combinatorial explosion:
int    max_int(int a, int b)       { return a > b ? a : b; }
double max_double(double a, double b) { return a > b ? a : b; }
float  max_float(float a, float b)  { return a > b ? a : b; }

// With templates — write once, works for ANY comparable type:
template<typename T>
T max_val(T a, T b) { return a > b ? a : b; }

int    r1 = max_val(3, 5);         // T deduced as int
double r2 = max_val(3.14, 2.72);   // T deduced as double
std::string r3 = max_val(std::string("apple"), std::string("banana")); // T = string
```

---

## 7.2 Function Templates

```cpp
// Basic function template:
template<typename T>
void swap_vals(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Multiple template parameters:
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {   // Return type depends on T and U
    return a + b;
}

// Or with C++14 auto return type deduction:
template<typename T, typename U>
auto add2(T a, U b) {
    return a + b;  // Return type deduced automatically
}

int r1 = add(3, 4);           // T=int, U=int → int
double r2 = add(3, 4.5);      // T=int, U=double → double
auto r3 = add(3, 4.5f);       // T=int, U=float → float

// Non-type template parameters:
template<int N>
void print_n_times(const std::string& msg) {
    for (int i = 0; i < N; ++i) {
        std::cout << msg << "\n";
    }
}

print_n_times<3>("Hello");  // N is a compile-time constant!

// Template with default:
template<typename T = int, int N = 10>
std::array<T, N> make_array() {
    return {};
}

auto a1 = make_array();           // array<int, 10>
auto a2 = make_array<double, 5>(); // array<double, 5>
```

---

## 7.3 Class Templates

```cpp
// A generic Stack:
template<typename T>
class Stack {
    std::vector<T> data;
public:
    void push(const T& item) { data.push_back(item); }
    void push(T&& item) { data.push_back(std::move(item)); }

    T pop() {
        if (data.empty()) throw std::underflow_error("Stack empty");
        T top = std::move(data.back());
        data.pop_back();
        return top;
    }

    const T& top() const {
        if (data.empty()) throw std::underflow_error("Stack empty");
        return data.back();
    }

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
};

// Usage:
Stack<int> int_stack;
int_stack.push(1);
int_stack.push(2);
int_stack.push(3);
std::cout << int_stack.pop() << "\n";  // 3

Stack<std::string> str_stack;
str_stack.push("hello");
str_stack.push("world");

// Class template with multiple parameters:
template<typename Key, typename Value, size_t MaxSize = 100>
class FixedMap {
    std::array<std::pair<Key, Value>, MaxSize> data;
    size_t count = 0;
public:
    void insert(Key k, Value v) {
        if (count >= MaxSize) throw std::overflow_error("Map full");
        data[count++] = {k, v};
    }
    // ...
};
```

---

## 7.4 Template Specialization

```cpp
// Primary template (generic):
template<typename T>
struct TypeName {
    static std::string get() { return "unknown"; }
};

// Full specialization for int:
template<>
struct TypeName<int> {
    static std::string get() { return "int"; }
};

// Full specialization for double:
template<>
struct TypeName<double> {
    static std::string get() { return "double"; }
};

// Full specialization for char*:
template<>
struct TypeName<char*> {
    static std::string get() { return "char*"; }
};

TypeName<int>::get()     // "int"
TypeName<double>::get()  // "double"
TypeName<float>::get()   // "unknown"

// Partial specialization (only for class templates):
template<typename T>
struct TypeName<T*> {   // Specialization for ALL pointer types
    static std::string get() { return TypeName<T>::get() + "*"; }
};

TypeName<int*>::get()    // "int*"
TypeName<double*>::get() // "double*"

// Function template full specialization:
template<typename T>
T my_abs(T x) { return x < 0 ? -x : x; }

template<>
float my_abs<float>(float x) {
    return std::fabsf(x);  // Use optimized float version
}
```

---

## 7.5 Variadic Templates (C++11)

```cpp
// Variadic template: accepts ANY number of arguments of ANY type

// Base case (zero arguments):
void print() { std::cout << "\n"; }

// Recursive case:
template<typename First, typename... Rest>
void print(First first, Rest... rest) {
    std::cout << first << " ";
    print(rest...);  // Recursively call with remaining args
}

print(1, 2.5, "hello", 'a');  // 1 2.5 hello a

// Count arguments at compile time:
template<typename... Args>
constexpr size_t count_args(Args...) { return sizeof...(Args); }

count_args(1, 2, 3, 4);  // 4

// Perfect forwarding with variadic templates:
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// Fold expressions (C++17) — much cleaner:
template<typename... Args>
auto sum(Args... args) {
    return (... + args);  // (a1 + a2 + a3 + ...)
}

sum(1, 2, 3, 4, 5);  // 15

template<typename... Args>
bool all_true(Args... args) {
    return (... && args);  // All must be true
}

all_true(true, true, false);  // false

// Fold with function call:
template<typename... Args>
void print_all(Args&&... args) {
    ((std::cout << args << " "), ...);  // Comma fold
    std::cout << "\n";
}
```

---

## 7.6 Type Traits and `<type_traits>`

```cpp
#include <type_traits>

// Compile-time type queries:
static_assert(std::is_integral_v<int>);           // true
static_assert(std::is_floating_point_v<double>);  // true
static_assert(!std::is_pointer_v<int>);            // false
static_assert(std::is_pointer_v<int*>);            // true
static_assert(std::is_same_v<int, int>);           // true
static_assert(!std::is_same_v<int, long>);         // false

// Type transformations:
std::remove_const_t<const int>    // → int
std::add_const_t<int>             // → const int
std::remove_reference_t<int&>     // → int
std::remove_pointer_t<int*>       // → int
std::decay_t<int[5]>              // → int* (array decay)
std::conditional_t<true, int, double>  // → int (true case)
std::conditional_t<false, int, double> // → double (false case)

// Custom type trait:
template<typename T>
struct is_string : std::false_type {};

template<>
struct is_string<std::string> : std::true_type {};

template<>
struct is_string<std::string_view> : std::true_type {};

// Usage with constexpr if:
template<typename T>
void serialize(const T& value) {
    if constexpr (is_string<T>::value) {
        // Handle strings specially
        std::cout << "\"" << value << "\"";
    } else if constexpr (std::is_arithmetic_v<T>) {
        std::cout << value;
    } else {
        std::cout << "[complex type]";
    }
}
```

---

## 7.7 `constexpr if` (C++17) — Compile-Time Branching

```cpp
// constexpr if: branch decided at COMPILE TIME (dead branch not compiled)

template<typename T>
auto process(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2;          // Only compiled for integral types
    } else if constexpr (std::is_floating_point_v<T>) {
        return value * 2.0;        // Only compiled for floating point
    } else {
        return std::to_string(value); // Fallback
    }
}

// This allows mixing type-specific operations without breaking compilation:
process(5);     // Uses integer branch
process(3.14);  // Uses floating point branch

// Without constexpr if, you'd need specializations or SFINAE
// constexpr if is almost always cleaner

// Key insight: the false branch is NOT compiled → no "this type doesn't support X" errors!
template<typename T>
void print_size(const T& container) {
    if constexpr (requires { container.size(); }) {  // C++20 requires clause
        std::cout << "Size: " << container.size() << "\n";
    } else {
        std::cout << "No size available\n";
    }
}
```

---

## 7.8 SFINAE — Substitution Failure Is Not An Error

```cpp
// SFINAE: if template substitution fails, that overload is silently removed
// from consideration (not a compile error!)

// enable_if: only enable this overload if condition is true
template<typename T>
typename std::enable_if_t<std::is_integral_v<T>, T>
double_it(T value) {
    return value * 2;
}

template<typename T>
typename std::enable_if_t<std::is_floating_point_v<T>, T>
double_it(T value) {
    return value * 2.0;
}

double_it(5);     // Uses int version
double_it(3.14);  // Uses double version
// double_it("hi"); // ERROR: neither version works for string

// Modern C++20 approach with concepts (much cleaner!):
template<std::integral T>
T double_it_modern(T value) { return value * 2; }

template<std::floating_point T>
T double_it_modern(T value) { return value * 2.0; }
```

---

## 7.9 Concepts (C++20) — Constraining Templates

```cpp
#include <concepts>

// Concept: a named constraint on template parameters
template<typename T>
concept Printable = requires(T t) {
    std::cout << t;  // T must support << to cout
};

template<typename T>
concept Sortable = requires(T container) {
    container.begin();
    container.end();
    std::sort(container.begin(), container.end());
};

template<typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a == b } -> std::convertible_to<bool>;
};

// Using concepts:
template<Printable T>
void print(const T& value) {
    std::cout << value;
}

// Abbreviated function template (C++20):
void print_any(const Printable auto& value) {
    std::cout << value;
}

// Requires clause:
template<typename T>
    requires Comparable<T>
T my_max(T a, T b) {
    return a > b ? a : b;
}

// Built-in concepts from <concepts>:
template<std::integral T>        // T must be an integer type
T factorial(T n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

template<std::totally_ordered T> // T supports <, >, <=, >=, ==, !=
T clamp(T value, T min_val, T max_val) {
    return std::max(min_val, std::min(value, max_val));
}

// Standard concepts include:
// std::same_as, std::derived_from, std::convertible_to
// std::integral, std::floating_point, std::arithmetic
// std::signed_integral, std::unsigned_integral
// std::equality_comparable, std::totally_ordered
// std::movable, std::copyable, std::semiregular, std::regular
// std::invocable, std::predicate, std::relation
```

---

## 7.10 Template Metaprogramming (TMP)

```cpp
// Compute at COMPILE TIME using templates

// Factorial at compile time:
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

static_assert(Factorial<5>::value == 120);  // Computed at compile time!

// Modern C++ prefers constexpr functions:
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

static_assert(factorial(5) == 120);  // Also compile time!

// Fibonacci at compile time:
template<int N>
struct Fib {
    static constexpr int value = Fib<N-1>::value + Fib<N-2>::value;
};

template<> struct Fib<0> { static constexpr int value = 0; };
template<> struct Fib<1> { static constexpr int value = 1; };

static_assert(Fib<10>::value == 55);

// Type list operations:
template<typename... Types>
struct TypeList {};

template<typename List>
struct Size;

template<typename... Types>
struct Size<TypeList<Types...>> {
    static constexpr size_t value = sizeof...(Types);
};

using MyTypes = TypeList<int, double, char, float>;
static_assert(Size<MyTypes>::value == 4);
```

---

## 7.11 `auto`, `decltype`, and `decltype(auto)`

```cpp
// auto: deduce type from initializer
auto x = 42;           // int
auto y = 3.14;         // double
auto z = "hello";      // const char*
auto s = std::string("hi");  // std::string

// auto with references and const:
int n = 5;
auto a = n;            // int (copy)
auto& b = n;           // int& (reference)
const auto& c = n;     // const int& (const ref)
auto* p = &n;          // int*

// decltype: get type of an expression WITHOUT evaluating it
int val = 42;
decltype(val) copy = val;    // int
decltype(val)& ref = val;    // int&

// decltype with expressions:
decltype(3 + 4.0)  d = 0;    // double (int + double = double)
decltype(auto) e = val;       // int (preserves ref/const)

// Useful for generic code:
template<typename T, typename U>
auto multiply(T t, U u) -> decltype(t * u) {
    return t * u;
}

// C++14: just use auto return type:
template<typename T, typename U>
auto multiply2(T t, U u) {
    return t * u;  // Return type deduced
}
```

---

## 📝 Module 07 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| Function templates | Write once, works for any type; T deduced from arguments |
| Class templates | Generic data structures; specify T at instantiation |
| Template specialization | Custom behavior for specific types |
| Variadic templates | Accept any number of arguments |
| Fold expressions | Compact variadic operations (C++17) |
| Type traits | Compile-time type queries and transformations |
| `constexpr if` | Compile-time branching in templates (C++17) |
| SFINAE | Enable/disable overloads based on type properties |
| Concepts | Clean, readable template constraints (C++20) |
| TMP | Computation at compile time; prefer `constexpr` in modern C++ |

---

## ➡️ Next: [Module 08: The Standard Library](../module_08_stdlib/08_stdlib.md)
