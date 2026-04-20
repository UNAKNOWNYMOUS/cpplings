# Module 01 Exercises — Foundations: How C++ Works Under the Hood

Based on your Module 01 notes on C++ foundations. Source notes: fileciteturn0file0

## Part A — Quick Recall

### 1. Define each term in 1–2 sentences.
a. compiled language  
b. statically typed  
c. translation unit  
d. declaration  
e. definition  
f. linker  
g. namespace  
h. undefined behavior

---

### 2. Fill in the compilation pipeline in order.
Source file → ________ → ________ → ________ → ________ → Executable

---

### 3. Match the stage to what it does.

1. Preprocessor  
2. Compiler front-end  
3. Compiler back-end  
4. Assembler  
5. Linker  

A. Resolves symbol references across object files  
B. Produces machine code object files from assembly  
C. Handles tokens, parsing, semantic checks, template instantiation  
D. Expands macros and includes, strips comments  
E. Optimizes and generates assembly

---

### 4. What file is typically produced by each command?

a. `g++ -E main.cpp -o main.i`  
b. `g++ -S main.cpp -o main.s`  
c. `g++ -c main.cpp -o main.o`  
d. `g++ main.cpp -o main`

---

### 5. What is the purpose of `main()` in C++?

---

## Part B — Code Reading

### 6. Read the program and answer the questions.

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, World!\n";
    return 0;
}
```

a. What does `#include <iostream>` do?  
b. What namespace is `cout` in?  
c. What does `::` mean here?  
d. Why is `"\n"` usually preferred over `std::endl` when you only want a newline?  
e. What does `return 0;` communicate?

---

### 7. For each line below, say whether it happens at preprocessing time, compile time, link time, or runtime.

a. `#include <iostream>`  
b. Type-checking a function call  
c. Resolving a call to `add(int, int)` defined in another `.cpp` file  
d. `std::cout << "Hi\n";` actually printing to the terminal  
e. Expanding a macro

---

### 8. What is wrong with this setup?

```cpp
// math.hpp
#pragma once
int add(int a, int b) {
    return a + b;
}
```

```cpp
// file1.cpp
#include "math.hpp"
```

```cpp
// file2.cpp
#include "math.hpp"
```

Assume both `.cpp` files are compiled into the same program.

---

### 9. What is wrong with this setup?

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

```cpp
// other.cpp
int add(int a, int b) {
    return a - b;
}
```

---

### 10. Read the code and explain the ambiguity.

```cpp
#include <iostream>

namespace physics {
    double gravity = 9.81;
}

namespace chemistry {
    double gravity = 6.674e-11;
}

int main() {
    using namespace physics;
    using namespace chemistry;
    std::cout << gravity << "\n";
}
```

---

## Part C — Declarations, Definitions, and ODR

### 11. Label each item as either a declaration, a definition, or both.

a. `int add(int a, int b);`  
b.
```cpp
int add(int a, int b) {
    return a + b;
}
```
c. `class Calculator;`  
d.
```cpp
class Calculator {
public:
    int add(int a, int b);
};
```
e. `extern int x;`  
f. `int x = 5;`

---

### 12. Why can a `.cpp` file call a function that is not defined inside that same file?

---

### 13. Explain the One-Definition Rule in your own words.

---

### 14. Why are templates, `inline` functions, and `constexpr` functions special with respect to multiple definitions?

---

### 15. Suppose `main.cpp` includes `math.hpp`, which declares `int add(int, int);`, but you compile only:

```bash
g++ main.cpp -o program
```

and do not compile `math.cpp`.

What kind of problem will you get, and at what stage?

---

## Part D — Include Guards and Headers

### 16. What problem do include guards solve?

---

### 17. Write a traditional include guard for a header named `logger.hpp`.

---

### 18. Rewrite this header using `#pragma once`.

```cpp
#ifndef LOGGER_HPP
#define LOGGER_HPP

void log_message();

#endif
```

---

### 19. Why is `using namespace std;` especially bad inside a header file?

---

### 20. A header is included twice through different paths:

- `main.cpp` includes `foo.hpp`
- `main.cpp` includes `bar.hpp`
- `bar.hpp` includes `foo.hpp`

What happens:  
a. without include guards?  
b. with include guards?

---

## Part E — Commands and Tooling

### 21. What does each command do?

a. `g++ -E main.cpp -o main.i`  
b. `g++ -S main.cpp -o main.s`  
c. `g++ -c main.cpp -o main.o`  
d. `nm main.o`  
e. `objdump -d main.o`

---

### 22. You want to inspect the assembly generated from a C++ file, but you do not want to link. Which command should you use?

---

### 23. You want to compile several files separately and link later. Which intermediate file type do you need?

