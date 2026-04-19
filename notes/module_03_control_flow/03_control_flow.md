# Module 03: Control Flow & Expressions

> *"Code without control flow is just a list of instructions. Control flow is what makes programs intelligent."*

---

## 3.1 Operators — Complete Reference

### Arithmetic Operators

```cpp
int a = 10, b = 3;

int sum  = a + b;   // 13
int diff = a - b;   // 7
int prod = a * b;   // 30
int quot = a / b;   // 3  (integer division — truncates!)
int rem  = a % b;   // 1  (modulo/remainder)
int neg  = -a;      // -10 (unary negation)

// IMPORTANT: Integer division truncates toward zero
int x = 7 / 2;   // 3  (not 3.5!)
int y = -7 / 2;  // -3 (not -4!)

// For floating-point division:
double d = 7.0 / 2;   // 3.5
double e = static_cast<double>(7) / 2;  // 3.5

// DANGER: Division by zero
int bad = 10 / 0;    // UB for integers! (runtime crash usually)
double bad2 = 10.0 / 0.0;  // IEEE 754: Infinity (not UB for doubles)
```

### Increment and Decrement

```cpp
int n = 5;

// Pre-increment: increment FIRST, then use
int a = ++n;  // n becomes 6, a = 6

// Post-increment: use FIRST, then increment
int b = n++;  // b = 6 (old value), n becomes 7

// Pre vs Post — does it matter?
// In simple statements: no difference
n++;   // Same as ++n here
++n;   // Same result

// But in expressions: VERY different!
int x = 5;
int y = x++ * 2;  // y = 5*2 = 10,  x = 6 (post: old value used)
int z = ++x * 2;  // z = 7*2 = 14,  x = 7 (pre: new value used)

// PERFORMANCE: Pre-increment is often faster for iterators/objects
// (post-increment must save a copy)
// For plain ints: identical performance (compiler optimizes)
```

### Comparison Operators

```cpp
int a = 5, b = 10;

bool eq  = (a == b);  // false: equal?
bool neq = (a != b);  // true:  not equal?
bool lt  = (a <  b);  // true:  less than?
bool gt  = (a >  b);  // false: greater than?
bool lte = (a <= b);  // true:  less than or equal?
bool gte = (a >= b);  // false: greater than or equal?

// C++20: Spaceship operator <=> (three-way comparison)
auto result = a <=> b;
// result < 0 means a < b
// result == 0 means a == b
// result > 0 means a > b

#include <compare>
std::strong_ordering r = 5 <=> 10;  // std::strong_ordering::less

// FLOATING POINT COMPARISON — Never use == for floats!
double x = 0.1 + 0.2;
double y = 0.3;
bool bad  = (x == y);   // FALSE! (0.1+0.2 = 0.30000000000000004)
bool good = (std::abs(x - y) < 1e-9);  // TRUE: epsilon comparison

#include <cmath>
bool better = std::fabs(x - y) < std::numeric_limits<double>::epsilon();
```

### Logical Operators

```cpp
bool a = true, b = false;

bool and_result = a && b;  // false: both must be true
bool or_result  = a || b;  // true:  at least one must be true
bool not_result = !a;      // false: negation

// SHORT-CIRCUIT EVALUATION (critical concept!)
// && stops at FIRST false
// || stops at FIRST true

int n = 0;
if (n != 0 && (10 / n > 2)) {  // Safe! If n==0, right side NOT evaluated
    // ...
}

// If we reversed: (10/n > 2) && (n != 0) — would crash! (division by zero)

// Practical use: null pointer check
std::string* ptr = nullptr;
if (ptr != nullptr && ptr->size() > 0) {  // Safe!
    // ptr->size() only called if ptr is not null
}
```

### Bitwise Operators

```cpp
// Bitwise operators work on individual BITS
uint8_t a = 0b11001010;  // 202 decimal
uint8_t b = 0b10110101;  // 181 decimal

uint8_t and_  = a & b;   // 0b10000000  AND: bit is 1 only if both are 1
uint8_t or_   = a | b;   // 0b11111111  OR:  bit is 1 if either is 1
uint8_t xor_  = a ^ b;   // 0b01111111  XOR: bit is 1 if exactly one is 1
uint8_t not_  = ~a;      // 0b00110101  NOT: flips all bits

// SHIFT operators
uint8_t left  = a << 2;  // Shift left 2 bits = multiply by 4
uint8_t right = a >> 1;  // Shift right 1 bit = divide by 2

// Common bitwise patterns:
// Setting a bit:
int flags = 0;
flags |= (1 << 3);   // Set bit 3

// Clearing a bit:
flags &= ~(1 << 3);  // Clear bit 3

// Testing a bit:
bool is_set = (flags & (1 << 3)) != 0;  // Is bit 3 set?

// Toggling a bit:
flags ^= (1 << 3);   // Flip bit 3

// EXAMPLE: Using bit flags for options
enum class Options : uint32_t {
    NONE       = 0,
    VERBOSE    = 1 << 0,  // 0001
    DEBUG      = 1 << 1,  // 0010
    LOGGING    = 1 << 2,  // 0100
    OPTIMIZED  = 1 << 3,  // 1000
};

uint32_t opts = static_cast<uint32_t>(Options::VERBOSE) |
                static_cast<uint32_t>(Options::DEBUG);
// opts = 0011 = VERBOSE + DEBUG
```

