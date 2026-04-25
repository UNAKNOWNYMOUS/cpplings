# Module 05 Exercises — Object-Oriented Programming

Built from your Module 05 notes on structs and classes, class anatomy, object memory layout, the `this` pointer, constructors, `explicit`, the Big Six, deep vs shallow copy, operator overloading, inheritance, polymorphism, abstract classes, access control, `override`/`final`, multiple inheritance, `enum class`, `friend`, static members, and `dynamic_cast`.

---

## Instructions

Answer each question in the space provided. Try to answer from memory first, then check the answer key at the end.

---

## Questions

### 1) `struct` vs `class`
What is the only real language difference between a `struct` and a `class` in C++?

Then explain why people still often use them differently in style and design.

**Your answer:** `struct` access specifier is public by default and for `class` the default access specifier is private. `struct`s are used for simple aggregates types and `class`es are used for data encapsulation and a clear public interface.




---

### 2) Class anatomy
Look at this class and identify the role of each major part:

```cpp
class BankAccount {
private:
    std::string owner;
    double balance;

public:
    BankAccount() : owner("Unknown"), balance(0.0) {}
    BankAccount(const std::string& name, double initial_balance)
        : owner(name), balance(initial_balance) {}

    ~BankAccount() {}

    void deposit(double amount) {
        if (amount > 0) balance += amount;
    }

    double get_balance() const { return balance; }
};
```

Include:
- private section
- public section
- data members
- constructors
- destructor
- member functions
- const member function

**Your answer:**
- private section - what you can't access directly outside of the class.
- public section - what can be accessed outside the class.
- data members - data members + data functions; data + attributes.
- constructors - used for initialization of a object.
- destructor - used for destruction of a object.
- member functions - methods, also known as the attributes of a class.
- const member function - a promise that this function won't modify any class members.




---

### 3) Memory layout of an object
Explain what is stored inside a class object in memory and what is **not** stored inside the object.

Use the notes’ `BankAccount` example to explain:
- member variables
- member functions
- alignment/padding
- the idea that methods live in the `.text` segment

**Your answer:**
- member variables - This is stored inside the object.
- member functions - This is stored in the text segment of memory not in the variable - and the `this` pointer allows the methods to act upon a certain object.
- alignment/padding - This is used inside objects.
- the idea that methods live in the `.text` segment - Methods live inside the `.text` segment.




---

### 4) The `this` pointer
What is the `this` pointer?

Then explain what `return *this;` means in a member function such as:

```cpp
Counter& increment() {
    count++;
    return *this;
}
```

Why does returning `*this` make method chaining possible?

**Your answer:** The `this` pointer is a pointer to the current object. Returning `*this` returns a reference to the current object. Since returning `*this` returns a reference this would allow for chaining.




---

### 5) Constructors and member initializer lists
Why is a member initializer list preferred over assigning inside the constructor body?

Use `Rectangle(int w, int h, const std::string& n)` as your model and explain the efficiency difference.

**Your answer:**




---

### 6) Constructor delegation and `explicit`
Answer both parts:

1. What is constructor delegation, and why is it useful?
2. What does `explicit` prevent in a constructor like this?

```cpp
class Meters {
    double value;
public:
    explicit Meters(double v) : value{v} {}
};
```

Give one example of a call that is allowed and one that is prevented by `explicit`.

**Your answer:**




---

### 7) The Big Six
List the six special member functions.

Then explain the Rule of Five and the Rule of Zero in simple words.

**Your answer:**




---

### 8) Deep copy vs shallow copy
Suppose a class owns heap memory through a raw pointer.

Explain:
- what a shallow copy does
- why it causes bugs
- what a deep copy does
- why deep copy avoids double-free problems

**Your answer:**




---

### 9) Copy and move operations
In a class like `MyString`, what is the purpose of each of these?

- copy constructor
- copy assignment operator
- move constructor
- move assignment operator
- destructor

Also explain why move operations often set the source object’s pointer to `nullptr`.

**Your answer:**




---

### 10) Operator overloading
Using the `Vector2D` examples from the notes, explain what operator overloading is.

