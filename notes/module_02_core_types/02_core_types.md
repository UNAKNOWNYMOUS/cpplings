# Module 02: Core Types, Variables & Memory Layout

> *"Every byte in memory has a type. Understanding types means understanding what the CPU is actually doing."*

---

## 2.1 Fundamental Types — The Building Blocks

C++ has a small set of **fundamental types** built into the language. All other types are built from these.

```
┌──────────────────────────────────────────────────────────────────────┐
│                    C++ FUNDAMENTAL TYPES                             │
│                                                                      │
│  BOOLEAN                                                             │
│  ┌────────────────────────────────────────────────────┐              │
│  │ bool        │ 1 byte  │ true (1) or false (0)      │              │
│  └────────────────────────────────────────────────────┘              │
│                                                                      │
│  INTEGER TYPES (signed)                                              │
│  ┌──────────────┬─────────┬────────────────────────────┐             │
│  │ short        │ 2 bytes │ -32,768 to 32,767          │             │
│  │ int          │ 4 bytes │ -2,147,483,648 to 2,147... │             │
│  │ long         │ 4 or 8  │ Platform dependent         │             │
│  │ long long    │ 8 bytes │ ±9.2 × 10^18               │             │
│  └──────────────┴─────────┴────────────────────────────┘             │
│                                                                      │
│  INTEGER TYPES (unsigned)                                            │
│  ┌───────────────────┬────┬────────────────────────────┐             │
│  │ unsigned short    │ 2  │ 0 to 65,535                │             │
│  │ unsigned int      │ 4  │ 0 to 4,294,967,295         │             │
│  │ unsigned long long│ 8  │ 0 to 18.4 × 10^18          │             │
│  └───────────────────┴────┴────────────────────────────┘             │
│                                                                      │
│  FLOATING POINT                                                      │
│  ┌──────────────┬──────────┬───────────────────────────┐             │
│  │ float        │ 4 bytes  │ ~7 significant digits     │             │
│  │ double       │ 8 bytes  │ ~15 significant digits    │             │
│  │ long double  │ 10/16 B  │ ~18-19 significant digits │             │
│  └──────────────┴──────────┴───────────────────────────┘             │
│                                                                      │
│  CHARACTER TYPES                                                     │
│  ┌──────────────┬─────────┬────────────────────────────┐             │
│  │ char         │ 1 byte  │ ASCII character (or byte)  │             │
│  │ char8_t      │ 1 byte  │ UTF-8 code unit (C++20)    │             │
│  │ char16_t     │ 2 bytes │ UTF-16 code unit           │             │
│  │ char32_t     │ 4 bytes │ UTF-32 code unit           │             │
│  │ wchar_t      │ 2 or 4  │ Wide character (platform)  │             │
│  └──────────────┴─────────┴────────────────────────────┘             │
│                                                                      │
│  VOID                                                                │
│  ┌────────────────────────────────────────────────────┐              │
│  │ void         │ no size │ "nothing" or "any type"   │              │
│  └────────────────────────────────────────────────────┘              │
└──────────────────────────────────────────────────────────────────────┘
```

### Checking Sizes at Runtime

```cpp
#include <iostream>
#include <climits>
#include <cfloat>

int main() {
    // sizeof gives you the size in BYTES at compile time
    std::cout << "bool:        " << sizeof(bool)        << " bytes\n";
    std::cout << "char:        " << sizeof(char)        << " bytes\n";
    std::cout << "short:       " << sizeof(short)       << " bytes\n";
    std::cout << "int:         " << sizeof(int)         << " bytes\n";
    std::cout << "long:        " << sizeof(long)        << " bytes\n";
    std::cout << "long long:   " << sizeof(long long)   << " bytes\n";
    std::cout << "float:       " << sizeof(float)       << " bytes\n";
    std::cout << "double:      " << sizeof(double)      << " bytes\n";
    std::cout << "long double: " << sizeof(long double) << " bytes\n";

    // Min and max values
    std::cout << "\nINT_MAX = " << INT_MAX << "\n";   // 2147483647
    std::cout << "INT_MIN = " << INT_MIN << "\n";   // -2147483648
    std::cout << "UINT_MAX = " << UINT_MAX << "\n"; // 4294967295
    std::cout << "DBL_MAX = " << DBL_MAX << "\n";   // ~1.8e308

    return 0;
}
```

