# Module 04 Exercises — Functions

Built from your Module 04 notes on function anatomy, the call stack, parameter passing, return values, default arguments, overloading, templates, inline functions, variadic functions, lambdas, `std::function`, function pointers, `noexcept`, recursion, and `constexpr` functions.

---

## Instructions

Answer each question in the space provided. Try to answer from memory first, then check the answer key at the end.

---

## Questions

### 1) Function anatomy
Identify the parts of this function and explain what each one means:

```cpp
const std::string& get_name(const Person& p, bool full_name = true) {
    return full_name ? p.full_name : p.first_name;
}
```

Include:
- return type
- function name
- parameter list
- default argument
- function body

**Your answer:**
- return type = `const std::string&`: The type the function returns
- function name = `get_name`: The name of the function used to call it
- parameter list = `(const Perosn& p, bool full_name = true)`: The formal parameter list of what information is passed when called
- default argument = `full_name = true`: Used for when this piece of information is not passed in the actual argument parameter list.
- function body = `return full_name ? p.full_name : p.first_name;`: Enclosed in `{}` which defines what the function does.




---

### 2) What happens during a function call?
Using the notes on the call stack, explain what happens when `main()` calls `add(3, 4)`.

Include the ideas of:
- arguments
- return address
- stack frame
- jump to function
- return value

**Your answer:**
Arguments are pushed onto the stack (which can be stored in registers), return address is pushed onto the stack, jump to entry point of the function, create stack frame for that function, compute result (which can be stored in registers EAX/RAX), stack pops local variables + `add`'s stack frame. Return's back to caller's stack frame and reads result from (EAX/RAX).




---

### 3) Pass by value
Given this code:

```cpp
void double_it(int x) {
    x *= 2;
}

int n = 5;
double_it(n);
```

What is the value of `n` afterward, and why?

Then state when pass-by-value is usually the best choice.

**Your answer:**
`n` is still 5 because of pass-by-value.
For small types (<8 bytes) no need to return something and just need a local copy.




---

### 4) Pass by reference vs const reference vs pointer
Compare these three parameter forms:

```cpp
void f1(int& x);
void f2(const std::string& s);
void f3(int* p);
```

For each one, explain:
- whether a copy is made
- whether the original can be modified
- whether `nullptr` is possible
- when the style is appropriate

**Your answer:**
```cpp
void f1(int& x); // no copy, yes original can be modified, nullptr not possible, when you want to modify originals
void f2(const std::string& s); // no copy, yes originals can be modified, nullptr not possible, for large objects that only need to be read-only
void f3(int* p); // yes copy of the pointer variable, yes originals can be modified, nullptr is possible, when you possibly need to check nullptr, need to change pointers, or working with C code
```




---

### 5) Choosing the right parameter passing mode
For each case below, pick the best passing style and explain why:

1. A function takes an `int` and only reads it.
2. A function takes a large `std::vector<int>` and only reads it.
3. A function must modify the caller’s `std::string`.
4. A function parameter is optional and may be absent.
5. A function takes ownership of a temporary `std::string`.

**Your answer:**
1. pass-by-value
2. const reference
3. reference
4. pointer
5. move semantics




---

### 6) Return values and dangling references
Explain why this function is dangerous:

```cpp
int& dangerous() {
    int local = 42;
    return local;
}
```

What is the problem called, and why does it happen?

Also state one safe case where returning a reference is fine.

**Your answer:**
returning reference to a local variable will be UB. It is safe returning a reference it what you are returning outlives the current method/function.




---

### 7) Returning by value and optimization
Why is returning by value usually fine in modern C++?

Explain what RVO/NRVO means in simple words, using this idea:

```cpp
std::vector<int> make_vector() {
    std::vector<int> v = {1,2,3,4,5};
    return v;
}
```

**Your answer:**
Compiler will optimize to do it in the caller instead! RVO - Return Value Optimization + NRVO - Named Return Value Optimzation where the compiler instead of pass-by-value it will just construct the function in the caller. So for the function where it returns the object v that is constructing locally in that function and will be passed to the caller via pass-by-value the compiler will construct that in caller instead to prevent creating a temp copy of the object.




---

### 8) Multiple return values and “might fail” functions
What are two modern ways to return more than one value from a function?

What is `std::optional` used for, and what does `std::nullopt` mean?

**Your answer:** `std::paid`, `std::tuple`, `struct`. `std::optional` - means it is optional that the function returns something of value and `std::nullopt` means the function did not return a value.




---

### 9) Default arguments
Answer all parts:

1. Why must default arguments appear at the end of the parameter list?
2. Why can you not specify the same default argument in both the declaration and the definition?
3. For this function:

```cpp
void print(std::string msg, int times = 1, char sep = '\n');
```

What happens in each call?

```cpp
print("Hello");
print("Hello", 3);
print("Hello", 3, ' ');
```

**Your answer:**
1. In the tail-end because they should be the ones that are omitted the most since you cannot skip arguments you input.
2. Violates ODR
3.
```cpp
print("Hello"); // "Hello", 1, '\n'
print("Hello", 3); // "Hello", 3, '\n'
print("Hello", 3, ' '); // "Hello", 3, ' '
```




---

### 10) Function overloading
Given these overloads:

```cpp
int add(int a, int b);
double add(double a, double b);
int add(int a, int b, int c);
```

Which overload is called for each of these?

```cpp
add(1, 2); // 1
add(1.0, 2.0); // 2
add(1, 2, 3); // 3
```

Then explain why `add(1, 2.0)` is problematic.

**Your answer:**
Ambiguous call




---

### 11) Templates, inline, and variadic functions
Answer all parts:

1. What problem does a function template solve?
2. What is the practical meaning of `inline` in modern C++?
3. Why are variadic templates preferred over old C-style variadic functions?

**Your answer:**
1. Allows you to work with multiple types without repeating code.
2. `inline` is suggestion to the C++ compiler to inline a function and not do all the overhead.
3. type-safe, compile time check




---

### 12) Lambda basics
For this lambda:

```cpp
auto add = [](int a, int b) -> int {
    return a + b;
};
```

Explain each part:
- capture list
- parameter list
- return type
- body

Then explain why lambdas are useful.

**Your answer:**
1. none
2. int a, int b
3. int
4. return a + b



---

### 13) Lambda captures and `mutable`
Given:

```cpp
int x = 10;
int y = 20;

auto f1 = [x, y]() { return x + y; };
auto f2 = [&x, &y]() { return x + y; };
```

Explain the difference between capture-by-value and capture-by-reference.

Then explain what `mutable` does for a lambda that captures by value.

**Your answer:**




---

### 14) `std::function` vs function pointers
Compare:
- `std::function<int(int, int)>`
- `int (*fp)(int, int)`

What can each store? Which is more flexible? Which usually has less overhead?

**Your answer:**




---

### 15) `noexcept`, recursion, and `constexpr`
Answer all parts:

1. What does `noexcept` promise?
2. What happens if an exception escapes a `noexcept` function?
3. Why does recursion always need a base case?
4. What problem can happen with very deep recursion?
5. What is a `constexpr` function, and when can it run at compile time?

**Your answer:**




---

# Answer Key

## 1) Function anatomy
In:

```cpp
const std::string& get_name(const Person& p, bool full_name = true) {
    return full_name ? p.full_name : p.first_name;
}
```

- **Return type:** `const std::string&`
- **Function name:** `get_name`
- **Parameter list:** `(const Person& p, bool full_name = true)`
- **Default argument:** `full_name = true`
- **Function body:** the block inside `{ ... }`

The function returns a read-only reference to a string, takes a `Person` by const reference, and takes a boolean flag that defaults to `true` if the caller omits it.

## 2) What happens during a function call?
When `main()` calls `add(3, 4)`:
- the arguments are passed, either in registers or on the stack
- a return address is saved so the CPU knows where to continue afterward
- control jumps to the start of `add`
- `add` creates its own stack frame for local state
- `add` computes the result
- the return value is placed in a return register
- the function frame is removed and execution jumps back to the saved return address in `main`

## 3) Pass by value
`n` is still `5` afterward.

Reason: `double_it(int x)` receives a copy of `n`. Modifying `x` changes only the local copy, not the original variable.

Pass-by-value is usually best for:
- small, cheap-to-copy types such as `int`, `double`, `bool`, and `char`
- cases where you want an independent local copy

## 4) Pass by reference vs const reference vs pointer
```cpp
void f1(int& x);
```
- no copy is made
- the original can be modified
- `nullptr` is not possible
- use when the function must modify the caller’s object

```cpp
void f2(const std::string& s);
```
- no copy is made
- the original cannot be modified through this parameter
- `nullptr` is not possible
- use for large input-only objects to avoid copying

```cpp
void f3(int* p);
```
- no copy of the pointee is made, but the pointer value itself is passed
- the original pointee can be modified through the pointer
- `nullptr` is possible
- use when the parameter is optional, when null is meaningful, or for C-style APIs

## 5) Choosing the right parameter passing mode
1. Read-only `int` → **pass by value**
2. Read-only large `std::vector<int>` → **pass by `const&`**
3. Must modify caller’s `std::string` → **pass by reference**
4. Optional parameter that may be absent → **pass by pointer**
5. Taking ownership of a temporary `std::string` → **pass by rvalue reference `&&`**

