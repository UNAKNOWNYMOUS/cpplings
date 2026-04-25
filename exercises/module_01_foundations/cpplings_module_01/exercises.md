# Cpplings — Module 01 Exercises

## Exercise 01 — Hello, C++ Runtime

**Goal:** Understand the minimal C++ program and `main`.

File: `starter/ex01_hello.cpp`

**Task:** Make the program compile and return success.

Expected output:

```text
Cpplings Module 01
```

---

## Exercise 02 — `std::cout`, `::`, and `\n`

**Goal:** Practice standard namespace usage and output.

File: `starter/ex02_cout.cpp`

**Rules:**

Do not use:

```cpp
using namespace std;
```

**Task:** Make the program compile using proper standard-library qualification.

---

## Exercise 03 — Preprocessor Stage

**Goal:** Learn what the preprocessor does.

File: `starter/ex03_preprocessor.cpp`

Run only the preprocessor:

```bash
g++ -E starter/ex03_preprocessor.cpp -o ex03_preprocessor.i
```

Then answer:

1. Is `#include <iostream>` still visible in the `.i` file?
2. Is `COURSE` still visible as a macro name inside `main`?
3. Why is the `.i` file much larger than the original `.cpp` file?

---

## Exercise 04 — Compilation Pipeline Commands

**Goal:** Match each compiler flag to the correct pipeline stage.

File: `starter/ex04_pipeline.cpp`

Fill in the correct commands:

```bash
# TODO 1: Generate preprocessed file ex04_pipeline.i

# TODO 2: Generate assembly file ex04_pipeline.s

# TODO 3: Generate object file ex04_pipeline.o

# TODO 4: Generate final executable ex04_pipeline
```

---

## Exercise 05 — Translation Unit Thinking

**Goal:** Understand what a translation unit is.

Files:

```text
starter/ex05_translation_unit/
├── main.cpp
├── message.cpp
└── message.hpp
```

Task 1:

```bash
g++ -std=c++23 starter/ex05_translation_unit/main.cpp -o app
```

Task 2: Explain why this fails.

Task 3: Write the correct compile command.

---

## Exercise 06 — Declaration vs Definition

**Goal:** Separate “this exists” from “this is how it works.”

Files:

```text
starter/ex06_declaration_definition/
├── main.cpp
├── math.cpp
└── math.hpp
```

**Task:** Complete the declaration and definition.

Expected output:

```text
30
```

---

## Exercise 07 — Linker Error: Undefined Reference

**Goal:** Identify when the compiler is satisfied but the linker is not.

Files:

```text
starter/ex07_undefined_reference/
├── main.cpp
└── operations.hpp
```

Compile:

```bash
g++ -std=c++23 starter/ex07_undefined_reference/main.cpp -o app
```

**Task:** Explain the likely error and fix the project by adding the missing file.

---

## Exercise 08 — ODR Violation

**Goal:** Understand the One-Definition Rule.

Files:

```text
starter/ex08_odr_violation/
├── a.cpp
├── b.cpp
└── main.cpp
```

Compile:

```bash
g++ -std=c++23 starter/ex08_odr_violation/main.cpp starter/ex08_odr_violation/a.cpp starter/ex08_odr_violation/b.cpp -o app
```

**Task:** Explain why this fails and fix the design.

---

## Exercise 09 — Include Guard Repair

**Goal:** Prevent double inclusion.

File: `starter/ex09_include_guard/config.hpp`

**Task:** Rewrite this header using a traditional include guard.

Do not use `#pragma once`.

---

## Exercise 10 — `#pragma once`

**Goal:** Use the modern include guard style.

File: `starter/ex10_pragma_once/user.hpp`

**Task:** Rewrite this header using `#pragma once`.

---

## Exercise 11 — Namespace Collision

**Goal:** Use namespaces to avoid name conflicts.

File: `starter/ex11_namespace_collision.cpp`

**Task:** Put the two `gravity` variables into separate namespaces:

```cpp
namespace physics { ... }
namespace chemistry { ... }
```

Then print both values explicitly.

---

## Exercise 12 — Bad Header Practice

**Goal:** Understand why `using namespace std;` does not belong in headers.

Files:

```text
starter/ex12_bad_header/
├── printer.hpp
└── printer.cpp
```

**Task:** Rewrite the header so it does not pollute every file that includes it.

Then implement `print_message()` so it prints:

```text
safe header
```

---

## Exercise 13 — Abstract Machine and As-If Rule

**Goal:** Reason about compiler optimization.

File: `starter/ex13_as_if.cpp`

Compile to assembly twice:

```bash
g++ -std=c++23 -O0 -S starter/ex13_as_if.cpp -o ex13_O0.s
g++ -std=c++23 -O2 -S starter/ex13_as_if.cpp -o ex13_O2.s
```

Answer:

1. What is the output of the program?
2. Why might the optimized assembly not show every variable?
3. Is the compiler allowed to compute `z` at compile time?
4. What rule allows this?

---

## Exercise 14 — Undefined Behavior Hunt

**Goal:** Identify undefined behavior.

File: `starter/ex14_ub_hunt.cpp`

**Task:** Identify each UB case and explain why.

---

## Exercise 15 — Mini Cpplings Project

**Goal:** Combine headers, source files, namespaces, declarations, definitions, and linking.

Files:

```text
starter/ex15_project/
├── include/
│   └── cpplings_math.hpp
├── src/
│   ├── cpplings_math.cpp
│   └── main.cpp
```

Compile:

```bash
g++ -std=c++23 -Wall -Wextra -pedantic -Istarter/ex15_project/include starter/ex15_project/src/main.cpp starter/ex15_project/src/cpplings_math.cpp -o cpplings_app
```

Expected output:

```text
5
25
```