### Assignment Operators

```cpp
int x = 10;

x += 5;   // x = x + 5  = 15
x -= 3;   // x = x - 3  = 12
x *= 2;   // x = x * 2  = 24
x /= 4;   // x = x / 4  = 6
x %= 4;   // x = x % 4  = 2
x <<= 1;  // x = x << 1 = 4
x >>= 1;  // x = x >> 1 = 2
x &= 0xFF;// x = x & 255
x |= 0x10;// x = x | 16
x ^= 0x0F;// x = x ^ 15
```

### Operator Precedence (Higher = Evaluated First)

```
┌─────────────────────────────────────────────────────────┐
│           OPERATOR PRECEDENCE TABLE                     │
│  (Highest to Lowest)                                    │
│                                                         │
│  1. ::          Scope resolution                        │
│  2. () [] . ->  Postfix, function call, member access   │
│  3. ++ -- ~ !   Prefix, unary (right-to-left)           │
│  4. * / %       Multiplicative                          │
│  5. + -         Additive                                │
│  6. << >>       Bit shift                               │
│  7. < <= > >=   Relational                              │
│  8. == !=       Equality                                │
│  9. &           Bitwise AND                             │
│ 10. ^           Bitwise XOR                             │
│ 11. |           Bitwise OR                              │
│ 12. &&          Logical AND                             │
│ 13. ||          Logical OR                              │
│ 14. ?:          Ternary (right-to-left)                 │
│ 15. = += -= etc Assignment (right-to-left)              │
│ 16. ,           Comma operator                          │
└─────────────────────────────────────────────────────────┘
```

```cpp
// When in doubt, USE PARENTHESES!
int result = 2 + 3 * 4;      // 14 (not 20) — * before +
int clear  = (2 + 3) * 4;   // 20 — explicit

bool check = x == 5 || y > 3;         // Unclear intent
bool clear2 = (x == 5) || (y > 3);   // Explicit, readable
```

---

## 3.2 The Conditional (`if`) Statement

```cpp
int score = 85;

// Simple if
if (score >= 90) {
    std::cout << "A\n";
}

// if-else
if (score >= 90) {
    std::cout << "A\n";
} else {
    std::cout << "Not A\n";
}

// if-else if-else chain
if (score >= 90) {
    std::cout << "A\n";
} else if (score >= 80) {
    std::cout << "B\n";
} else if (score >= 70) {
    std::cout << "C\n";
} else if (score >= 60) {
    std::cout << "D\n";
} else {
    std::cout << "F\n";
}

// IMPORTANT: Always use braces {}!
// DANGEROUS (no braces):
if (score > 90)
    std::cout << "High\n";
    std::cout << "Done\n";  // This runs ALWAYS! (not part of if)

// SAFE (with braces):
if (score > 90) {
    std::cout << "High\n";
    std::cout << "Done\n";  // Only runs if score > 90
}
```

### if with Initializer (C++17)

```cpp
// Declare a variable scoped to the if block
if (int n = compute_value(); n > 0) {
    // n is only in scope here
    std::cout << "Positive: " << n << "\n";
} else {
    std::cout << "Non-positive: " << n << "\n";
}
// n is gone here

// Very useful for error handling:
if (auto result = try_operation(); result.has_value()) {
    process(*result);
}
```

---

## 3.3 The Ternary (Conditional) Operator

```cpp
// condition ? value_if_true : value_if_false
int a = 5, b = 10;
int max_val = (a > b) ? a : b;   // max_val = 10

// Ternary is an EXPRESSION (returns a value), not a statement
// Perfect for inline initialization:
std::string status = (score >= 60) ? "Pass" : "Fail";

// Can be nested (but avoid — hard to read):
std::string grade = (score >= 90) ? "A" :
                    (score >= 80) ? "B" :
                    (score >= 70) ? "C" : "F";
```

---

## 3.4 `switch` Statement

