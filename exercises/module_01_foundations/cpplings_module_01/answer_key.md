# Cpplings — Module 01 Answer Key

## Exercise 01

```cpp
#include <iostream>

int main() {
    std::cout << "Cpplings Module 01\n";
    return 0;
}
```

`return 0;` tells the operating system the program finished successfully.

---

## Exercise 02

```cpp
#include <iostream>

int main() {
    std::cout << "Hello from cout" << '\n';
    return 0;
}
```

`cout` lives in the `std` namespace. Prefer `'\n'` over `std::endl` unless you specifically need flushing.

---

## Exercise 03

1. `#include <iostream>` is no longer present as a simple include directive. The preprocessor expands it.
2. `COURSE` is replaced with `"Cpplings"`.
3. The file is larger because the contents of included headers are copied into the translation unit.

---

## Exercise 04

```bash
g++ -E ex04_pipeline.cpp -o ex04_pipeline.i
g++ -S ex04_pipeline.cpp -o ex04_pipeline.s
g++ -c ex04_pipeline.cpp -o ex04_pipeline.o
g++ ex04_pipeline.cpp -o ex04_pipeline
```

Meanings:

```text
-E = preprocess only
-S = compile to assembly
-c = compile/assemble to object file, but do not link
no special flag = full compile and link
```

---

## Exercise 05

Compiling only `main.cpp` fails because `main.cpp` sees the declaration of `message()`, but the definition is in `message.cpp`.

Correct command:

```bash
g++ -std=c++23 main.cpp message.cpp -o app
```

The compiler handles each `.cpp` as a separate translation unit. The linker combines them later.

---

## Exercise 06

```cpp
// math.hpp
#pragma once

int add(int a, int b);
```

```cpp
// math.cpp
#include "math.hpp"

int add(int a, int b) {
    return a + b;
}
```

Compile:

```bash
g++ -std=c++23 main.cpp math.cpp -o app
```

---

## Exercise 07

The declaration exists:

```cpp
int multiply(int a, int b);
```

But no definition is linked, so the linker reports something like:

```text
undefined reference to `multiply(int, int)'
```

Fix:

```cpp
// operations.cpp
#include "operations.hpp"

int multiply(int a, int b) {
    return a * b;
}
```

Compile:

```bash
g++ -std=c++23 main.cpp operations.cpp -o app
```

---

## Exercise 08

This violates the One-Definition Rule because `version()` is defined in both `a.cpp` and `b.cpp`.

Fix by keeping only one external definition.

Example:

```cpp
// version.hpp
#pragma once

int version();
```

```cpp
// version.cpp
#include "version.hpp"

int version() {
    return 1;
}
```

```cpp
// main.cpp
#include <iostream>
#include "version.hpp"

int main() {
    std::cout << version() << "\n";
    return 0;
}
```

Compile:

```bash
g++ -std=c++23 main.cpp version.cpp -o app
```

---

## Exercise 09

```cpp
#ifndef CONFIG_HPP
#define CONFIG_HPP

struct Config {
    int width;
    int height;
};

#endif // CONFIG_HPP
```

The guard prevents the same header contents from being processed multiple times in one translation unit.

---

## Exercise 10

```cpp
#pragma once

struct User {
    const char* name;
    int id;
};
```

`#pragma once` tells the compiler to include the header only once per translation unit.

---

## Exercise 11

```cpp
#include <iostream>

namespace physics {
    double gravity = 9.81;
}

namespace chemistry {
    double gravity = 6.674e-11;
}

int main() {
    std::cout << physics::gravity << "\n";
    std::cout << chemistry::gravity << "\n";
    return 0;
}
```

The fully qualified names remove ambiguity.

---

## Exercise 12

Header:

```cpp
// printer.hpp
#pragma once

void print_message();
```

Implementation:

```cpp
// printer.cpp
#include <iostream>
#include "printer.hpp"

void print_message() {
    std::cout << "safe header\n";
}
```

Optional `main.cpp`:

```cpp
#include "printer.hpp"

int main() {
    print_message();
    return 0;
}
```

Compile:

```bash
g++ -std=c++23 main.cpp printer.cpp -o app
```

The header should not contain `using namespace std;` because every file that includes it would inherit that namespace pollution.

---

## Exercise 13

Output:

```text
16
```

The compiler may remove `x`, `y`, and even `z` as actual runtime variables if it can prove the final observable behavior is unchanged.

Yes, the compiler is allowed to compute `z` at compile time.

The rule is the **as-if rule**: the compiler can transform the program however it wants as long as the observable behavior is as if the original abstract-machine program had run.

---

## Exercise 14

```cpp
int a = arr[2];      // Safe
int b = arr[10];     // UB
int y = x + 1;       // UB if x == INT_MAX
*p = 42;             // UB if uncommented
int z = 10 / denominator; // UB because denominator is 0
```

Explanation:

```text
Case 1: Safe. arr[2] is within bounds.
Case 2: UB. arr[10] reads out of bounds.
Case 3: UB. Signed integer overflow is undefined behavior.
Case 4: UB. Dereferencing nullptr is undefined behavior.
Case 5: UB. Integer division by zero is undefined behavior.
```

---

## Exercise 15

```cpp
// include/cpplings_math.hpp
#pragma once

namespace cpplings {
    int add(int a, int b);
    int square(int x);
}
```

```cpp
// src/cpplings_math.cpp
#include "cpplings_math.hpp"

namespace cpplings {
    int add(int a, int b) {
        return a + b;
    }

    int square(int x) {
        return x * x;
    }
}
```

```cpp
// src/main.cpp
#include <iostream>
#include "cpplings_math.hpp"

int main() {
    std::cout << cpplings::add(2, 3) << "\n";
    std::cout << cpplings::square(5) << "\n";
    return 0;
}
```

Compile:

```bash
g++ -std=c++23 -Wall -Wextra -pedantic -Iinclude src/main.cpp src/cpplings_math.cpp -o cpplings_app
```

Run:

```bash
./cpplings_app
```

Expected output:

```text
5
25
```
