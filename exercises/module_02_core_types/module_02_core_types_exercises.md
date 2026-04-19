# Module 02 Exercises — Core Types, Variables & Memory Layout

Built from your Module 02 notes on fundamental types, fixed-width integers, initialization, literals, `const`/`constexpr`, type modifiers, `auto`, aliases, stack vs heap, conversions, scope/lifetime, references, and structured bindings.

---

## Instructions

Answer each question in the space provided. Try to answer from memory first, then check the answer key at the end.

---

## Questions

### 1) Fundamental types
Name the main categories of C++ fundamental types covered in the notes, and give at least one example from each category.

**Your answer:** char, bool, void, int, float, double.




---

### 2) `sizeof` and platform dependence
Which fundamental types in the notes were described as having platform-dependent sizes? Why is that important when writing portable code?

**Your answer:** well all types but specifically long and this is important because of the potential data that can be stored in them.




---

### 3) Fixed-width integers
What is the difference between `int`, `int32_t`, and `int_fast32_t`? When would you prefer each one?

**Your answer:** int is at least 4 bytes, int32_t is exactly 32 bits (4 bytes) and int_fast32_t is the type fastest on that architecture ~ 8 bytes.




---

### 4) Initialization styles
Explain the difference between these initialization forms:

```cpp
int b = 5;
int c(5);
int d{5};
int e = {5};
```

Which style is preferred in the notes, and why?

**Your answer:**
1. asignment
2. constructor
3. list initialization
4. assignment list initalization
The list initializaiton is performed with possible type checking when downgrading a variable type.




---

### 5) Narrowing conversions
What is narrowing? For each line below, say whether it is allowed and why:

```cpp
int z1 = 3.9;
int z2{3.9};
```

**Your answer:**
1. Is allowed, narrowing.
2. Not allowed the list initialization will warn you about this.




---

### 6) Endianness
If the value `0x12345678` is stored in memory, how would its bytes appear in:
- little-endian
- big-endian

Also explain what “little-endian” means in one sentence.

**Your answer:** little-endian and little-endian stores bits starting from the least significant bit.




---

### 7) Literals
Identify the type or meaning of each literal below:

```cpp
42
42u
42LL
3.14f
3.14L
0x2A
0b00101010
'\n'
nullptr
R"(Hello\nWorld)"
```

**Your answer:**
1. int
2. unsigned int
3. long long int
4. float
5. long double.
6. hexadecimal
7. binary




---

### 8) `const` vs `constexpr`
Explain the difference between `const` and `constexpr`. Give one example where `const` is valid but `constexpr` would not be.

**Your answer:**
const is saying this value can't change at runtime, constexpr is used for functions where the value may be determined at compile time if the given value is not a variable.




---

### 9) Pointer constness
For each declaration below, explain what is const and what can still change:

```cpp
const int* p1;
int* const p2 = nullptr;
const int* const p3 = nullptr;
```

**Your answer:**
1. is a pointer to an integer constant
2. constant pointer to an int
3. const pointer to a int const




---

### 10) Unsigned behavior
What happens here, and why?

```cpp
unsigned int u = 0;
u -= 1;
```

Is this undefined behavior?

**Your answer:**
No it is not UB it is defined and will underflow to the largest unsigned int value.




---

### 11) `auto` deduction
State the deduced type of each variable:

```cpp
auto x = 42;
auto y = 3.14;
auto z = 3.14f;
const int ci = 42;
auto a1 = ci;
auto& a2 = ci;
const auto a3 = ci;
```

Also explain what the notes mean by saying `auto` “drops const and references by default.”

**Your answer:**
1. int
2. double
3. float
4. const int
5. int
6. const int
7. const int
When assigned the auto keyword does not take the trait of const.




---

### 12) Stack vs heap
Compare stack and heap allocation in terms of:
- speed
- lifetime management
- typical use
- danger/risk

Then explain this code:

```cpp
int* p = new int(42);
delete p;
p = nullptr;
```

