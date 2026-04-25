# Module 05 Exercises — Object-Oriented Programming (OOP)

---

## Instructions

Answer each question in the space provided. Try to answer from memory first, then check the answer key at the end.

---

## Questions

### 1) `struct` vs `class`

In C++, what is the only difference between `struct` and `class`?

For each type below, state whether `x` is public or private by default:

```cpp
struct Point {
    double x;
};

class Circle {
    double radius;
};
```

**Your answer:**
The only difference between `struct` and `class` is:
`struct` - default access specifier is public.
`class` - default access specifier is private.

```cpp
struct Point {
    double x; // public
};

class Circle {
    double radius; // private
};
```




---

### 2) Class anatomy

Using this class, identify the private data members, public interface, constructor, member initializer list, member functions, and const member functions:

```cpp
class BankAccount {
private:
    std::string owner;
    double balance;

public:
    BankAccount(const std::string& name, double initial_balance)
        : owner(name), balance(initial_balance) {}

    void deposit(double amount);
    bool withdraw(double amount);
    double get_balance() const;
    const std::string& get_owner() const;
};
```

**Your answer:**




---

### 3\) Encapsulation and private data

Why should `balance` usually be private in a `BankAccount` class instead of public?

Explain what could go wrong if outside code could directly do this:

```cpp
acc.balance = -999999;
```

**Your answer:**




---

### 4\) Object memory layout and member functions

For this class:

```cpp
class BankAccount {
    std::string owner;
    double balance;
public:
    void deposit(double amount);
    double get_balance() const;
};
```

Answer all parts:

1. Which parts are stored inside each `BankAccount` object?
2. Are member functions stored inside every object?
3. Where do member functions live conceptually?
4. How does a member function know which object to operate on?

**Your answer:**




---

### 5\) The `this` pointer and method chaining

Explain what `this` means inside a member function.

Then explain why this class supports chaining:

```cpp
class Counter {
    int count;
public:
    Counter(int start) : count(start) {}

    Counter& increment() {
        count++;
        return *this;
    }

    Counter& add(int n) {
        this->count += n;
        return *this;
    }

    int get() const { return count; }
};

Counter c(0);
c.increment().increment().add(10).increment();
```

What is `c.get()` afterward?

**Your answer:**




---

### 6\) Member initializer lists

Explain why the second constructor is preferred:

```cpp
class Rectangle {
    int width;
    int height;
    std::string name;
public:
    Rectangle(int w, int h, std::string n) {
        width = w;
        height = h;
        name = n;
    }

    Rectangle(int w, int h, const std::string& n)
        : width{w}, height{h}, name{n} {}
};
```

Also explain when the constructor body runs relative to member initialization.

**Your answer:**




---

### 7\) Constructor delegation and `explicit`

Answer all parts:

1. What is constructor delegation?
2. What constructor does `Server()` eventually delegate to here?
3. What does `explicit` prevent?

```cpp
class Server {
    std::string host;
    int port;
    int timeout;
public:
    Server(std::string h, int p, int t) : host{h}, port{p}, timeout{t} {}
    Server(std::string h, int p) : Server{h, p, 30} {}
    Server() : Server{"localhost", 8080} {}
};

class Meters {
    double value;
public:
    explicit Meters(double v) : value{v} {}
};
```

**Your answer:**




---

### 8\) The Big Six and Rule of Five/Zero

List the Big Six special member functions.

Then explain the Rule of Five and the Rule of Zero.

**Your answer:**




---

### 9\) Deep copy vs shallow copy

For a class that owns heap memory:

```cpp
class MyString {
    char* data;
    size_t length;
};
```

Explain the difference between a shallow copy and a deep copy.

Why can a shallow copy cause a double-free bug?

**Your answer:**




---

### 10\) Move constructor and move assignment

Explain what the move constructor is doing here:

```cpp
MyString(MyString&& other) noexcept
    : data{other.data}, length{other.length} {
    other.data = nullptr;
    other.length = 0;
}
```

Why is the moved-from object set to `nullptr` and `0`?

**Your answer:**




---

### 11\) Operator overloading

Given this class:

```cpp
class Vector2D {
public:
    double x, y;

    Vector2D operator+(const Vector2D& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};
```

Answer all parts:

1. Why does `operator+` return by value?
2. Why does `operator+=` return `Vector2D&`?
3. Why are some operators, like `.` and `?:`, not overloadable?

**Your answer:**




---

### 12\) Inheritance, `virtual`, and `override`

For this code:

```cpp
class Animal {
protected:
    std::string name;
public:
    Animal(const std::string& n) : name{n} {}
    virtual ~Animal() {}
    virtual void speak() const {
        std::cout << name << " makes a sound\n";
    }
};

class Dog : public Animal {
public:
    Dog(const std::string& n) : Animal{n} {}
    void speak() const override {
        std::cout << name << " barks!\n";
    }
};
```

Answer all parts:

