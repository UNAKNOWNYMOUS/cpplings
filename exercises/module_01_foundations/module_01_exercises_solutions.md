# Module 01 Exercises — Foundations

Built from your Module 01 notes on the compilation pipeline, translation units, declarations vs definitions, ODR, include guards, namespaces, the abstract machine, and undefined behavior.

---

## Instructions
Write your answers in the space provided under each question. Try to answer from memory first, then check the answer key at the end.

---

## Questions

### 1. Define C++ using four descriptors from the notes. What do those descriptors mean in practice?
**Your answer:** General-purpose, statically-typed, systems-levl programming language.



---

### 2. Write the five major stages of the C++ compilation pipeline in order, starting from source code and ending at an executable.
**Your answer:** preprocessor -> compiler front-end -> compiler back-end -> assembler -> linker.



---

### 3. What does the preprocessor do? Name at least three specific jobs it performs.
**Your answer:** Replaces the preprocessor directives, expands macros, removes comments.



---

### 4. What is produced after the assembler stage?
**Your answer:** Object file *.o* files.



---

### 5. What is the job of the linker?
**Your answer:** Combines object files and resolves symbol references.



---

### 6. Explain the difference between a declaration and a definition. Give one example of each.
**Your answer:** Declaration--"Hey this thing exists" Definition--"Hey here's how"



---

### 7. What is a translation unit?
**Your answer:** A translation unit is a file after the processor has done its work on it.



---

### 8. Why can `main.cpp` call a function declared in `math.hpp` even if the function body is in `math.cpp`?
**Your answer:** Because the compiler goes "Trust me this thing exists" and it will later be checked at link-time.



---

### 9. What is the One-Definition Rule? What kind of error can happen if you violate it?
**Your answer:** Only define things once, it can cause a redefinition issue with or the compiler doesn't know which one to call because the call is amigous.



---

### 10. Why do include guards or `#pragma once` exist? What problem do they prevent?
**Your answer:** They prevent multiple inclusion of the same header file and prevent redefintion issues.



---

### 11. In your own words, what problem do namespaces solve?
**Your answer:** Naming problems with other libraries.



---

### 12. What is the difference between the following?
- `physics::force`
- `using physics::force;`
- `using namespace physics;`

**Your answer:** fully-qualified name, brings only that element into scope, brings all programming elements into scope.



---

### 13. Why is `using namespace std;` especially bad in a header file?
**Your answer:** Pollutes files and can cause issues with names.



---

### 14. Explain the “as-if” rule in simple words. Why is the compiler allowed to reorder or remove some operations?
**Your answer:** It can do anything under the software for optimization purposes as long as the semantics stay the same.



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
1. Not
2. Yes signed integer overflow.
3. yes UB it will crash
4. Not



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