**Your answer:**
1. stack is faster than heap, stack just needs to move stack pointers and heap needs to to go through free list to find chunk.
2. idk
3. local variables and function frames for stacks and dynamic memory for heap.
4. smaller in size & heap is bigger in size but slower
5. allocates memory on the heap with the value 42 and then deletes it and then handles the dangling pointer by setting it to nullptr.




---

### 13) Type conversions
For each example below, describe what conversion happens and whether it is safe, lossy, or dangerous:

```cpp
long l = 42;
double d = 42;
int n = 3.14;
int m = static_cast<int>(3.14);
char* bytes = reinterpret_cast<char*>(&n);
```

**Your answer:**
1. long promotion, safe.
2. double promotion, safe.
3. narrowing, bad.
4. barrowing, safe
5. safe




---

### 14) Scope and lifetime
In the code below, explain:
- which `local` is used inside the inner block
- when `inner` is destroyed
- why `counter()` prints increasing values

```cpp
void example() {
    int local = 5;
    {
        int inner = 3;
        int local = 99;
    }
}

void counter() {
    static int count = 0;
    ++count;
    std::cout << count << "\n";
}
```

**Your answer:**




---

### 15) References and structured bindings
Answer all parts:

1. What is a reference in C++?
2. How is a reference different from a pointer?
3. What happens here?

```cpp
int x = 42;
int& ref = x;
int y = 99;
ref = y;
```

4. What do structured bindings do in this example?

```cpp
std::pair<int, double> p = {42, 3.14};
auto [integer, decimal] = p;
```

**Your answer:**




---

# Answer Key

## 1) Fundamental types
The main categories covered were:
- **Boolean**: `bool`
- **Integer types**: `short`, `int`, `long`, `long long`, and unsigned versions
- **Floating point**: `float`, `double`, `long double`
- **Character types**: `char`, `char8_t`, `char16_t`, `char32_t`, `wchar_t`
- **Void**: `void`

## 2) `sizeof` and platform dependence
The notes described these as platform dependent:
- `long` can be 4 or 8 bytes
- `wchar_t` can be 2 or 4 bytes
- `long double` can be 10 or 16 bytes

This matters because code that assumes exact sizes can break across systems. If size matters, prefer fixed-width types like `int32_t`.

## 3) Fixed-width integers
- `int`: normal general-purpose integer type; size is implementation-dependent, though often 4 bytes
- `int32_t`: exactly 32 bits
- `int_fast32_t`: at least 32 bits, but chosen to be fast on that platform

Use:
- `int` for everyday arithmetic
- `int32_t` when exact size matters, like file formats, protocols, serialization, hardware work
- `int_fast32_t` when you want at least 32 bits and care about performance portability

## 4) Initialization styles
```cpp
int b = 5;   // copy initialization
int c(5);    // direct initialization
int d{5};    // brace/list initialization
int e = {5}; // copy-list initialization
```
Preferred style: **brace initialization `{}`**

Why:
- prevents narrowing conversions
- works consistently across types
- is safer and more uniform

## 5) Narrowing conversions
Narrowing means converting a value to a type that cannot represent it exactly without losing information.

```cpp
int z1 = 3.9;
```
Allowed, but lossy. `z1` becomes `3`.

```cpp
int z2{3.9};
```
Not allowed. Brace initialization rejects narrowing.

## 6) Endianness
For `0x12345678`:
- **Little-endian**: `78 56 34 12`
- **Big-endian**: `12 34 56 78`

Little-endian means the **least significant byte is stored first** at the lowest memory address.

## 7) Literals
- `42` → decimal integer literal, usually `int`
- `42u` → unsigned integer literal
- `42LL` → `long long` literal
- `3.14f` → `float` literal
- `3.14L` → `long double` literal
- `0x2A` → hexadecimal integer literal
- `0b00101010` → binary integer literal
- `'\n'` → newline character literal
- `nullptr` → null pointer literal
- `R"(Hello\nWorld)"` → raw string literal, where `\n` is treated as the characters backslash and `n`, not as an escaped newline