### Fixed-Width Integer Types (Prefer These!)

```cpp
#include <cstdint>  // or <stdint.h>

int8_t   a = 127;       // Exactly 8 bits,  signed
int16_t  b = 32767;     // Exactly 16 bits, signed
int32_t  c = 2147483647;// Exactly 32 bits, signed
int64_t  d = 9223372036854775807LL; // Exactly 64 bits

uint8_t  e = 255;       // Exactly 8 bits,  unsigned
uint16_t f = 65535;     // Exactly 16 bits, unsigned
uint32_t g = 4294967295u;
uint64_t h = 18446744073709551615ull;

// "Fastest" types (may be wider for performance):
int_fast32_t  fast_int = 0;
uint_fast64_t fast_uint = 0;

// "Smallest" types:
int_least8_t  small = 0;
```

> **Rule of thumb:** Use `int` for general integer work. Use `int32_t`, `int64_t`, etc. when size matters (serialization, hardware, protocols).

---

## 2.2 Variables — Declarations, Initialization, and Memory

### Variable Declaration and Initialization Styles

```cpp
int a;            // DECLARATION: declared but NOT initialized!
                  // Contains garbage data (undefined behavior to read!)

int b = 5;        // Copy initialization (C-style)
int c(5);         // Direct initialization
int d{5};         // List/brace initialization (C++11) — PREFERRED
int e = {5};      // Copy-list initialization

// Brace initialization is the BEST because:
// 1. It prevents narrowing conversions
// 2. Works uniformly for all types
// 3. Safer

double x{3.14};
double y = 3.14;

// DANGER: Narrowing conversion — brace init catches it!
int z1 = 3.9;    // OK (but lossy! z1 = 3)
int z2{3.9};     // ERROR: narrowing conversion from double to int!
```

### How Variables Live in Memory

```
┌───────────────────────────────────────────────────────────────┐
│                    MEMORY LAYOUT                              │
│                                                               │
│  int x = 42;   (4 bytes, address 0x1000)                      │
│                                                               │
│  Address:  0x1000  0x1001  0x1002  0x1003                     │
│           ┌───────┬───────┬───────┬───────┐                   │
│           │  42   │   0   │   0   │   0   │   (little-endian) │
│           │0x2A   │0x00   │0x00   │0x00   │                   │
│           └───────┴───────┴───────┴───────┘                   │
│                                                               │
│  The integer 42 in binary: 00000000 00000000 00000000 00101010│
│  In little-endian: least significant byte stored first        │
│                                                               │
│  char c = 'A';   (1 byte, ASCII 65 = 0x41)                    │
│  Address:  0x1004                                             │
│           ┌───────┐                                           │
│           │  65   │                                           │
│           │0x41   │                                           │
│           └───────┘                                           │
└───────────────────────────────────────────────────────────────┘
```

### Little-Endian vs Big-Endian

```cpp
#include <iostream>
#include <cstdint>

// Check endianness at compile time (C++20)
#include <bit>
if constexpr (std::endian::native == std::endian::little) {
    // x86, ARM (most common): bytes stored LSB first
} else {
    // PowerPC, SPARC, network protocols: bytes stored MSB first
}

// Practical example: 0x12345678 in memory
// Little-endian: 78 56 34 12
// Big-endian:    12 34 56 78
```

---

## 2.3 Literals

