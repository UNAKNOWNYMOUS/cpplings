# Module 01: Foundations — How C++ Works Under the Hood

> *"To master a language, you must understand what happens when you compile."*

---

## 1.1 What Is C++?

C++ is a **compiled, statically-typed, multi-paradigm, systems-level programming language** created by Bjarne Stroustrup at Bell Labs, beginning in 1979. It was originally called "C with Classes."

### Why C++ Matters
- Used in **operating systems**, **game engines**, **databases**, **browsers**, **trading systems**, and **embedded systems**
- Gives you **direct control over hardware** and memory
- Zero-cost abstractions: you pay only for what you use
- The **fastest general-purpose language** available

### C++ Timeline

```
C (1972)
  └── C with Classes (1979)
        └── C++ 1.0 (1985)
              └── C++98 (first ISO standard)
                    └── C++03 (bug-fix)
                          └── C++11 ← REVOLUTION ("Modern C++")
                                └── C++14 (refinements)
                                      └── C++17 (big additions)
                                            └── C++20 (concepts, ranges, coroutines)
                                                  └── C++23 (current stable)
```

---

## 1.2 The C++ Compilation Pipeline

This is the most important diagram to understand. When you write `.cpp` code, **multiple stages** transform it into a running program.

```
┌─────────────────────────────────────────────────────────────────────┐
│                    THE C++ COMPILATION PIPELINE                     │
│                                                                     │
│  Source Files (.cpp)                                                │
│  ┌──────────────┐                                                   │
│  │  main.cpp    │                                                   │
│  │  utils.cpp   │                                                   │
│  └──────┬───────┘                                                   │
│         │                                                           │
│         ▼  STAGE 1: PREPROCESSOR (cpp)                              │
│  ┌──────────────────────────────────────────┐                       │
│  │  - Expands #include directives           │                       │
│  │  - Expands #define macros                │                       │
│  │  - Processes #ifdef / #ifndef            │                       │
│  │  - Strips comments                       │                       │
│  │  Output: Translation Unit (.i file)      │                       │
│  └──────────────────┬───────────────────────┘                       │
│                     │                                               │
│                     ▼  STAGE 2: COMPILER FRONT-END                  │
│  ┌──────────────────────────────────────────┐                       │
│  │  - Lexical Analysis (tokens)             │                       │
│  │  - Syntax Analysis (AST)                 │                       │
│  │  - Semantic Analysis (type checking)     │                       │
│  │  - Template instantiation                │                       │
│  └──────────────────┬───────────────────────┘                       │
│                     │                                               │
│                     ▼  STAGE 3: COMPILER BACK-END                   │
│  ┌──────────────────────────────────────────┐                       │
│  │  - IR Generation (LLVM IR / GCC GIMPLE)  │                       │
│  │  - Optimization passes (-O0, -O1, -O2,   │                       │
│  │    -O3, -Os, -Oz)                        │                       │
│  │  - Code generation                       │                       │
│  │  Output: Assembly (.s file)              │                       │
│  └──────────────────┬───────────────────────┘                       │
│                     │                                               │
│                     ▼  STAGE 4: ASSEMBLER (as)                      │
│  ┌──────────────────────────────────────────┐                       │
│  │  - Converts assembly to machine code     │                       │
│  │  Output: Object file (.o / .obj)         │                       │
│  └──────────────────┬───────────────────────┘                       │
│                     │                                               │
│                     ▼  STAGE 5: LINKER (ld)                         │
│  ┌──────────────────────────────────────────┐                       │
│  │  - Combines all .o files                 │                       │
│  │  - Resolves symbol references            │                       │
│  │  - Links standard libraries (libc++,     │                       │
│  │    libstdc++)                            │                       │
│  │  Output: Executable or Shared Library    │                       │
│  └──────────────────┬───────────────────────┘                       │
│                     │                                               │
│                     ▼                                               │
│              ┌─────────────┐                                        │
│              │ ./program   │  ← Final Executable                    │
│              └─────────────┘                                        │
└─────────────────────────────────────────────────────────────────────┘
```

### Seeing Each Stage Yourself

```bash
# Stage 1: Only run the preprocessor
g++ -E main.cpp -o main.i

# Stage 1+2+3: Compile to assembly only (no linking)
g++ -S main.cpp -o main.s

# Stage 1+2+3+4: Compile to object file only
g++ -c main.cpp -o main.o

# All stages: Full compilation + linking
g++ main.cpp -o main

# View the symbols in an object file
nm main.o

# View assembly generated
objdump -d main.o
```

