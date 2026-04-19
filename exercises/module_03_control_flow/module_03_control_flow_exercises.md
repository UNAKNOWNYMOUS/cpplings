# Module 03 Exercises — Control Flow & Expressions

Built from your Module 03 notes on operators, precedence, `if`, ternary, `switch`, loops, range-for, `break`/`continue`, expressions vs statements, `constexpr if`, branch hints, exceptions, and assertions.

---

## Instructions

Answer each question in the space provided. Try to answer from memory first, then check the answer key at the end.

---

## Questions

### 1) Arithmetic operators and integer division
Explain what each line produces, and why:

```cpp
int a = 10, b = 3;
int quot = a / b;
int rem  = a % b;
int x = 7 / 2;
int y = -7 / 2;
double d = 7.0 / 2;
```

**Your answer:**




---

### 2) Division by zero
For each line below, say whether it is valid, undefined behavior, or produces a special floating-point result:

```cpp
int bad = 10 / 0;
double bad2 = 10.0 / 0.0;
```

Also explain why the two cases differ.

**Your answer:**




---

### 3) Pre-increment vs post-increment
Given this code:

```cpp
int x = 5;
int y = x++ * 2;
int z = ++x * 2;
```

State the values of `x`, `y`, and `z` after each line executes, and explain the difference between pre-increment and post-increment.

**Your answer:**




---

### 4) Floating-point comparison
Why is this usually a bad comparison?

```cpp
double x = 0.1 + 0.2;
double y = 0.3;
bool bad = (x == y);
```

What is the safer comparison style shown in the notes?

**Your answer:**




---

### 5) Short-circuit evaluation
Explain why the following is safe:

```cpp
int n = 0;
if (n != 0 && (10 / n > 2)) {
    // ...
}
```

Then explain what would go wrong if the condition were reversed.

**Your answer:**




---

### 6) Bitwise operations and flags
Describe what each operation does:

```cpp
flags |= (1 << 3);
flags &= ~(1 << 3);
bool is_set = (flags & (1 << 3)) != 0;
flags ^= (1 << 3);
```

What is the practical use of this style of programming?

**Your answer:**




---

### 7) Operator precedence
Evaluate these expressions and explain why parentheses improve readability:

```cpp
int result = 2 + 3 * 4;
int clear  = (2 + 3) * 4;
```

Then explain which has higher precedence: `*` or `+`.

**Your answer:**




---

### 8) `if`, braces, and `if` with initializer
Answer both parts:

1. Why do the notes strongly recommend always using braces with `if` statements?
2. What does this C++17 form do?

```cpp
if (int n = compute_value(); n > 0) {
    std::cout << n;
}
```

**Your answer:**




---

### 9) Ternary operator
What does the ternary operator do, and why is it called an expression rather than a statement?

Rewrite this logic using the ternary operator:

```cpp
if (score >= 60) {
    status = "Pass";
} else {
    status = "Fail";
}
```

**Your answer:**




---

### 10) `switch`, `break`, and fall-through
What is the role of `break` in a `switch` statement? What happens if you leave it out?

Also explain the purpose of `[[fallthrough]]`.

**Your answer:**




---

### 11) Loop types
Compare these loop forms:
- `while`
- `do-while`
- `for`
- range-based `for`

For each one, state when it is most appropriate.

**Your answer:**




---

### 12) `break`, `continue`, and nested loops
Explain the difference between `break` and `continue`.

Then answer: if a `break` appears inside an inner loop nested inside an outer loop, which loop does it exit?

**Your answer:**




---

### 13) Expressions vs statements
Explain the difference between an expression and a statement.

Classify each of these as an expression, a statement, or both in context:

```cpp
5 + 3
x = 10;
if (x > 0) { }
return x;
```

**Your answer:**




---

### 14) Assignment inside conditions
Explain the bug in this line:

```cpp
while (int ch = getchar() != EOF) {
    // ...
}
```

Then explain why this version is correct:

```cpp
while ((ch = getchar()) != EOF) {
    // ...
}
```

**Your answer:**




---

### 15) Compile-time branching, exceptions, and assertions
Answer all parts:

1. What does `if constexpr` do that regular `if` does not?
2. What are `[[likely]]` and `[[unlikely]]` used for?
3. What is the difference between `assert` and `static_assert`?
4. When would you use `throw`, `try`, and `catch`?

**Your answer:**




---

# Answer Key

## 1) Arithmetic operators and integer division
```cpp
int a = 10, b = 3;
int quot = a / b; // 3
int rem  = a % b; // 1
int x = 7 / 2;    // 3
int y = -7 / 2;   // -3
double d = 7.0 / 2; // 3.5
```
- Integer division truncates toward zero.
- `%` gives the remainder for integer operands.
- Floating-point division preserves the fractional part.