```cpp
// INTEGER LITERALS
int decimal    = 42;          // Base 10
int octal      = 052;         // Base 8  (leading 0)
int hex        = 0x2A;        // Base 16 (0x prefix)
int binary     = 0b00101010;  // Base 2  (0b prefix, C++14)
long long big  = 9'223'372'036'854LL;  // Digit separators (C++14)
unsigned int u = 42u;         // 'u' suffix = unsigned
long long ll   = 42LL;        // 'LL' suffix = long long

// FLOATING POINT LITERALS
double  d1 = 3.14;
double  d2 = 3.14e10;         // 3.14 × 10^10
float   f1 = 3.14f;           // 'f' suffix = float
long double ld = 3.14L;       // 'L' suffix = long double
double hex_fp = 0x1.8p+1;     // Hex float literal = 3.0

// CHARACTER LITERALS
char a = 'A';
char newline = '\n';           // Escape sequences
char tab     = '\t';
char null    = '\0';
char bell    = '\a';
char hex_c   = '\x41';        // Hex: 'A'
char oct_c   = '\101';        // Octal: 'A'

// STRING LITERALS
const char* s1 = "Hello";     // C-string (null-terminated array of chars)
const char* s2 = "Line1\nLine2";
const char* s3 = R"(Raw string: no need to escape \n or "quotes")";

// BOOLEAN LITERALS
bool t = true;   // = 1
bool f = false;  // = 0

// NULL POINTER LITERAL (C++11)
int* ptr = nullptr;  // Use this, NOT NULL or 0
```

---

## 2.4 `const` and `constexpr`

### `const` — Runtime Immutability

```cpp
const int MAX_SIZE = 100;   // Cannot be changed after initialization
// MAX_SIZE = 200;           // ERROR: assignment of read-only variable

const double PI = 3.14159265358979;

// const with pointers — the tricky part!
int x = 5;
int y = 10;

// Read right-to-left from the variable name:
const int*       p1 = &x;  // pointer to CONST int
                            // Can change where p1 points, cannot change *p1
int* const       p2 = &x;  // CONST pointer to int
                            // Cannot change where p2 points, can change *p2
const int* const p3 = &x;  // CONST pointer to CONST int
                            // Cannot change either

*p1 = 10;   // ERROR: *p1 is const
p1  = &y;   // OK: p1 itself is not const

*p2 = 10;   // OK: *p2 is not const
p2  = &y;   // ERROR: p2 is const

// Mnemonic: "const applies to whatever is directly to its LEFT"
// (or to the RIGHT if nothing is on the left)
```

```
┌───────────────────────────────────────────────────────────────┐
│               CONST POINTER CHEAT SHEET                       │
│                                                               │
│  const int*  p    ←  "p points to something that is const"    │
│  [pointer can move, pointee cannot change]                    │
│                                                               │
│  int* const  p    ←  "p itself is const"                      │
│  [pointer cannot move, pointee can change]                    │
│                                                               │
│  const int* const p ← "p is const, points to const"           │
│  [neither can change]                                         │
│                                                               │
│  TRICK: Read declarations RIGHT TO LEFT:                      │
│  "int* const p" = "p is a const pointer to int"               │
│  "const int* p" = "p is a pointer to const int"               │
└───────────────────────────────────────────────────────────────┘
```

### `constexpr` — Compile-Time Computation

```cpp
// constexpr = evaluated at COMPILE TIME (not runtime)
constexpr int ARRAY_SIZE = 1024;
constexpr double GRAVITY = 9.80665;

// constexpr functions — evaluated at compile time when possible
constexpr int square(int x) {
    return x * x;
}

constexpr int s = square(5);   // Computed at compile time: s = 25
// Equivalent to: constexpr int s = 25;

int runtime_val = 7;
int r = square(runtime_val);   // Computed at RUNTIME (not constexpr context)

// constexpr vs const:
// const:    value fixed after initialization (can be set at runtime)
// constexpr: value MUST be known at compile time
const int n = square(5);      // OK, but might not be compile-time
constexpr int m = square(5);  // Guaranteed compile-time
```

### `consteval` (C++20) — Must Be Compile-Time