---

## 1.3 Your First C++ Program — Dissected Completely

```cpp
// FILE: hello.cpp

// ① Preprocessor directive — tells the preprocessor to copy
//    the contents of the <iostream> header file here
#include <iostream>

// ② 'int main()' is the ENTRY POINT of every C++ program.
//    The OS calls this function to start your program.
//    'int' means it returns an integer exit code to the OS.
//    (0 = success, non-zero = error)
int main() {

    // ③ 'std' is the standard NAMESPACE.
    //    '::' is the scope resolution operator.
    //    'cout' is an object representing standard output (stdout).
    //    '<<' is the stream insertion operator.
    //    '\n' is a newline character (faster than std::endl).
    std::cout << "Hello, World!\n";

    // ④ Return 0 to signal success to the OS.
    //    Modern C++ allows omitting this — the compiler adds it.
    return 0;
}
```

### What Happens When You Run This?

```
┌────────────────────────────────────────────────────────────────┐
│                    RUNTIME EXECUTION                           │
│                                                                │
│  OS creates a process for your program                         │
│         │                                                      │
│         ▼                                                      │
│  C++ Runtime (CRT) initializes:                                │
│    - Sets up stack and heap                                    │
│    - Calls global object constructors                          │
│    - Initializes standard streams (cin, cout, cerr)            │
│         │                                                      │
│         ▼                                                      │
│  main() is called                                              │
│         │                                                      │
│         ▼                                                      │
│  std::cout << "Hello, World!\n"                                │
│    - Writes bytes to the stdout file descriptor (fd=1)         │
│    - OS kernel handles I/O                                     │
│         │                                                      │
│         ▼                                                      │
│  return 0                                                      │
│         │                                                      │
│         ▼                                                      │
│  C++ Runtime destructs global objects                          │
│  Process exits with code 0                                     │
└────────────────────────────────────────────────────────────────┘
```

---

## 1.4 Headers, Source Files, and the One-Definition Rule

### The Separation of Declaration and Definition

```
┌──────────────────────────────────────────────────────────────┐
│  HEADER FILE (.h / .hpp)          SOURCE FILE (.cpp)         │
│                                                              │
│  Contains DECLARATIONS:           Contains DEFINITIONS:      │
│  "This thing EXISTS"              "This is HOW it works"     │
│                                                              │
│  // math.hpp                      // math.cpp                │
│  int add(int a, int b);           #include "math.hpp"        │
│                                   int add(int a, int b) {    │
│  class Calculator {                   return a + b;          │
│  public:                          }                          │
│      int add(int a, int b);                                  │
│  };                               Calculator::add(int a,     │
│                                       int b) {               │
│                                       return a + b;          │
│                                   }                          │
└──────────────────────────────────────────────────────────────┘
```

### Why This Separation Exists

When you compile `main.cpp`, the compiler only processes one file at a time (one **translation unit**). The header tells the compiler "trust me, this function exists — it'll be resolved at link time."

```cpp
// math.hpp — DECLARATION (what)
#pragma once  // Include guard: prevents multiple inclusion

int add(int a, int b);
double multiply(double x, double y);

// math.cpp — DEFINITION (how)
#include "math.hpp"

int add(int a, int b) {
    return a + b;
}

double multiply(double x, double y) {
    return x * y;
}

// main.cpp — USER
#include <iostream>
#include "math.hpp"  // We need the declarations

int main() {
    std::cout << add(3, 4) << "\n";       // 7
    std::cout << multiply(2.5, 4.0) << "\n"; // 10.0
    return 0;
}
```

```bash
# Compile together:
g++ -std=c++23 main.cpp math.cpp -o program
```

### The One-Definition Rule (ODR)

> **Every entity (function, variable, type) must be defined exactly once across the entire program.**

```cpp
// BAD: Defining the same function in two .cpp files
// file_a.cpp:
int foo() { return 1; }

// file_b.cpp:
int foo() { return 2; }  // ← LINKER ERROR: multiple definition of 'foo'
```

**Exception:** `inline` functions, templates, and `constexpr` functions can appear in multiple translation units (they must be identical).

---

## 1.5 Include Guards and `#pragma once`

Every header file must be protected from being included multiple times:

```cpp
// Method 1: Traditional include guard
#ifndef MATH_HPP   // "if MATH_HPP is NOT defined..."
#define MATH_HPP   // "...define it now"

int add(int a, int b);

#endif // MATH_HPP

// Method 2: pragma once (modern, preferred, compiler-specific but universal)
#pragma once

int add(int a, int b);
```

**Why it matters:**
```
main.cpp includes foo.hpp
main.cpp includes bar.hpp
bar.hpp includes foo.hpp  ← foo.hpp included TWICE without guard!
```

Without guards, you'd get "redefinition" errors.

---

## 1.6 Namespaces

Namespaces prevent **name collisions** between code from different libraries.

```cpp
#include <iostream>

namespace physics {
    double gravity = 9.81;
    double force(double mass, double acceleration) {
        return mass * acceleration;
    }
}

namespace chemistry {
    double gravity = 6.674e-11;  // gravitational constant (different!)
    void react() { std::cout << "Reaction!\n"; }
}

int main() {
    // Fully qualified names — no ambiguity
    std::cout << physics::gravity << "\n";    // 9.81
    std::cout << chemistry::gravity << "\n";  // 6.674e-11
    physics::force(10.0, 2.0);               // 20.0

    // 'using' declaration — brings ONE name into scope
    using physics::force;
    force(10.0, 2.0);  // OK

    // 'using namespace' — brings ALL names into scope
    // WARNING: Can cause collisions! Avoid in headers!
    using namespace chemistry;
    react();  // OK, but now 'gravity' is ambiguous!

    return 0;
}
```

### The `std` Namespace

All standard library components live in `std::`. Never write `using namespace std;` in a header file — it pollutes every file that includes yours.

```cpp
// These are all equivalent:
std::cout << "hello\n";

// After 'using std::cout;'
cout << "hello\n";

// After 'using namespace std;' — BAD practice in headers
cout << "hello\n";
```

---

## 1.7 Comments and Code Style

```cpp
// Single-line comment — use for brief explanations

/*
 * Multi-line comment
 * Use for longer explanations or temporarily disabling code
 */

/**
 * Doxygen documentation comment
 * @param x  The first value
 * @param y  The second value
 * @return   The sum of x and y
 */
int add(int x, int y);
```

---

## 1.8 The Abstract Machine

C++ is defined in terms of an **abstract machine** — a hypothetical computer that executes C++ programs. The actual hardware can do anything as long as the **observable behavior** matches.

This is why:
- The compiler can **reorder your code**
- The compiler can **eliminate variables** that you never use
- The compiler can **inline functions** (and not call them at all)

```cpp
int x = 5;
int y = x + 3;    // Compiler might compute this at compile time: y = 8
int z = y * 2;    // z = 16, all computed at compile time
                  // No runtime work done at all!
std::cout << z;   // The only "observable" operation is this output
```

This is called the **"as-if" rule**: the compiler can do anything as long as the program **behaves as if** it ran the abstract machine exactly.

---

## 1.9 Undefined Behavior (UB) — The Enemy

C++ has a concept called **undefined behavior** (UB). When your code triggers UB:

> **The standard makes NO guarantees. The compiler can do ANYTHING — including appearing to work correctly, crashing, or corrupting data silently.**

```cpp
int arr[5] = {1, 2, 3, 4, 5};
int x = arr[10];   // UB: reading out of bounds!
                   // Might read garbage, might crash, might "work"

int i = INT_MAX;
i = i + 1;        // UB: signed integer overflow!

int* p = nullptr;
*p = 42;          // UB: null pointer dereference! (will crash)

int a = 0;
int b = 1 / a;    // UB: division by zero!
```

**Why does UB exist?** Because defining behavior for all these cases would make C++ slower. The compiler assumes UB never happens, which allows aggressive optimizations.

We'll see this in depth in Module 11. For now: **avoid UB at all costs**.

---

## Module 01 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| Compilation Pipeline | Source → Preprocessor → Compiler → Assembler → Linker → Executable |
| Translation Unit | One `.cpp` file + all its `#include`d headers |
| Declaration vs Definition | Declaration: "it exists". Definition: "here's how" |
| ODR | Define each thing exactly once |
| Include Guards | Prevent double-inclusion of headers |
| Namespaces | Prevent name collisions |
| Abstract Machine | C++ semantics are defined on a fictional machine; compiler exploits this |
| UB | Undefined behavior = anything can happen |

---

## ➡️ Next: [Module 02: Core Types, Variables & Memory Layout](../module_02_core_types/02_core_types.md)