Then answer:
1. Why should `operator+=` usually return `*this` by reference?
2. Why is `operator<<` often written as a non-member friend?
3. Name two operators that **cannot** be overloaded.

**Your answer:**




---

### 11) Inheritance basics
Using `Animal`, `Dog`, and `Cat`, explain:
- what a base class is
- what a derived class is
- what `public` inheritance means conceptually
- why the derived constructor calls the base constructor first

**Your answer:**




---

### 12) Virtual functions and polymorphism
What does `virtual` do, and what is runtime polymorphism?

Then explain why this works the way it does:

```cpp
Animal* ptr = &dog;
ptr->speak();
```

Why does it call `Dog::speak()` instead of `Animal::speak()`?

**Your answer:**




---

### 13) Abstract classes, pure virtual functions, `override`, and `final`
Answer all parts:

1. What makes a class abstract?
2. What does `= 0` mean on a member function?
3. Why is `override` important?
4. What does `final` prevent on a virtual function or on a class?

**Your answer:**




---

### 14) Access control, friends, static members, and enums
Answer all parts briefly:

1. What is the difference between `private`, `protected`, and `public`?
2. What does a `friend` function or class get access to?
3. What is a static data member, and how is it different from a normal data member?
4. Why is `enum class` preferred over old-style `enum`?

**Your answer:**




---

### 15) Multiple inheritance and `dynamic_cast`
Answer both parts:

1. What is the diamond problem, and how does virtual inheritance solve it?
2. What does `dynamic_cast` do, when is it safe to use, and what requirement must the base class satisfy?

**Your answer:**




---

# Answer Key

## 1) `struct` vs `class`
The only language-level difference is the default access:
- `struct` members are **public** by default
- `class` members are **private** by default

In practice, people often use:
- `struct` for simple data aggregates with openly accessible members
- `class` for types with encapsulation and an explicit public interface

That is mostly style and design convention, not a major language difference.

## 2) Class anatomy
In the example:

- **private section**: holds implementation details hidden from outside code
- **public section**: exposes the usable interface of the class
- **data members**: `owner` and `balance`
- **constructors**: initialize newly created objects
- **destructor**: runs when the object is destroyed
- **member functions**: functions that operate on the object, such as `deposit`
- **const member function**: `get_balance() const`, which promises not to modify the object

## 3) Memory layout of an object
A class object stores its **member variables** in memory.

For `BankAccount`, the object contains storage for:
- `owner`
- `balance`

The exact total size can include **padding** added for alignment.

What is **not** stored inside each object:
- member function code

Member functions are regular functions stored in the program’s code section, often called the **.text segment**. They operate on a particular object through the hidden `this` pointer.

## 4) The `this` pointer
`this` is a hidden pointer available inside non-static member functions. It points to the specific object that the function was called on.

In:

```cpp
Counter& increment() {
    count++;
    return *this;
}
```

- `this` is a pointer to the current `Counter`
- `*this` is the actual current object
- returning `*this` by reference returns the same object after modification

This enables method chaining because each call returns the original object, so another member function can be called immediately:

```cpp
c.increment().increment().add(10);
```

## 5) Constructors and member initializer lists
A member initializer list is preferred because it constructs members **directly** with their desired values.

If you assign inside the constructor body:
- members are first default-constructed
- then assigned afterward

That can be less efficient, especially for types like `std::string`.

With an initializer list:

```cpp
Rectangle(int w, int h, const std::string& n)
    : width{w}, height{h}, name{n} {}
```

each member is initialized once, directly.

## 6) Constructor delegation and `explicit`
1. **Constructor delegation** means one constructor calls another constructor in the same class to reuse initialization logic and avoid duplication.

Example:
```cpp
Server(std::string h, int p) : Server{h, p, 30} {}
```

2. **`explicit`** prevents unintended implicit conversions.

For:
```cpp
Meters m1(3.5);      // allowed
Meters m2{3.5};      // allowed
```

But:
```cpp
Meters m3 = 3.5;     // prevented
set_height(3.5);     // prevented if set_height takes Meters
```

This reduces surprising automatic conversions.

## 7) The Big Six
The six special member functions are:
1. default constructor
2. destructor
3. copy constructor
4. copy assignment operator
5. move constructor
6. move assignment operator