```cpp
consteval int must_be_compiletime(int x) {
    return x * 2;
}

constexpr int a = must_be_compiletime(5);  // OK: 10
// int b = must_be_compiletime(runtime_val); // ERROR: must be compile-time
```

---

## 2.5 Type Modifiers: `signed`, `unsigned`, `short`, `long`

```cpp
// All of these are valid and common:
signed int     a = -5;    // same as 'int'
unsigned int   b = 5u;
short int      c = 100;   // same as 'short'
long int       d = 100L;  // same as 'long'
long long int  e = 100LL; // same as 'long long'

// Shorthand (you can drop 'int'):
short    f = 100;
long     g = 100L;
unsigned h = 5u;

// DANGER: unsigned wraparound
unsigned int u = 0;
u -= 1;  // u = 4294967295 (wraps around!), NOT -1
          // This is DEFINED behavior for unsigned, unlike signed overflow
```

---

## 2.6 Type Inference with `auto`

```cpp
// 'auto' deduces the type from the initializer
auto x = 42;           // int
auto y = 3.14;         // double
auto z = 3.14f;        // float
auto s = "hello";      // const char*
auto b = true;         // bool

// auto preserves the deduced type exactly
auto a = 1;            // int (NOT long, NOT short)
auto b = 1u;           // unsigned int
auto c = 1L;           // long
auto d = 1LL;          // long long
auto e = 1.0;          // double
auto f = 1.0f;         // float

// auto drops const and references by default!
const int ci = 42;
auto a1 = ci;          // a1 is int (const dropped!)
auto& a2 = ci;         // a2 is const int& (reference preserves const)
const auto a3 = ci;    // a3 is const int

// auto is very useful with complex types:
std::vector<std::pair<int, std::string>> vec;
auto it = vec.begin();  // Much better than writing the full iterator type
```

---

## 2.7 Type Aliases

```cpp
// Method 1: typedef (old C-style)
typedef unsigned long long uint64;
typedef int (*FuncPtr)(double, double);  // function pointer type alias

// Method 2: using (C++11, preferred)
using uint64   = unsigned long long;
using FuncPtr  = int (*)(double, double);

// Template type aliases (only possible with 'using')
template<typename T>
using Vec = std::vector<T>;

Vec<int>    integers;   // Same as std::vector<int>
Vec<double> doubles;    // Same as std::vector<double>
```

---

## 2.8 The Stack vs The Heap

This is one of the most important concepts in C++.

```
┌────────────────────────────────────────────────────────────────────────┐
│                    PROCESS MEMORY MAP                                  │
│                                                                        │
│  High Addresses                                                        │
│  ┌─────────────────────────────────────────────────────┐               │
│  │                  KERNEL SPACE                       │               │
│  │               (OS, not accessible)                  │               │
│  ├─────────────────────────────────────────────────────┤               │
│  │                    STACK                            │               │
│  │  - Local variables                                  │               │
│  │  - Function call frames                             │               │
│  │  - Grows DOWNWARD (toward lower addresses)          │               │
│  │  - Fast allocation: just move stack pointer         │               │
│  │  - Fixed size (usually 1-8 MB)                      │               │
│  │  - Automatically managed (LIFO cleanup)             │               │
│  │  ↓↓↓  (grows down)                                  │               │
│  │                                                     │               │
│  │         ~~~~~~~~~~~~~ FREE SPACE ~~~~~~~~~~~~~~     │               │
│  │                                                     │               │
│  │  ↑↑↑  (grows up)                                    │               │
│  │                    HEAP                             │               │
│  │  - Dynamic allocations (new / malloc)               │               │
│  │  - Grows UPWARD (toward higher addresses)           │               │
│  │  - Slow: must find free block, update metadata      │               │
│  │  - Unlimited (constrained by RAM)                   │               │
│  │  - Manual management (or smart pointers)            │               │
│  ├─────────────────────────────────────────────────────┤               │
│  │                    BSS SEGMENT                      │               │
│  │  (uninitialized global/static variables)            │               │
│  ├─────────────────────────────────────────────────────┤               │
│  │                    DATA SEGMENT                     │               │
│  │  (initialized global/static variables)              │               │
│  ├─────────────────────────────────────────────────────┤               │
│  │                    TEXT SEGMENT                     │               │
│  │  (machine code, read-only)                          │               │
│  └─────────────────────────────────────────────────────┘               │
│  Low Addresses                                                         │
└────────────────────────────────────────────────────────────────────────┘
```