## 6) Return values and dangling references
This function is dangerous because it returns a reference to a local variable:

```cpp
int& dangerous() {
    int local = 42;
    return local;
}
```

When the function returns, `local` is destroyed. The returned reference now refers to an object that no longer exists.

This is called a **dangling reference**, and using it is undefined behavior.

A safe case is returning a reference to an object that outlives the function call, such as an element in a vector passed by reference:

```cpp
int& get_element(std::vector<int>& v, int i) {
    return v[i];
}
```

## 7) Returning by value and optimization
Returning by value is usually fine because modern C++ compilers can avoid extra copying through **RVO/NRVO**.

- **RVO**: Return Value Optimization
- **NRVO**: Named Return Value Optimization

In simple words, the compiler can construct the returned object directly in the caller’s storage instead of making a temporary copy.

So in:

```cpp
std::vector<int> make_vector() {
    std::vector<int> v = {1,2,3,4,5};
    return v;
}
```

the vector is often built directly where the caller needs it.

## 8) Multiple return values and “might fail” functions
Two modern ways to return multiple values are:
- `std::pair`
- `std::tuple`

A custom `struct` is also a very good modern option.

`std::optional<T>` is used when a function may or may not have a valid result.

`std::nullopt` means “there is no value.”

Example idea: square root of a negative number may return `std::nullopt` instead of a valid `double`.

## 9) Default arguments
1. Default arguments must appear at the end because arguments are filled from left to right, and C++ does not let you skip a middle argument.
2. You cannot repeat the default in both declaration and definition because that would be a duplicate specification and violates the rules around function declarations/definitions.
3. For:

```cpp
void print(std::string msg, int times = 1, char sep = '\n');
```

- `print("Hello");` → `times = 1`, `sep = '\n'`
- `print("Hello", 3);` → `times = 3`, `sep = '\n'`
- `print("Hello", 3, ' ');` → `times = 3`, `sep = ' '`

## 10) Function overloading
- `add(1, 2);` → calls `int add(int, int)`
- `add(1.0, 2.0);` → calls `double add(double, double)`
- `add(1, 2, 3);` → calls `int add(int, int, int)`

`add(1, 2.0)` is problematic because there is no exact match. The compiler may consider multiple conversions, which can lead to ambiguity.

## 11) Templates, inline, and variadic functions
1. A function template lets you write one generic function that works for many types.
2. In modern C++, `inline` mainly means the function is allowed to be defined in a header and appear in multiple translation units without violating the ODR. It is also still a hint that the compiler may inline it.
3. Variadic templates are preferred because they are type-safe, checked at compile time, and avoid the unsafe behavior of old C-style `...` functions.

## 12) Lambda basics
For:

```cpp
auto add = [](int a, int b) -> int {
    return a + b;
};
```

- **Capture list:** `[]` — captures nothing
- **Parameter list:** `(int a, int b)`
- **Return type:** `-> int`
- **Body:** `{ return a + b; }`

Lambdas are useful because they let you define small functions inline, especially for callbacks, algorithms, and short one-off logic.

## 13) Lambda captures and `mutable`
With capture-by-value:

```cpp
auto f1 = [x, y]() { return x + y; };
```

the lambda stores copies of `x` and `y` from the moment the lambda is created.

With capture-by-reference:

```cpp
auto f2 = [&x, &y]() { return x + y; };
```

the lambda uses the original variables, so later changes to `x` or `y` affect what the lambda sees.

`mutable` allows a lambda to modify its captured-by-value copies. It does **not** modify the original outside variables.

## 14) `std::function` vs function pointers
`std::function<int(int, int)>` can store many kinds of callables:
- regular functions
- lambdas
- functors
- other compatible callable objects

`int (*fp)(int, int)` stores only a pointer to a regular function with that exact signature.

- **More flexible:** `std::function`
- **Usually less overhead:** function pointer

`std::function` is convenient, but it may use type erasure and sometimes heap allocation, which adds overhead.

## 15) `noexcept`, recursion, and `constexpr`
1. `noexcept` promises that the function will not allow an exception to escape.
2. If an exception escapes a `noexcept` function, `std::terminate()` is called immediately.
3. Recursion needs a base case so the function eventually stops calling itself.
4. Very deep recursion can cause a **stack overflow** because each call uses another stack frame.
5. A `constexpr` function is a function that can be evaluated at compile time when called in a constant-expression context and given compile-time-known inputs. The same function can also run at runtime when needed.