**Rule of Five:** if you define one of the resource-management special members, you probably need to define the others too.

**Rule of Zero:** prefer designing your class so you do not need to write any of them yourself, usually by relying on standard library types that manage resources safely.

## 8) Deep copy vs shallow copy
A **shallow copy** copies the pointer value only, so two objects end up pointing to the same heap memory.

That causes bugs because:
- modifying one affects the other unexpectedly
- when both destructors run, both try to free the same memory
- this leads to double-free or dangling-pointer bugs

A **deep copy** allocates new memory and copies the actual contents into that new memory.

That way, each object owns its own separate resource, so destruction is safe and independent.

## 9) Copy and move operations
For a class like `MyString`:

- **copy constructor**: creates a new object as a deep copy of another object
- **copy assignment operator**: replaces an existing object’s contents with a deep copy of another object
- **move constructor**: steals resources from a temporary or expiring object
- **move assignment operator**: frees the current resource, then steals the other object’s resource
- **destructor**: releases owned resources when the object dies

Move operations often set the source pointer to `nullptr` so the moved-from object remains valid and its destructor will not free the resource twice.

## 10) Operator overloading
Operator overloading lets user-defined types behave naturally with operators such as `+`, `-`, `==`, `[]`, and `<<`.

1. `operator+=` should usually return `*this` by reference so operations can chain efficiently:

```cpp
a += b += c;
```

2. `operator<<` is often a non-member friend because the left operand is `std::ostream`, not your class. A friend can still access private members if needed.

3. Two operators that cannot be overloaded are:
- `::`
- `?:`

Others from the notes include `.`, `.*`, `sizeof`, and casts such as `static_cast`.

## 11) Inheritance basics
A **base class** is the general parent type, such as `Animal`.

A **derived class** is a more specific type that inherits from the base, such as `Dog` or `Cat`.

`public` inheritance means an **is-a** relationship:
- a `Dog` is an `Animal`
- a `Cat` is an `Animal`

The derived constructor calls the base constructor first because the base part of the object must be initialized before the derived-specific members.

## 12) Virtual functions and polymorphism
`virtual` enables **runtime dispatch**, meaning the program chooses the correct overridden function based on the object’s real type at runtime.

Runtime polymorphism means code can work through a base pointer or reference but still call the derived behavior.

So here:

```cpp
Animal* ptr = &dog;
ptr->speak();
```

it calls `Dog::speak()` because the actual object is a `Dog`, and `speak` is virtual. The lookup goes through the object’s vtable rather than binding only to the static type `Animal*`.

## 13) Abstract classes, pure virtual functions, `override`, and `final`
1. A class is **abstract** if it has at least one pure virtual function.
2. `= 0` marks a function as **pure virtual**, meaning derived classes must provide an override before concrete objects can be created.
3. `override` is important because the compiler checks that you are actually overriding a base-class virtual function. It helps catch mistakes such as wrong names or wrong signatures.
4. `final` prevents further overriding on a virtual function, or further inheritance from a class.

## 14) Access control, friends, static members, and enums
1. `private`: accessible only inside the class and friends  
   `protected`: accessible inside the class, friends, and derived classes  
   `public`: accessible from anywhere

2. A `friend` function or class gets access to the class’s private and protected members.

3. A **static data member** belongs to the class itself, not to each individual object. All instances share one copy.

4. `enum class` is preferred because:
- enumerator names stay scoped
- it avoids polluting the surrounding namespace
- conversions to integers are not implicit, which is safer

## 15) Multiple inheritance and `dynamic_cast`
1. The **diamond problem** happens when a class inherits from two classes that both inherit from the same base, causing duplicate base subobjects and ambiguity.

Virtual inheritance solves this by ensuring only **one shared base-class subobject** exists.

2. `dynamic_cast` performs a safe runtime checked cast in class hierarchies, usually for downcasting from a base pointer/reference to a derived pointer/reference.

It is safe because:
- pointer casts return `nullptr` on failure
- reference casts throw `std::bad_cast` on failure

Requirement: the base class must be **polymorphic**, meaning it must have at least one virtual function.