### Stack Variables (Automatic Storage Duration)

```cpp
void myFunction() {
    int x = 10;      // On the STACK — automatic lifetime
    double d = 3.14; // On the STACK

    // When myFunction returns:
    // - Stack frame is popped
    // - x and d no longer exist
    // - Memory is "freed" instantly (just stack pointer moves)
}
// x and d are GONE here
```

### Heap Variables (Dynamic Storage Duration)

```cpp
void myFunction() {
    int* p = new int(42);   // Allocated on the HEAP

    // p (the pointer) is on the stack
    // *p (the int) is on the heap

    std::cout << *p << "\n";  // 42

    delete p;   // MUST manually free! Otherwise: memory leak
    p = nullptr;  // Good practice: avoid dangling pointer
}
```

### Stack Frame Layout

```
┌────────────────────────────────────────────────────┐
│  STACK FRAME for: int foo(int a, int b)            │
│                                                    │
│  ┌──────────────────────────────────────┐          │
│  │  Return address (where to go after)  │ ← rsp+0  │
│  ├──────────────────────────────────────┤          │
│  │  Saved rbp (caller's frame pointer)  │ ← rsp+8  │
│  ├──────────────────────────────────────┤          │
│  │  Local variable: int x               │ ← rbp-4  │
│  ├──────────────────────────────────────┤          │
│  │  Local variable: int y               │ ← rbp-8  │
│  ├──────────────────────────────────────┤          │
│  │  Parameter: int a                    │ ← rbp+16 │
│  ├──────────────────────────────────────┤          │
│  │  Parameter: int b                    │ ← rbp+24 │
│  └──────────────────────────────────────┘          │
└────────────────────────────────────────────────────┘
```

---

## 2.9 Type Conversions

### Implicit (Automatic) Conversions

```cpp
// Implicit conversions happen automatically
int  i = 42;
long l = i;       // int → long: safe (widening)
double d = i;     // int → double: safe (widening)

double pi = 3.14;
int  n = pi;      // double → int: LOSSY! n = 3, decimal dropped
                  // Compiler may warn, but it compiles

// Integer promotions: small types promoted to int for arithmetic
char  a = 5;
char  b = 3;
auto  result = a + b;  // result is INT, not char!
                       // chars are promoted to int for the operation
```

### Explicit Casts (Use These Instead!)

```cpp
double pi = 3.14159;

// C-style cast (AVOID — no safety checks)
int n1 = (int)pi;     // 3 — works but old style

// C++ static_cast (use for safe, well-defined conversions)
int n2 = static_cast<int>(pi);    // 3 — explicit intent, compiler checks

// reinterpret_cast (for type-punning, low-level, DANGEROUS)
int x = 42;
char* bytes = reinterpret_cast<char*>(&x);  // View int as bytes

// const_cast (to add/remove const — almost always a code smell)
const int ci = 42;
int* p = const_cast<int*>(&ci);  // Remove const
*p = 99;  // UB if ci was declared const! Only valid if the
           // original object was not const

// dynamic_cast (for polymorphism — covered in OOP module)
```

### The Usual Arithmetic Conversions

When you mix types in arithmetic, C++ follows a hierarchy:

```
long double → (converts the other to long double)
    ↑
  double    → (converts the other to double)
    ↑
  float     → (converts the other to float)
    ↑
  long long → (converts the other to long long)
    ↑
  long      → (converts the other to long)
    ↑
  int       → (both promoted to int minimum)
```

