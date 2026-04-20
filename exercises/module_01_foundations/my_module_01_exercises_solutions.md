# Module 01 Exercises — Foundations

Built from your Module 01 notes on the compilation pipeline, translation units, declarations vs definitions, ODR, include guards, namespaces, the abstract machine, and undefined behavior.

---

## Instructions
Write your answers in the space provided under each question. Try to answer from memory first, then check the answer key at the end.

---

## Questions

### 1. Define C++ using four descriptors from the notes. What do those descriptors mean in practice?
**Your answer:**
1. Compiled - All source is translated to machine code before execution.
2. Statically typed - Types are checked at compile time.
3. Multi-paradigm - Supports OOP, procedural, generics, etc
4. Systems-level - It gives you low-level control over memory + hardware.



---

### 2. Write the five major stages of the C++ compilation pipeline in order, starting from source code and ending at an executable.
**Your answer:** Source code -> preprocessor -> compiler front end -> compiler back end -> assembler -> linker -> executable.



---

### 3. What does the preprocessor do? Name at least three specific jobs it performs.
**Your answer:** The preprocessor takes the source code (.cpp) and replaces preprocessor directives (`#include`), expands macros, resolves conditional preprocessor commands `ifndef`, etc, and removes comments.



---

### 4. What is produced after the assembler stage?
**Your answer:** An object file (.o files).



---

### 5. What is the job of the linker?
**Your answer:** The linker takes object files combines them together and resolves symbol references in the process.



---

### 6. Explain the difference between a declaration and a definition. Give one example of each.
**Your answer:**
Declaration - "This thing exists!"
```cpp
// math.hpp
int add(int x, int y);
```
Definition - "Here's how!"
```cpp
// math.cpp
int add(int x, int y) {
  return x + y;
}
```



---

### 7. What is a translation unit?
**Your answer:** A translation unit is file that the preprocessor creates.



---

### 8. Why can `main.cpp` call a function declared in `math.hpp` even if the function body is in `math.cpp`?
**Your answer:** The header file says to the compiler "trust me this exists" and will be checked at link time.



---

### 9. What is the One-Definition Rule? What kind of error can happen if you violate it?
**Your answer:** Every entity can only be defined once. If this is violated this will trigger an ambiguity to the linker -> resulting in a linker error.



---

### 10. Why do include guards or `#pragma once` exist? What problem do they prevent?
**Your answer:** Prevents multiple inclusion of the same header file -> which will cause a linker error stating "redefinition error".



---

### 11. In your own words, what problem do namespaces solve?
**Your answer:** Namespaces resolve naming collisions which can be caused when linking with libraries.



---

### 12. What is the difference between the following?
- `physics::force`
- `using physics::force;`
- `using namespace physics;`

**Your answer:**
- `physics::force` - fully qualified name
- `using physics::force;` - Brings only this name into the current scope.
- `using namespace physics;` - Brings all names from this name space into the current scope.



---

### 13. Why is `using namespace std;` especially bad in a header file?
**Your answer:** It pollutes all files including your own that include that file. It can cause name collisions.



---

### 14. Explain the “as-if” rule in simple words. Why is the compiler allowed to reorder or remove some operations?
**Your answer:** The compiler is allowed to reorder or remove some operations as long as the semantics of the code/ the code does exactly as if it were to run without any changes.



---

### 15. For each line below, say whether it is UB or not. If it is UB, state why.

```cpp
int arr[5] = {1,2,3,4,5};
int a = arr[2];

int i = INT_MAX;
i = i + 1;

int* p = nullptr;
// *p = 42;

int x = 10 / 2;
```

**Your answer:**

```cpp
int arr[5] = {1,2,3,4,5};
int a = arr[2]; // Defined

int i = INT_MAX;
i = i + 1; // UB: integer overflow

int* p = nullptr;
// *p = 42; // Fine as is because commented out but if not commented out it is UB because dereferencing a nullptr is UB.

int x = 10 / 2; // Defined
```


---

# Answer Key

## 1.
C++ is:
- **Compiled**: source code is translated to machine code before running.
- **Statically typed**: types are checked at compile time.
- **Multi-paradigm**: it supports procedural, object-oriented, generic, and more.
- **Systems-level**: it gives low-level control over memory and hardware.

## 2.
Order:
1. Preprocessor
2. Compiler front-end / compiler
3. Compiler back-end / code generation
4. Assembler
5. Linker

You can also summarize it as:

**Source → Preprocessor → Compiler → Assembler → Linker → Executable**

## 3.
The preprocessor:
- expands `#include`
- expands `#define` macros
- processes conditional compilation like `#ifdef` / `#ifndef`
- strips comments

## 4.
The assembler produces an **object file** such as `.o` or `.obj`.

## 5.
The linker:
- combines object files
- resolves symbol references
- links libraries
- produces the final executable or shared library

## 6.
A **declaration** says something exists.  
A **definition** provides the actual implementation or storage.

Example:
```cpp
int add(int a, int b);   // declaration
```

```cpp
int add(int a, int b) { return a + b; }   // definition
```

## 7.
A translation unit is:

**one `.cpp` file plus everything copied into it through `#include` after preprocessing**

## 8.
Because the header gives the compiler the function’s declaration, so the compiler knows the function exists and how to type-check calls to it. The actual function body is found later by the linker in `math.cpp`.

## 9.
The One-Definition Rule says each entity in the program must be defined exactly once across the whole program, except special cases like identical `inline` functions, templates, and `constexpr` functions.

If you violate it, you often get a **linker error**, such as a multiple-definition error.

## 10.
Include guards and `#pragma once` prevent the same header from being included multiple times in one translation unit.

They prevent **redefinition errors**.

## 11.
Namespaces prevent **name collisions**. They let different libraries or modules use the same names without conflicting.

Example:
```cpp
physics::gravity
chemistry::gravity
```

## 12.
- `physics::force`  
  Uses the fully qualified name directly from the namespace.

- `using physics::force;`  
  Brings only that one name into the current scope.

- `using namespace physics;`  
  Brings all names from `physics` into the current scope, which increases the chance of collisions or ambiguity.

## 13.
Because a header is included by other files. If it says `using namespace std;`, it pollutes the scope of every file that includes it, which can create name collisions and ambiguity far beyond that header.

## 14.
The “as-if” rule means the compiler can change, reorder, remove, or optimize code however it wants **as long as the observable behavior stays the same**.

That is why the compiler can compute things at compile time, inline functions, or remove unused work.

## 15.
```cpp
int arr[5] = {1,2,3,4,5};
int a = arr[2];
```
**Not UB.** `arr[2]` is in bounds.

```cpp
int i = INT_MAX;
i = i + 1;
```
**UB.** Signed integer overflow.

```cpp
int* p = nullptr;
// *p = 42;
```
As written, **not UB**, because the dereference is commented out.  
If `*p = 42;` were executed, it would be **UB** because it dereferences a null pointer.

```cpp
int x = 10 / 2;
```
**Not UB.** This is valid division.
