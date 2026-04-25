# Module 05 Exercises — Object-Oriented Programming (OOP)

Built from your Module 05 notes.

---

## Instructions
- Answer all 15 questions.
- Write your answers in the space provided under each question.
- An answer key is included at the end for checking your work after you finish.

---

## Questions

### 1. Struct vs Class
What is the only default difference between a `struct` and a `class` in C++?

**Your answer:**




---

### 2. Access Specifiers
What does each of the following mean?
- `private`
- `protected`
- `public`

**Your answer:**




---

### 3. Object Memory Layout
A class object stores its data members inside the object itself. Are member functions stored inside each object too? Explain where member functions live and what `this` is used for.

**Your answer:**




---

### 4. The `this` Pointer
In a non-static member function, what does `this` point to? Why does returning `*this` make method chaining possible?

**Your answer:**




---

### 5. Member Initializer Lists
Why is a member initializer list preferred over assigning to members inside the constructor body?

**Your answer:**




---

### 6. Constructor Features
Briefly explain each of these:
- constructor delegation
- `explicit` constructor

**Your answer:**




---

### 7. The Big Six
List the six special member functions.

**Your answer:**




---

### 8. Rule of Five / Rule of Zero
What does the Rule of Five mean? What does the Rule of Zero suggest?

**Your answer:**




---

### 9. Deep Copy vs Shallow Copy
Why is shallow copying dangerous for a class that owns heap memory like `MyString`?

**Your answer:**




---

### 10. Operator Overloading
Give two operators from the notes that can be overloaded, and two that cannot be overloaded.

**Your answer:**




---

### 11. Inheritance and `override`
What does it mean for `Dog` to publicly inherit from `Animal`? Why is the `override` keyword important?

**Your answer:**




---

### 12. Virtual Functions and Polymorphism
Explain what happens when an `Animal*` points to a `Dog` object and `ptr->speak()` is called.

**Your answer:**




---

### 13. Abstract Classes
What makes a class abstract? Can you create an object of an abstract class directly?

**Your answer:**




---

### 14. `enum class` vs plain `enum`
Why is `enum class` generally preferred over old-style `enum`?

**Your answer:**




---

### 15. Static Members and `dynamic_cast`
Answer both parts:
1. What is a static data member, and how is it different from a normal data member?
2. What does `dynamic_cast` do, and what must be true about the base class for it to work safely?

**Your answer:**




---

# Answer Key

## 1. Struct vs Class
The only default difference is access level:
- `struct` members are **public by default**
- `class` members are **private by default**

Otherwise, they are almost the same in C++.

---

## 2. Access Specifiers
- `private`: accessible only inside the same class (and friends)
- `protected`: accessible inside the class and derived classes
- `public`: accessible from anywhere the object is visible

---

## 3. Object Memory Layout
No, member functions are **not** stored inside each object.
They live like normal functions in the program’s code section (the `.text` segment).
The object stores its data members, and member functions use the hidden `this` pointer to know **which object** they are working on.

---

## 4. The `this` Pointer
In a non-static member function, `this` points to the current object that called the function.
Returning `*this` returns a reference to that same object, which allows chaining like:

```cpp
c.increment().add(10).increment();
```

Each function returns the same object again so the next call can continue.

---

## 5. Member Initializer Lists
A member initializer list is preferred because it **directly initializes** members instead of default-constructing them first and then assigning later.
That is usually more efficient and is required for some members such as:
- references
- `const` members
- members without a default constructor

---

## 6. Constructor Features
- **Constructor delegation**: one constructor calls another constructor in the same class to reuse initialization logic.
- **`explicit` constructor**: prevents unintended implicit conversions.

Example:
```cpp
Meters m1(3.5);     // OK
Meters m2 = 3.5;    // Not allowed if constructor is explicit
```

---

## 7. The Big Six
The six special member functions are:
1. Default constructor
2. Destructor
3. Copy constructor
4. Copy assignment operator
5. Move constructor
6. Move assignment operator

---

## 8. Rule of Five / Rule of Zero
- **Rule of Five**: if you define one of the important resource-management special member functions (especially destructor, copy, or move operations), you usually should define all relevant ones.
- **Rule of Zero**: prefer designing classes so you do **not** need to manually define them at all, by using well-behaved standard types that manage resources for you.

---

## 9. Deep Copy vs Shallow Copy
Shallow copy is dangerous because both objects end up holding the **same pointer** to the same heap memory.
That causes problems like:
- double deletion
- dangling pointers
- accidental shared mutation

Deep copy allocates new memory and copies the contents, so each object owns its own separate resource.

---

## 10. Operator Overloading
Examples of operators that **can** be overloaded:
- `+`
- `[]`
- `==`
- `<<`

Examples of operators that **cannot** be overloaded:
- `::`
- `.`
- `?:`
- `sizeof`

---

## 11. Inheritance and `override`
If `Dog` publicly inherits from `Animal`, then `Dog` is an **is-a** `Animal`.
It gets the accessible base-class members and can be used anywhere an `Animal` is expected.

`override` is important because it tells the compiler that you intend to override a virtual base-class function. If you accidentally mismatch the signature, the compiler gives an error instead of silently creating a different function.

---

## 12. Virtual Functions and Polymorphism
If an `Animal*` points to a `Dog` object and you call `ptr->speak()`, the call is resolved at runtime using **dynamic dispatch**.
Because `speak()` is virtual, C++ uses the object’s vtable/vptr mechanism to call `Dog::speak()` instead of `Animal::speak()`.

That is runtime polymorphism.

---

## 13. Abstract Classes
A class becomes abstract when it has at least one **pure virtual function**, written with `= 0`.

Example:
```cpp
virtual double area() const = 0;
```

You **cannot** create objects directly from an abstract class.
You must derive from it and implement the pure virtual functions.

---

## 14. `enum class` vs plain `enum`
`enum class` is preferred because:
- its enumerator names are scoped
- it avoids polluting the surrounding namespace
- it prevents many accidental implicit conversions to integers
- it is safer and clearer

Example:
```cpp
Direction::NORTH
```
instead of just `NORTH` floating in the outer scope.

---

## 15. Static Members and `dynamic_cast`
1. A **static data member** belongs to the class itself, not to each individual object. All instances share one copy. A normal data member exists separately inside every object.

2. `dynamic_cast` performs a checked runtime cast, usually for safe downcasting in inheritance hierarchies, such as converting `Animal*` to `Dog*`. For it to work safely, the base class must be **polymorphic**, meaning it must have at least one virtual function.

---