```cpp
int day = 3;

switch (day) {
    case 1:
        std::cout << "Monday\n";
        break;   // CRITICAL: break prevents fall-through!
    case 2:
        std::cout << "Tuesday\n";
        break;
    case 3:
        std::cout << "Wednesday\n";
        break;
    case 6:
    case 7:
        // Intentional fall-through (both 6 and 7 are weekend)
        [[fallthrough]];  // C++17: suppress compiler warning
    case 5:
        std::cout << "Weekend or Friday\n";
        break;
    default:
        std::cout << "Unknown day\n";
        break;  // Good practice even in default
}
```

### What Happens Without `break`?

```cpp
int x = 1;
switch (x) {
    case 1:
        std::cout << "One\n";
        // NO BREAK! Falls through to case 2!
    case 2:
        std::cout << "Two\n";
        // NO BREAK! Falls through to case 3!
    case 3:
        std::cout << "Three\n";
        break;
}
// Output:
// One
// Two
// Three
// (All three execute because of fall-through!)
```

### switch with Initializer (C++17)

```cpp
switch (auto val = get_value(); val) {
    case 0: std::cout << "Zero\n"; break;
    case 1: std::cout << "One\n";  break;
    default: std::cout << "Other: " << val << "\n";
}
```

---

## 3.5 Loops

### `while` Loop

```cpp
int n = 10;

// while: check FIRST, then execute
while (n > 0) {
    std::cout << n << " ";
    n--;
}
// Output: 10 9 8 7 6 5 4 3 2 1

// Potential infinite loop:
while (true) {
    // do work
    if (should_stop()) break;
}
```

### `do-while` Loop

```cpp
int n = 0;

// do-while: execute FIRST, then check
// Body always runs AT LEAST ONCE
do {
    std::cout << "Enter a positive number: ";
    std::cin >> n;
} while (n <= 0);  // Repeat if n is not positive
```

### `for` Loop

```cpp
// for (init; condition; update) { body }
for (int i = 0; i < 10; i++) {
    std::cout << i << " ";
}
// Output: 0 1 2 3 4 5 6 7 8 9

// Multiple variables:
for (int i = 0, j = 10; i < j; i++, j--) {
    std::cout << i << "," << j << " ";
}

// Infinite loop:
for (;;) {  // All three parts optional
    // do work
    if (done()) break;
}

// Reverse iteration:
for (int i = 9; i >= 0; i--) {
    std::cout << i << " ";
}

// Iterating over arrays:
int arr[] = {10, 20, 30, 40, 50};
int size = sizeof(arr) / sizeof(arr[0]);  // = 5
for (int i = 0; i < size; i++) {
    std::cout << arr[i] << " ";
}
```

### Range-Based `for` Loop (C++11) — Prefer This!

```cpp
#include <vector>
#include <string>

std::vector<int> nums = {1, 2, 3, 4, 5};

// Read-only iteration (copy):
for (int n : nums) {
    std::cout << n << " ";
}

// Read-only iteration (by reference — more efficient, no copy):
for (const int& n : nums) {
    std::cout << n << " ";
}

// Modification:
for (int& n : nums) {
    n *= 2;  // Doubles each element
}

// Works with any container that has begin()/end():
std::string s = "Hello";
for (char c : s) {
    std::cout << c << " ";  // H e l l o
}

// C++20: Range-for with initializer
for (auto& container = get_container(); auto& elem : container) {
    process(elem);
}
```

### Loop Control: `break`, `continue`, `goto`

```cpp
// break: exit the innermost loop immediately
for (int i = 0; i < 10; i++) {
    if (i == 5) break;    // Stop when i == 5
    std::cout << i << " ";  // 0 1 2 3 4
}

// continue: skip to next iteration
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) continue;   // Skip even numbers
    std::cout << i << " ";      // 1 3 5 7 9
}

// break only breaks the INNERMOST loop!
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (j == 1) break;     // Only breaks inner loop
        std::cout << i << "," << j << " ";
    }
}
// 0,0  1,0  2,0

// To break out of nested loops — use goto (one valid use case):
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (i == 1 && j == 1) goto done;
        std::cout << i << "," << j << " ";
    }
}
done:
    std::cout << "\nDone!\n";

// Better alternative for nested break: use a flag or lambda
auto found = [&]() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 1 && j == 1) return true;
        }
    }
    return false;
};
```

---

## 3.6 The `for` Loop — How the Compiler Sees It

```
The for loop:
    for (init; condition; update) { body }

Is exactly equivalent to:
    {
        init;
        while (condition) {
            body;
            update;
        }
    }

Assembly perspective (simplified):
    mov eax, 0        ; i = 0
loop_start:
    cmp eax, 10       ; i < 10?
    jge loop_end      ; if not, jump to end
    ; ... body executes ...
    add eax, 1        ; i++
    jmp loop_start    ; go back to start
loop_end:
```