---

## Part F — Abstract Machine and UB

### 24. What is the “abstract machine” in C++?

---

### 25. State the “as-if” rule in plain English.

---

### 26. For each snippet, say whether it is well-defined or UB, and explain why.

a.
```cpp
int arr[5] = {1,2,3,4,5};
int x = arr[2];
```

b.
```cpp
int arr[5] = {1,2,3,4,5};
int x = arr[10];
```

c.
```cpp
int* p = nullptr;
*p = 42;
```

d.
```cpp
int a = 10;
int b = 2;
int c = a / b;
```

e.
```cpp
int a = 1;
int b = 0;
int c = a / b;
```

f.
```cpp
int i = INT_MAX;
i = i + 1;
```

---

### 27. Why does C++ allow UB instead of defining behavior for everything?

---

### 28. Why can code with UB sometimes appear to “work”?

---

## Part G — Find the Mistake

### 29. Find the mistake.

```cpp
// a.cpp
int foo() { return 1; }

// b.cpp
int foo() { return 1; }
```

---

### 30. Find the mistake.

```cpp
// utils.hpp
#pragma once
using namespace std;

int square(int x);
```

---

### 31. Find the mistake.

```cpp
// main.cpp
#include "math.hpp"
#include <iostream>

int main() {
    std::cout << multiply(2, 3) << "\n";
}
```

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

double multiply(double x, double y) {
    return x * y;
}
```

---

### 32. Find the mistake.

```cpp
#ifndef CONFIG_HPP
int version();
#endif
```

---

## Part H — Short Written Mastery Questions

### 33. Why is separating declarations into headers and definitions into source files useful for large projects?

---

### 34. What does the linker do that the compiler alone cannot do?

---

### 35. Explain why namespaces are important when many libraries are used in the same program.

---

### 36. Why is C++ considered a systems language?

---

### 37. What does it mean that C++ offers “zero-cost abstractions”?

---

## Part I — Practical Design Exercises

### 38. Create a correct 3-file setup for a math utility with:
- a header
- a source file
- a `main.cpp`

The utility should provide:
- `int add(int, int);`
- `double multiply(double, double);`

Write the contents of all three files.

---

### 39. Write a small program that demonstrates namespace collisions safely by using two namespaces that both contain a variable named `value`.

---

### 40. Write one example of:
- a declaration only
- a definition
- an include guard
- a `#pragma once` header
- a piece of UB

---

# Answer Key

## 1.
a. A compiled language is translated into machine code before execution.  
b. A statically typed language checks types at compile time rather than mainly at runtime.  
c. A translation unit is one `.cpp` file after all of its included headers have been expanded by the preprocessor.  
d. A declaration tells the compiler something exists.  
e. A definition provides the actual implementation or storage.  
f. The linker combines object files and resolves symbol references.  
g. A namespace groups names to avoid collisions.  
h. Undefined behavior means the C++ standard gives no guarantees about what happens.

---

## 2.
Source file → Preprocessor → Compiler → Assembler → Linker → Executable

A more detailed breakdown from the notes is:
Source → Preprocessor → Compiler front-end → Compiler back-end → Assembler → Linker → Executable

---

## 3.
1-D  
2-C  
3-E  
4-B  
5-A

---

## 4.
a. Preprocessed file (`main.i`)  
b. Assembly file (`main.s`)  
c. Object file (`main.o`)  
d. Final executable (`main`)

---

## 5.
`main()` is the program entry point. The OS starts the program by eventually invoking it through the runtime setup.

---

## 6.
a. It tells the preprocessor to include the contents of the `<iostream>` header.  
b. `std`  
c. Scope resolution operator  
d. `"\n"` usually just inserts a newline, while `std::endl` also flushes the stream, which can be slower.  
e. It signals successful program termination to the OS.

---

## 7.
a. Preprocessing time  
b. Compile time  
c. Link time  
d. Runtime  
e. Preprocessing time

---

## 8.
The function is being defined in the header. If that header is included in multiple translation units, the program may violate the ODR and produce multiple-definition linker errors. It should usually be declared in the header and defined in one `.cpp`, unless it is `inline`.

---

## 9.
`add` is defined twice across the program: once in `math.cpp` and once in `other.cpp`. That violates the One-Definition Rule and causes a linker error.

---

## 10.
Both namespaces inject a name called `gravity` into the same scope because of `using namespace physics;` and `using namespace chemistry;`. Therefore `gravity` becomes ambiguous.

---

## 11.
a. Declaration  
b. Definition  
c. Declaration  
d. Definition of the class type, and declaration of the member function  
e. Declaration  
f. Definition

---