## 8) `const` vs `constexpr`
- `const`: value cannot be changed after initialization, but it may be initialized at runtime
- `constexpr`: value must be known at compile time

Example where `const` is valid but `constexpr` is not:

```cpp
int runtime_val = 7;
const int a = runtime_val;      // OK
// constexpr int b = runtime_val; // ERROR
```

## 9) Pointer constness
```cpp
const int* p1;
```
`p1` is a pointer to const int.
- can change where `p1` points
- cannot modify the pointee through `p1`

```cpp
int* const p2 = nullptr;
```
`p2` is a const pointer to int.
- cannot change where `p2` points after initialization
- can modify the pointee through `p2` if it points to valid non-const int

```cpp
const int* const p3 = nullptr;
```
`p3` is a const pointer to const int.
- cannot change pointer
- cannot modify pointee through it

## 10) Unsigned behavior
```cpp
unsigned int u = 0;
u -= 1;
```
This wraps around to the maximum value representable by `unsigned int`, such as `4294967295` on a 32-bit unsigned type.

This is **defined behavior**, not UB. Unsigned arithmetic wraps modulo the range size.

## 11) `auto` deduction
```cpp
auto x = 42;      // int
auto y = 3.14;    // double
auto z = 3.14f;   // float
const int ci = 42;
auto a1 = ci;     // int
auto& a2 = ci;    // const int&
const auto a3 = ci; // const int
```

“Drops const and references by default” means plain `auto` deduces a non-reference type and usually strips top-level `const`.

So with:
```cpp
const int ci = 42;
auto a1 = ci;
```
`a1` becomes plain `int`, not `const int`.

## 12) Stack vs heap
**Stack**
- very fast allocation/deallocation
- automatic lifetime management
- used for local variables and function call frames
- limited size; objects die automatically at scope end

**Heap**
- slower allocation
- manual management, or managed through smart pointers
- used for dynamic lifetime / objects that outlive a scope / flexible sizing
- risk of leaks, dangling pointers, double frees

Code:
```cpp
int* p = new int(42);
delete p;
p = nullptr;
```
- `new int(42)` allocates an `int` on the heap and initializes it to `42`
- `p` itself is a stack variable that stores the address
- `delete p` frees the heap memory
- `p = nullptr` avoids leaving a dangling pointer value behind

## 13) Type conversions
```cpp
long l = 42;
```
`int` to `long`; widening conversion; generally safe

```cpp
double d = 42;
```
`int` to `double`; widening conversion; generally safe

```cpp
int n = 3.14;
```
`double` to `int`; lossy conversion; decimal part is discarded

```cpp
int m = static_cast<int>(3.14);
```
Still lossy, but explicit and clearer than relying on implicit conversion

```cpp
char* bytes = reinterpret_cast<char*>(&n);
```
Low-level reinterpretation of the object’s bytes; potentially useful for byte-level inspection, but dangerous and not for normal safe conversions

## 14) Scope and lifetime
- Inside the inner block, `local = 99` shadows the outer `local = 5`, so the inner one is used there
- `inner` is destroyed when the inner block ends, at its closing `}`
- `counter()` prints increasing values because `count` is `static`, so it is initialized only once and keeps its value across function calls

## 15) References and structured bindings
1. A reference is an alias for an existing object.
2. A reference differs from a pointer because it must be initialized, cannot be rebound, and conceptually behaves like another name for the same object. A pointer can be null and can point somewhere else later.
3. In this code:

```cpp
int x = 42;
int& ref = x;
int y = 99;
ref = y;
```

`ref` still refers to `x`. The statement `ref = y;` assigns the value of `y` into `x`. It does **not** rebind the reference. Afterward, `x` becomes `99`.

4. Structured bindings unpack an object into named variables. Here:

```cpp
std::pair<int, double> p = {42, 3.14};
auto [integer, decimal] = p;
```

`integer` gets `42` and `decimal` gets `3.14`.