```cpp
int i = 5;
double d = 2.0;
auto result = i + d;  // i converted to double, result is double (7.0)

int a = 5;
unsigned int b = 3u;
auto r = a - b;  // Both converted to unsigned: r is unsigned int
// If a < b, this wraps! r = UINT_MAX - 2 (not -2!)
```

---

## 2.10 Scope and Lifetime

```cpp
int global_var = 10;   // Global scope — lives for the entire program
                       // Stored in DATA segment

void example() {
    int local = 5;          // Local scope — lives until '}' of this function
    
    {
        int inner = 3;      // Inner block scope — lives until inner '}'
        int local = 99;     // Shadows outer 'local'! (name hiding)
        std::cout << inner;  // 3
        std::cout << local;  // 99 (inner one)
    }  // inner and inner-local destroyed here

    // 'inner' is gone, 'local' is back to being the outer one
    std::cout << local;  // 5

}  // local destroyed here

// STATIC local variables: initialized once, live until program ends
void counter() {
    static int count = 0;  // Only initialized ONCE
    ++count;
    std::cout << count << "\n";
}

int main() {
    counter();  // 1
    counter();  // 2
    counter();  // 3
    return 0;
}
```

---

## 2.11 References — Aliases for Variables

References are **not pointers** — they are aliases (other names) for existing variables.

```cpp
int x = 42;
int& ref = x;    // ref is an alias for x (must be initialized!)

ref = 100;       // Same as: x = 100
std::cout << x;  // 100

// References CANNOT be rebound — they always refer to the same object
int y = 99;
ref = y;    // This ASSIGNS y's value to x (through ref), does NOT rebind ref!

// References vs Pointers:
//   Pointer: can be null, can be rebound, has its own memory address
//   Reference: always valid (no null ref), cannot rebind, no address

// References are ESSENTIAL for function parameters (pass by reference):
void increment(int& n) {
    n++;  // Modifies the ORIGINAL variable
}

int main() {
    int a = 5;
    increment(a);
    std::cout << a;  // 6
}
```

### Reference Memory Layout

```
┌──────────────────────────────────────────────────────────┐
│  int x = 42;                                             │
│  int& ref = x;                                           │
│                                                          │
│  Memory:                                                 │
│  Address 0x1000: [  42  ]  ← x (and also ref)            │
│                                                          │
│  A reference is typically implemented as a pointer       │
│  under the hood, but the C++ standard doesn't require    │
│  it to have any storage at all. It's a compile-time      │
│  alias concept.                                          │
└──────────────────────────────────────────────────────────┘
```

---

## 2.12 Structured Bindings (C++17)

```cpp
#include <tuple>
#include <map>

// Decompose pairs
std::pair<int, double> p = {42, 3.14};
auto [integer, decimal] = p;  // integer=42, decimal=3.14

// Decompose structs
struct Point { int x; int y; };
Point pt = {10, 20};
auto [px, py] = pt;   // px=10, py=20

// Very useful in range-for over maps:
std::map<std::string, int> scores = {{"Alice", 90}, {"Bob", 85}};
for (const auto& [name, score] : scores) {
    std::cout << name << ": " << score << "\n";
}
```

---

## 📝 Module 02 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| Fundamental Types | `bool`, `int`, `double`, `char` — know their sizes |
| Fixed-Width Types | Use `int32_t`, `uint64_t` when size matters |
| Variable Init | Prefer `{}` brace initialization — prevents narrowing |
| `const` | Compile-time promise of immutability |
| `constexpr` | Value/function evaluated at compile time |
| `auto` | Deduces type from initializer; drops const/refs |
| Stack vs Heap | Stack: fast, auto; Heap: manual, flexible |
| Scope & Lifetime | Variables live until their enclosing `}` |
| References | Aliases, not pointers — always valid, cannot rebind |
| Type Conversions | Use `static_cast<>` for explicit, safe conversions |

---

## ➡️ Next: [Module 03: Control Flow & Expressions](../module_03_control_flow/03_control_flow.md)