## 12.
Because the header provides the declaration, which tells the compiler the function exists. The actual definition is found later by the linker.

---

## 13.
Each entity that requires a definition in the whole program must have exactly one definition program-wide, unless it falls into a special allowed category like identical inline/template cases.

---

## 14.
Because the language allows them to appear in multiple translation units as long as the definitions are identical. This is necessary especially for templates and common header-based code.

---

## 15.
You will typically get a linker error for an unresolved or undefined reference to `add`. The compiler was satisfied by the declaration, but the linker could not find the definition.

---

## 16.
They prevent multiple inclusion of the same header within one translation unit, which would otherwise cause redefinition problems.

---

## 17.
```cpp
#ifndef LOGGER_HPP
#define LOGGER_HPP

// declarations here

#endif
```

---

## 18.
```cpp
#pragma once

void log_message();
```

---

## 19.
Because every file that includes the header also gets all names from `std` pulled into scope, which pollutes the global namespace and increases the chance of collisions and ambiguity.

---

## 20.
a. Without include guards, the contents of `foo.hpp` may be processed multiple times, often causing redefinition errors.  
b. With include guards, the header’s contents are only processed once per translation unit.

---

## 21.
a. Runs only preprocessing  
b. Compiles to assembly only  
c. Compiles to object file only  
d. Displays symbols in the object file  
e. Disassembles the object file

---

## 22.
```bash
g++ -S main.cpp -o main.s
```

---

## 23.
Object files such as `.o`

---

## 24.
The abstract machine is the hypothetical machine used by the C++ standard to define program behavior. Real compilers and hardware may do things differently internally as long as the observable behavior matches.

---

## 25.
The compiler may transform your program in any way it wants as long as the final observable behavior is the same as if the program had executed normally according to the language rules.

---

## 26.
a. Well-defined — valid index in bounds  
b. UB — out-of-bounds access  
c. UB — dereferencing null pointer  
d. Well-defined — valid division  
e. UB — division by zero  
f. UB — signed integer overflow

---

## 27.
Because leaving those cases undefined allows the compiler to generate faster and more optimized code without needing extra checks for every dangerous situation.

---

## 28.
Because UB means there are no guarantees. Sometimes the bad action does not immediately trigger a visible crash, so the program can seem fine even though it is incorrect.

---

## 29.
`foo()` is defined twice in two different translation units. That violates the ODR.

---

## 30.
`using namespace std;` should not be placed in a header because it pollutes every including file.

---

## 31.
`multiply` is not declared in the header, so `main.cpp` has no declaration for it. `math.hpp` must declare it.

Correct header:
```cpp
#pragma once
int add(int a, int b);
double multiply(double x, double y);
```

---

## 32.
The include guard is incomplete. It never defines the macro.

Correct version:
```cpp
#ifndef CONFIG_HPP
#define CONFIG_HPP

int version();

#endif
```

---

## 33.
It lets many source files share interfaces without exposing implementation details everywhere, supports separate compilation, improves organization, and helps large codebases scale.

---

## 34.
The compiler translates each translation unit separately, but it does not combine them. The linker combines object files and resolves symbol references across files and libraries.

---

## 35.
Different libraries may use the same names for functions, classes, or variables. Namespaces prevent these names from colliding.

---

## 36.
Because it gives low-level control over memory, object layout, performance, and hardware-near behavior, making it useful for operating systems, engines, embedded systems, and similar software.

---

## 37.
It means abstractions do not impose extra runtime cost unless you actually use features that require it. You pay only for what you use.

---

## 38.
One correct answer:

```cpp
// math.hpp
#pragma once

int add(int a, int b);
double multiply(double x, double y);
```

```cpp
// math.cpp
#include "math.hpp"

int add(int a, int b) {
    return a + b;
}

double multiply(double x, double y) {
    return x * y;
}
```

```cpp
// main.cpp
#include <iostream>
#include "math.hpp"

int main() {
    std::cout << add(3, 4) << "\n";
    std::cout << multiply(2.5, 4.0) << "\n";
    return 0;
}
```

---

## 39.
One correct answer:

```cpp
#include <iostream>

namespace first {
    int value = 10;
}

namespace second {
    int value = 20;
}

int main() {
    std::cout << first::value << "\n";
    std::cout << second::value << "\n";
    return 0;
}
```

---

## 40.
One possible answer:

Declaration only:
```cpp
int add(int a, int b);
```

Definition:
```cpp
int add(int a, int b) {
    return a + b;
}
```

Include guard:
```cpp
#ifndef TEST_HPP
#define TEST_HPP

int f();

#endif
```

`#pragma once` header:
```cpp
#pragma once
int f();
```

UB:
```cpp
int* p = nullptr;
*p = 5;
```
