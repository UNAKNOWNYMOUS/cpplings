# My Answers:
**E00.1** — Explain in one sentence what the linker does differently from the compiler.
ANSWER: The linker combines separately-compiled object files by resolving symbolic references (function/variable names) to actual memory addresses.

**E00.2** — Given this code, identify what segment each variable lives in:
```cpp
int a = 10;           // ?
int b;                // ?
void foo() {
    int c = 5;        // ?
    int* d = new int; // d lives ?, *d lives ?
}
```
ANSWER:
1. Data Segment
2. BSS Segment
3. stack
4. stack, heap

**E00.3** — Why does this cause a linker error, not a compiler error?
```cpp
// file1.cpp
void bar();         // declaration
int main() { bar(); }

// file2.cpp  ← you forgot to compile this file
void bar() { }      // definition
```
1. This is because the linker links together separately-compiled object files and it's missing the definition which will lead to an unknown reference.