1. What does `protected` mean?
2. Why should the base destructor be virtual when using polymorphism?
3. What does `virtual` enable?
4. What does `override` protect you from?

**Your answer:**




---

### 13\) Polymorphism, vtables, and abstract classes

Answer all parts:

1. Why does this call `Dog::speak()` even though the pointer type is `Animal*`?

```cpp
Dog dog{"Rex"};
Animal* ptr = &dog;
ptr->speak();
```

2. What is a vtable?
3. What is a vptr?
4. What makes a class abstract?
5. Can you instantiate an abstract class directly?

**Your answer:**




---

### 14\) Access control, `final`, multiple inheritance, and the diamond problem

Answer all parts:

1. Compare `private`, `protected`, and `public` access.
2. What does `final` mean on a virtual function?
3. What does `final` mean on a class?
4. What is the diamond problem?
5. How does virtual inheritance solve the diamond problem?

**Your answer:**




---

### 15\) `enum class`, `friend`, static members, and `dynamic_cast`

Answer all parts:

1. Why is `enum class` preferred over old-style `enum`?
2. How do you convert an `enum class` value to an integer?
3. What does a `friend` function or class get access to?
4. What is a static data member?
5. Why must a static data member usually be defined in exactly one `.cpp` file?
6. What does `dynamic_cast` do?
7. What does `dynamic_cast<Dog*>(ptr)` return if `ptr` does not actually point to a `Dog`?
8. What requirement must the base class meet for `dynamic_cast` to work?

**Your answer:**




---

# Answer Key

## 1\) `struct` vs `class`

The only difference is default access:

- `struct`: members default to `public`
- `class`: members default to `private`

So:

```cpp
struct Point {
    double x;
};
```

`x` is public by default.

```cpp
class Circle {
    double radius;
};
```

`radius` is private by default.

## 2\) Class anatomy

Private data members:

```cpp
std::string owner;
double balance;
```

Public interface:

```cpp
BankAccount(...);
void deposit(double amount);
bool withdraw(double amount);
double get_balance() const;
const std::string& get_owner() const;
```

Constructor:

```cpp
BankAccount(const std::string& name, double initial_balance)
```

Member initializer list:

```cpp
: owner(name), balance(initial_balance)
```

Member functions:

```cpp
void deposit(double amount);
bool withdraw(double amount);
double get_balance() const;
const std::string& get_owner() const;
```

Const member functions:

```cpp
double get_balance() const;
const std::string& get_owner() const;
```

The `const` at the end promises not to modify the object’s member variables.

## 3\) Encapsulation and private data

`balance` should be private so the class can protect its own invariants.

For a bank account, valid behavior might require rules such as:

- no negative deposits
- no withdrawals above the current balance
- no arbitrary balance changes from outside code

If outside code could directly write:

```cpp
acc.balance = -999999;
```

then the object could be placed into an invalid state. Encapsulation forces outside code to go through controlled functions like `deposit()` and `withdraw()`.

## 4\) Object memory layout and member functions

1. Each `BankAccount` object stores its non-static data members, such as `owner` and `balance`.
2. Member functions are not stored inside every object.
3. Member functions live conceptually in the program’s code/text segment, like regular functions.
4. A member function knows which object to operate on through the hidden `this` pointer.

So every object has its own data, but the member function code is shared.

## 5\) The `this` pointer and method chaining

Inside a member function, `this` is a pointer to the object the function was called on.

In:

```cpp
Counter& increment() {
    count++;
    return *this;
}
```

`*this` means “the current object.” Returning `Counter&` allows the next member call to continue operating on the same object.

Starting from `Counter c(0);`:

```cpp
c.increment().increment().add(10).increment();
```

Step by step:

- start: `0`
- first `increment()`: `1`
- second `increment()`: `2`
- `add(10)`: `12`
- final `increment()`: `13`

So `c.get()` returns `13`.

## 6\) Member initializer lists

The member initializer list is preferred because it directly initializes members.

This version:

```cpp
Rectangle(int w, int h, std::string n) {
    width = w;
    height = h;
    name = n;
}
```

first initializes the members, then assigns to them inside the constructor body. For class-type members like `std::string`, this can mean default construction followed by assignment.

This version:

```cpp
Rectangle(int w, int h, const std::string& n)
    : width{w}, height{h}, name{n} {}
```

directly constructs each member with the intended value.

The constructor body runs **after** all members have already been initialized.

## 7\) Constructor delegation and `explicit`

1. Constructor delegation means one constructor calls another constructor in the same class to reuse initialization logic.
2. `Server()` delegates to:

```cpp
Server{"localhost", 8080}
```

which delegates to:

```cpp
Server{"localhost", 8080, 30}
```

So it eventually reaches the main three-argument constructor.

3. `explicit` prevents implicit conversions.

For example:

```cpp
explicit Meters(double v) : value{v} {}
```

prevents this:

```cpp
Meters m = 3.5;
```

and also prevents passing a raw `double` where a `Meters` object is expected unless you construct it explicitly:

```cpp
set_height(Meters{3.5});
```

## 8\) The Big Six and Rule of Five/Zero

The Big Six are:

1. Default constructor: `T()`
2. Destructor: `~T()`
3. Copy constructor: `T(const T&)`
4. Copy assignment operator: `T& operator=(const T&)`
5. Move constructor: `T(T&&)`
6. Move assignment operator: `T& operator=(T&&)`

Rule of Five:

If you define any of the destructor, copy constructor, copy assignment, move constructor, or move assignment, you should strongly consider defining all five because the class likely manages a resource manually.

Rule of Zero:

Prefer designing classes so they do not manually manage resources. Use standard library types like `std::string`, `std::vector`, and smart pointers so the compiler-generated special member functions are correct.

## 9\) Deep copy vs shallow copy

A shallow copy copies the pointer value only.

That means two objects point to the same heap allocation:

```cpp
a.data ---> [H e l l o \0]
b.data ---^
```

A deep copy allocates new memory and copies the characters, so each object owns its own separate buffer.

A shallow copy can cause a double-free bug because both objects think they own the same memory. When one destructor calls `delete[] data`, the memory is freed. When the second destructor later calls `delete[] data` on the same pointer, it tries to free memory that has already been freed.

## 10\) Move constructor and move assignment

The move constructor steals the resource from `other`:

```cpp
MyString(MyString&& other) noexcept
    : data{other.data}, length{other.length} {
    other.data = nullptr;
    other.length = 0;
}
```

Instead of allocating a new buffer and copying characters, it takes ownership of `other.data` directly.

Then it sets `other.data` to `nullptr` and `other.length` to `0` so the moved-from object remains valid and safe to destroy.

If `other.data` were not nulled out, both objects could believe they own the same memory, causing double deletion.

## 11\) Operator overloading

1. `operator+` returns by value because addition creates a new result object. It should not modify either operand.

```cpp
Vector2D c = a + b;
```

2. `operator+=` returns `Vector2D&` because it modifies the left-hand object and returns that same object, allowing chaining.

```cpp
a += b += c;
```

3. Operators like `.`, `?:`, `sizeof`, `::`, and casts are not overloadable because C++ protects core language semantics. Allowing those to be redefined would make basic language behavior unpredictable.

## 12\) Inheritance, `virtual`, and `override`

1. `protected` means members are accessible inside the same class and inside derived classes, but not from unrelated outside code.
2. A base destructor should be virtual when using polymorphism so deleting through a base pointer correctly runs the derived destructor first, then the base destructor.
3. `virtual` enables runtime dispatch, meaning the function called depends on the actual object type at runtime, not only the pointer/reference type.
4. `override` tells the compiler that this function is meant to override a base-class virtual function. If the name, parameters, or const-qualification are wrong, the compiler catches the mistake.

## 13\) Polymorphism, vtables, and abstract classes

1. This calls `Dog::speak()` because `speak()` is virtual. Even though the pointer type is `Animal*`, the actual object is a `Dog`, so dynamic dispatch calls the derived override.

```cpp
Dog dog{"Rex"};
Animal* ptr = &dog;
ptr->speak();
```

2. A vtable is a table of function pointers used for virtual dispatch.
3. A vptr is a hidden pointer inside each polymorphic object that points to the correct vtable for that object’s dynamic type.
4. A class becomes abstract when it has at least one pure virtual function, such as:

```cpp
virtual double area() const = 0;
```

5. No, you cannot instantiate an abstract class directly.

## 14\) Access control, `final`, multiple inheritance, and the diamond problem

1. Access control:

- `private`: accessible only inside the same class and friends
- `protected`: accessible inside the same class, derived classes, and friends
- `public`: accessible from anywhere

2. `final` on a virtual function means derived classes cannot override that function.
3. `final` on a class means no class can inherit from it.
4. The diamond problem happens when a derived class inherits from two classes that both inherit from the same base, producing two copies of the base subobject.
5. Virtual inheritance ensures there is only one shared base subobject, removing the ambiguity.

Example shape:

```cpp
class A {};
class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {}; // one shared A
```

## 15\) `enum class`, `friend`, static members, and `dynamic_cast`

1. `enum class` is preferred because it is scoped and does not pollute the surrounding namespace.
2. Convert to an integer with `static_cast`:

```cpp
int n = static_cast<int>(Direction::EAST);
```

3. A `friend` function or class gets access to private and protected members.
4. A static data member is shared across all instances of a class instead of being stored separately in each object.
5. A static data member usually needs one definition in exactly one `.cpp` file so storage is allocated once and the One-Definition Rule is not violated.
6. `dynamic_cast` performs safe runtime downcasting in polymorphic class hierarchies.
7. `dynamic_cast<Dog*>(ptr)` returns `nullptr` if `ptr` does not actually point to a `Dog`.
8. The base class must have at least one virtual function so runtime type information \(RTTI\) is available.