---

## 3.7 Expressions vs Statements

An **expression** produces a value. A **statement** is an instruction.

```cpp
// EXPRESSIONS (have a value):
5 + 3        // value: 8
x = 10       // assignment is an expression! value: 10
x++          // value: old x
x > 0        // value: true or false
func()       // value: return value of func

// STATEMENTS (do something, no value):
int x = 5;          // declaration statement
x++;                // expression statement
if (x > 0) {...}    // selection statement
for (;;) {...}      // iteration statement
return x;           // jump statement

// Since assignment is an expression, you can chain:
int a, b, c;
a = b = c = 0;      // Right-to-left: c=0, b=0, a=0
// (This is c = 0, returns 0; b = 0, returns 0; a = 0)

// COMMON MISTAKE: Assignment in condition (not always a bug!)
while (int ch = getchar() != EOF) {  // BUG: ch = (getchar() != EOF)
    // ...
}
while ((ch = getchar()) != EOF) {    // CORRECT: get char, compare to EOF
    // ...
}
```

---

## 3.8 `constexpr if` (C++17) — Compile-Time Branching

```cpp
#include <type_traits>

template<typename T>
void print_info(T value) {
    // This branch is chosen at COMPILE TIME, not runtime
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integer: " << value << "\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Float: " << value << "\n";
    } else {
        std::cout << "Other type\n";
    }
}

// The DISCARDED branch is not compiled at all for that type
// This allows different code paths to use different member functions
// without compilation errors (unlike regular if)
```

---

## 3.9 `[[likely]]` and `[[unlikely]]` (C++20) — Branch Hints

```cpp
// Tell the compiler which branch is more likely to be taken
// This helps branch prediction and code layout

int n = get_value();

if (n > 0) [[likely]] {
    // This branch executes most of the time
    fast_path();
} else [[unlikely]] {
    // Rare case
    slow_path();
}

// In loops:
for (int i = 0; i < 1000; i++) {
    if (should_skip(i)) [[unlikely]] {
        continue;
    }
    process(i);
}
```

---

## 3.10 Exception Handling Basics

```cpp
#include <stdexcept>
#include <iostream>

// Throwing an exception
void divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero!");
    }
    std::cout << a / b << "\n";
}

// Catching exceptions
int main() {
    try {
        divide(10, 2);   // OK: prints 5
        divide(10, 0);   // Throws!
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    catch (const std::exception& e) {  // Catch-all for std exceptions
        std::cerr << "Std exception: " << e.what() << "\n";
    }
    catch (...) {  // Catch absolutely anything
        std::cerr << "Unknown exception!\n";
    }

    return 0;
}
```

### Standard Exception Hierarchy

```
std::exception
├── std::logic_error
│   ├── std::invalid_argument
│   ├── std::domain_error
│   ├── std::length_error
│   ├── std::out_of_range
│   └── std::future_error
├── std::runtime_error
│   ├── std::range_error
│   ├── std::overflow_error
│   ├── std::underflow_error
│   └── std::system_error
└── std::bad_alloc         (thrown by 'new' on failure)
    std::bad_cast          (thrown by dynamic_cast)
    std::bad_typeid        (thrown by typeid)
```

---

## 3.11 `assert` and `static_assert`

```cpp
#include <cassert>

// assert: runtime check (disabled in release builds with NDEBUG)
int divide(int a, int b) {
    assert(b != 0);  // Crash with message if b == 0 (debug only)
    return a / b;
}

// static_assert: COMPILE-TIME check (always active)
static_assert(sizeof(int) == 4, "int must be 4 bytes on this platform");
static_assert(sizeof(void*) == 8, "Must be 64-bit platform");

template<typename T>
void process(T val) {
    static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");
    // ...
}
```

---

## Module 03 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| Arithmetic ops | Integer division truncates; use `%` for remainder |
| Pre vs Post ++/-- | Pre returns new value; post returns old value |
| Short-circuit eval | `&&` and `||` stop early; use for safety checks |
| Bitwise ops | Work on individual bits; use for flags/masks |
| if-else | Always use braces; use `if (init; cond)` in C++17 |
| switch | Always use `break`; add `[[fallthrough]]` for intentional fall-through |
| for loop | Range-for is preferred for containers |
| break/continue | Only affects innermost loop |
| constexpr if | Compile-time branching in templates |
| Exceptions | throw/try/catch; use standard exception types |

---

## ➡️ Next: [Module 04: Functions — Deep Dive](../module_04_functions/04_functions.md)