## 2) Division by zero
```cpp
int bad = 10 / 0;
```
This is **undefined behavior** for integer division.

```cpp
double bad2 = 10.0 / 0.0;
```
This produces a special IEEE 754 floating-point result such as positive infinity, not UB.

They differ because integer division by zero is not defined by the language, while floating-point arithmetic follows IEEE floating-point rules.

## 3) Pre-increment vs post-increment
Start with:
```cpp
int x = 5;
```
Then:
```cpp
int y = x++ * 2;
```
- `y = 10`
- then `x` becomes `6`

Then:
```cpp
int z = ++x * 2;
```
- `x` first becomes `7`
- `z = 14`

Post-increment uses the old value first, then increments. Pre-increment increments first, then uses the new value.

## 4) Floating-point comparison
`0.1 + 0.2` is not represented exactly in binary floating-point, so comparing directly with `==` often fails.

Safer style:
```cpp
std::abs(x - y) < 1e-9
```
or another epsilon-based comparison.

## 5) Short-circuit evaluation
This is safe:
```cpp
if (n != 0 && (10 / n > 2))
```
because `&&` stops at the first false operand. If `n == 0`, the right side is never evaluated, so no division by zero occurs.

If reversed:
```cpp
if ((10 / n > 2) && (n != 0))
```
it may divide by zero before checking `n != 0`.

## 6) Bitwise operations and flags
```cpp
flags |= (1 << 3);
```
Sets bit 3.

```cpp
flags &= ~(1 << 3);
```
Clears bit 3.

```cpp
bool is_set = (flags & (1 << 3)) != 0;
```
Tests whether bit 3 is set.

```cpp
flags ^= (1 << 3);
```
Toggles bit 3.

This style is useful for compactly storing and manipulating on/off options, permissions, and configuration flags.

## 7) Operator precedence
```cpp
int result = 2 + 3 * 4;    // 14
int clear  = (2 + 3) * 4;  // 20
```
`*` has higher precedence than `+`, so multiplication happens first unless parentheses force another order.

Parentheses improve readability by making intent explicit.

## 8) `if`, braces, and `if` with initializer
1. Braces prevent bugs where only the first statement is controlled by the `if`, while later indented lines run unconditionally.
2. In:
```cpp
if (int n = compute_value(); n > 0)
```
`n` is declared and initialized only for the `if`/`else` statement, then goes out of scope afterward.

## 9) Ternary operator
The ternary operator chooses between two values based on a condition:
```cpp
condition ? value_if_true : value_if_false
```
It is an expression because it produces a value.

Rewritten:
```cpp
status = (score >= 60) ? "Pass" : "Fail";
```

## 10) `switch`, `break`, and fall-through
`break` exits the `switch` after a matching case runs.

Without `break`, execution falls through into later cases.

`[[fallthrough]]` marks intentional fall-through so the compiler knows it is deliberate.

## 11) Loop types
- `while`: best when the number of iterations is not known in advance and you want to test before each iteration.
- `do-while`: best when the body must run at least once before checking the condition.
- `for`: best when you have clear initialization, condition, and update steps.
- range-based `for`: best for iterating through arrays, strings, and containers cleanly and safely.

## 12) `break`, `continue`, and nested loops
- `break` exits the innermost loop immediately.
- `continue` skips the rest of the current iteration and moves to the next iteration of the innermost loop.

In nested loops, `break` exits only the inner loop where it appears.

## 13) Expressions vs statements
An **expression** produces a value. A **statement** performs an action.

Classification:
```cpp
5 + 3
```
Expression.

```cpp
x = 10;
```
The assignment `x = 10` is an expression; with the semicolon, the full line is an expression statement.

```cpp
if (x > 0) { }
```
Statement.

```cpp
return x;
```
Statement.

## 14) Assignment inside conditions
Buggy version:
```cpp
while (int ch = getchar() != EOF)
```
Because `!=` has higher precedence than `=`, this is parsed like:
```cpp
int ch = (getchar() != EOF)
```
So `ch` gets `0` or `1`, not the actual character.

Correct version:
```cpp
while ((ch = getchar()) != EOF)
```
This first assigns the character result of `getchar()` to `ch`, then compares that value to `EOF`.

## 15) Compile-time branching, exceptions, and assertions
1. `if constexpr` chooses a branch at compile time, and the discarded branch is not compiled for that type/context.
2. `[[likely]]` and `[[unlikely]]` are branch prediction hints for the compiler.
3. `assert` is a runtime debug check that can be disabled in release builds. `static_assert` is a compile-time check that is always enforced.
4. `throw` is used to signal an error or exceptional condition, `try` marks code that may throw, and `catch` handles the exception.
