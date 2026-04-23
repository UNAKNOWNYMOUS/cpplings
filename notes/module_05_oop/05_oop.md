# Module 05: Object-Oriented Programming (OOP)

> *"OOP is not about hiding data — it's about expressing relationships between concepts with clarity and safety."*

---

## 5.1 Structs and Classes

In C++, `struct` and `class` are almost identical. The **only difference** is default access:
- `struct`: members default to **public**
- `class`: members default to **private**

```cpp
// A struct (public by default)
struct Point {
    double x;  // public
    double y;  // public
};

// A class (private by default)
class Circle {
    double radius;  // private! Cannot access from outside
public:
    double get_radius() const { return radius; }
};
```

---

## 5.2 Classes — Complete Anatomy

```cpp
#include <iostream>
#include <string>

class BankAccount {
private:               // ← Access specifier (private section)
    std::string owner; // Member variable (data member)
    double balance;    // Member variable

public:                // ← Public interface

    // ─── CONSTRUCTORS ───────────────────────────────────────────────────
    // Default constructor (no arguments)
    BankAccount() : owner("Unknown"), balance(0.0) {}

    // Parameterized constructor
    BankAccount(const std::string& name, double initial_balance)
        : owner(name), balance(initial_balance) {
        // Member initializer list ↑ (preferred over assignment in body)
    }

    // Copy constructor (made by compiler if you don't write it)
    BankAccount(const BankAccount& other)
        : owner(other.owner), balance(other.balance) {}

    // ─── DESTRUCTOR ──────────────────────────────────────────────────────
    ~BankAccount() {
        std::cout << "Account for " << owner << " closed.\n";
    }

    // ─── MEMBER FUNCTIONS (METHODS) ──────────────────────────────────────
    void deposit(double amount) {
        if (amount > 0) balance += amount;
    }

    bool withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }

    // ─── CONST MEMBER FUNCTIONS ───────────────────────────────────────────
    // 'const' at end: promises NOT to modify any member variables
    double get_balance() const { return balance; }
    const std::string& get_owner() const { return owner; }

    void print() const {
        std::cout << owner << ": $" << balance << "\n";
    }
};

int main() {
    BankAccount acc("Alice", 1000.0);
    acc.deposit(500.0);
    acc.withdraw(200.0);
    acc.print();  // Alice: $1300.00

    // acc.balance = 9999;  // ERROR: balance is private!
    return 0;
}
```

---

## 5.3 The Memory Layout of a Class

```
┌──────────────────────────────────────────────────────────────────┐
│           CLASS OBJECT MEMORY LAYOUT                             │
│                                                                  │
│  class BankAccount {                                             │
│      std::string owner;  // 32 bytes (on typical 64-bit system)  │
│      double balance;     // 8 bytes                              │
│  };                                                              │
│                                                                  │
│  sizeof(BankAccount) = 40 bytes (subject to alignment padding)   │
│                                                                  │
│  Memory at address 0x2000:                                       │
│  ┌──────────────────────────────────────────┐                    │
│  │  owner (std::string, 32 bytes)           │ 0x2000–0x201F      │
│  │    [ptr to chars | size | capacity]      │                    │
│  ├──────────────────────────────────────────┤                    │
│  │  balance (double, 8 bytes)               │ 0x2020–0x2027      │
│  └──────────────────────────────────────────┘                    │
│                                                                  │
│  MEMBER FUNCTIONS are NOT stored in the object!                  │
│  They are regular functions in the .text segment.                │
│  The 'this' pointer tells them WHICH object to act on.           │
└──────────────────────────────────────────────────────────────────┘
```

### The `this` Pointer

```cpp
class Counter {
    int count;
public:
    Counter(int start) : count(start) {}

    // When you call this on an object, 'this' points to that object
    Counter& increment() {
        count++;
        return *this;  // Return reference to THIS object (for chaining)
    }

    Counter& add(int n) {
        this->count += n;  // 'this->' is usually redundant but explicit
        return *this;
    }

    int get() const { return count; }
};

Counter c(0);
c.increment().increment().add(10).increment();  // Method chaining!
std::cout << c.get();  // 13
```

---

## 5.4 Constructors — Deep Dive

### Member Initializer List (ALWAYS USE THIS!)

```cpp
class Rectangle {
    int width;
    int height;
    std::string name;  // Would be default-initialized then assigned in body
public:
    // BAD: Assignment in body (name is default-constructed, then assigned)
    Rectangle(int w, int h, std::string n) {
        width = w;        // Assignment (not initialization)
        height = h;
        name = n;         // Default-construct name, then copy-assign — WASTEFUL!
    }

    // GOOD: Member initializer list (directly constructs members)
    Rectangle(int w, int h, const std::string& n)
        : width{w}, height{h}, name{n}  // Direct initialization — ONE operation
    {
        // Body runs AFTER all members are initialized
    }
};
```

### Constructor Delegation (C++11)

```cpp
class Server {
    std::string host;
    int port;
    int timeout;
public:
    Server(std::string h, int p, int t) : host{h}, port{p}, timeout{t} {}

    // Delegate to main constructor:
    Server(std::string h, int p) : Server{h, p, 30} {}  // timeout defaults to 30
    Server() : Server{"localhost", 8080} {}              // All defaults
};
```

### `explicit` Constructor

```cpp
class Meters {
    double value;
public:
    explicit Meters(double v) : value{v} {}  // MUST be called explicitly
};

Meters m1(3.5);       // OK: explicit construction
Meters m2 = 3.5;      // ERROR: implicit conversion prevented by 'explicit'
Meters m3{3.5};       // OK: direct initialization

void set_height(Meters m) { /* ... */ }
set_height(3.5);      // ERROR: no implicit conversion
set_height(Meters{3.5}); // OK
```

---

## 5.5 The Big Six (Special Member Functions)

Every class can have up to **6 special member functions**. Understanding them is critical.

```
┌──────────────────────────────────────────────────────────────────────┐
│              THE BIG SIX SPECIAL MEMBER FUNCTIONS                    │
│                                                                      │
│  1. Default constructor:      T()                                    │
│  2. Destructor:               ~T()                                   │
│  3. Copy constructor:         T(const T&)                            │
│  4. Copy assignment:          T& operator=(const T&)                 │
│  5. Move constructor:         T(T&&)                     [C++11]     │
│  6. Move assignment:          T& operator=(T&&)          [C++11]     │
│                                                                      │
│  If you don't write them, the compiler GENERATES them (if possible)  │
│                                                                      │
│  THE RULE OF FIVE (or Zero):                                         │
│  If you define ANY of 2, 3, 4, 5, or 6 — define ALL of them!         │
│  OR: define NONE of them (rule of zero — prefer this!)               │
└──────────────────────────────────────────────────────────────────────┘
```

### Complete Example: A String Class

```cpp
class MyString {
    char* data;
    size_t length;

public:
    // 1. Default constructor
    MyString() : data{new char[1]}, length{0} {
        data[0] = '\0';
    }

    // 1b. Parameterized constructor
    MyString(const char* str) : length{std::strlen(str)} {
        data = new char[length + 1];
        std::memcpy(data, str, length + 1);
    }

    // 2. Destructor — FREE the heap memory!
    ~MyString() {
        delete[] data;  // Without this: memory leak!
    }

    // 3. Copy constructor — DEEP copy
    MyString(const MyString& other) : length{other.length} {
        data = new char[length + 1];
        std::memcpy(data, other.data, length + 1);
        // NOT: data = other.data;  ← SHALLOW copy = double-free bug!
    }

    // 4. Copy assignment operator
    MyString& operator=(const MyString& other) {
        if (this == &other) return *this;  // Self-assignment guard!

        delete[] data;                     // Free old memory
        length = other.length;
        data = new char[length + 1];
        std::memcpy(data, other.data, length + 1);
        return *this;
    }

    // 5. Move constructor (C++11) — STEAL the resources
    MyString(MyString&& other) noexcept
        : data{other.data}, length{other.length} {
        other.data = nullptr;   // Leave the moved-from object in valid state
        other.length = 0;
    }

    // 6. Move assignment operator
    MyString& operator=(MyString&& other) noexcept {
        if (this == &other) return *this;

        delete[] data;             // Free current resources
        data = other.data;         // Steal other's resources
        length = other.length;
        other.data = nullptr;      // Null out the moved-from object
        other.length = 0;
        return *this;
    }

    size_t size() const { return length; }
    const char* c_str() const { return data; }
};
```

### Why Deep Copy vs Shallow Copy Matters

```
┌─────────────────────────────────────────────────────────────────────┐
│            SHALLOW COPY vs DEEP COPY                                │
│                                                                     │
│  MyString a("Hello");     Heap:  [H|e|l|l|o|\0]                     │
│                                      ↑                              │
│  a.data ─────────────────────────────┘                              │
│                                                                     │
│  SHALLOW COPY (BAD):       DEEP COPY (GOOD):                        │
│  MyString b = a;           MyString b = a;                          │
│                                                                     │
│  a.data ──────┐            a.data ───────→ [H|e|l|l|o|\0]           │
│               ↓                                                     │
│  [H|e|l|l|o|\0]           b.data ───────→ [H|e|l|l|o|\0] (COPY)     │
│               ↑                                                     │
│  b.data ──────┘            Independent! Modifying b doesn't         │
│                            affect a. Destructors work correctly.    │
│  PROBLEM: Both a and b                                              │
│  have the SAME pointer!                                             │
│  When b destructs: delete[]                                         │
│  When a destructs: delete[] AGAIN → DOUBLE FREE! CRASH!             │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 5.6 Operator Overloading

You can give custom meaning to operators for your types.

```cpp
#include <iostream>
#include <cmath>

class Vector2D {
public:
    double x, y;

    Vector2D(double x = 0, double y = 0) : x{x}, y{y} {}

    // Addition
    Vector2D operator+(const Vector2D& other) const {
        return {x + other.x, y + other.y};
    }

    // Subtraction
    Vector2D operator-(const Vector2D& other) const {
        return {x - other.x, y - other.y};
    }

    // Scalar multiplication
    Vector2D operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }

    // Compound assignment
    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Equality
    bool operator==(const Vector2D& other) const {
        return x == other.x && y == other.y;
    }

    // Negation (unary)
    Vector2D operator-() const { return {-x, -y}; }

    // Magnitude
    double magnitude() const { return std::sqrt(x*x + y*y); }

    // Stream output (non-member, but often friend)
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }

    // Subscript operator
    double& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        throw std::out_of_range("Index out of range");
    }
};

int main() {
    Vector2D a{1, 2}, b{3, 4};
    Vector2D c = a + b;         // {4, 6}
    Vector2D d = a * 2.0;       // {2, 4}
    std::cout << c << "\n";     // (4, 6)
    std::cout << c.magnitude(); // sqrt(52)
    return 0;
}
```

### Operators You Can Overload

```
OVERLOADABLE:
+  -  *  /  %  ^  &  |  ~  !  =  <  >  +=  -=  *=  /=
%=  ^=  &=  |=  <<  >>  >>=  <<=  ==  !=  <=  >=  <=>
&&  ||  ++  --  ,  ->*  ->  ()  []  new  delete

NOT OVERLOADABLE (to protect core language semantics):
::  .  .*  ?:  sizeof  typeid  alignof  static_cast  ...
```

---

## 5.7 Inheritance

```
┌──────────────────────────────────────────────────────────────────┐
│                    INHERITANCE HIERARCHY                          │
│                                                                    │
│                        ┌─────────────┐                           │
│                        │   Animal    │  ← Base/Parent class      │
│                        │  +name      │                           │
│                        │  +sound()   │                           │
│                        │  +eat()     │                           │
│                        └──────┬──────┘                           │
│                        ┌──────┴──────┐                           │
│               ┌────────┴─┐         ┌─┴────────┐                 │
│               │   Dog    │         │   Cat    │  ← Derived       │
│               │  +breed  │         │  +indoor │                  │
│               │  +fetch()│         │  +purr() │                  │
│               └──────────┘         └──────────┘                  │
│                                                                    │
│  Dog IS-A Animal (inherits name, sound, eat)                     │
│  Dog HAS additional: breed, fetch()                              │
└──────────────────────────────────────────────────────────────────┘
```

```cpp
class Animal {
protected:           // Accessible to derived classes (not public)
    std::string name;
    int age;

public:
    Animal(const std::string& n, int a) : name{n}, age{a} {}
    
    virtual ~Animal() {}  // ALWAYS make destructor virtual if using polymorphism!

    virtual void speak() const {  // 'virtual': can be overridden
        std::cout << name << " makes a sound\n";
    }

    void eat() const {  // Non-virtual: cannot be overridden meaningfully
        std::cout << name << " eats\n";
    }

    const std::string& get_name() const { return name; }
};

class Dog : public Animal {  // Dog inherits from Animal
    std::string breed;

public:
    Dog(const std::string& n, int a, const std::string& b)
        : Animal{n, a}, breed{b} {}  // Call base constructor first!

    // Override virtual function
    void speak() const override {  // 'override': compiler checks this IS an override
        std::cout << name << " barks!\n";
    }

    void fetch() const {
        std::cout << name << " fetches the ball!\n";
    }
};

class Cat : public Animal {
public:
    Cat(const std::string& n, int a) : Animal{n, a} {}

    void speak() const override {
        std::cout << name << " meows!\n";
    }
};
```

---

## 5.8 Polymorphism — The Magic of `virtual`

```cpp
int main() {
    // STATIC BINDING (at compile time):
    Dog dog("Rex", 3, "Labrador");
    dog.speak();   // Calls Dog::speak() — no surprise

    // DYNAMIC BINDING (at runtime): THIS is polymorphism!
    Animal* ptr;
    
    ptr = &dog;
    ptr->speak();  // Calls Dog::speak()! Even through Animal* pointer

    Cat cat("Whiskers", 5);
    ptr = &cat;
    ptr->speak();  // Calls Cat::speak()! 

    // Using a vector of base class pointers (classic polymorphism):
    std::vector<std::unique_ptr<Animal>> zoo;
    zoo.push_back(std::make_unique<Dog>("Rex", 3, "Lab"));
    zoo.push_back(std::make_unique<Cat>("Whiskers", 5));
    zoo.push_back(std::make_unique<Dog>("Buddy", 2, "Poodle"));

    for (const auto& animal : zoo) {
        animal->speak();  // Correct function called for each type!
    }
    // Rex barks!
    // Whiskers meows!
    // Buddy barks!
}
```

### How Virtual Functions Work — The VTable

```
┌──────────────────────────────────────────────────────────────────────┐
│                    VTABLE MECHANISM                                    │
│                                                                        │
│  Each class with virtual functions has a VTABLE (virtual table):      │
│  An array of function pointers, one per virtual function.            │
│                                                                        │
│  Animal vtable:         Dog vtable:          Cat vtable:             │
│  ┌──────────────┐       ┌──────────────┐     ┌──────────────┐       │
│  │ Animal::speak│       │ Dog::speak   │     │ Cat::speak   │       │
│  │ Animal::~dtor│       │ Animal::~dtor│     │ Animal::~dtor│       │
│  └──────────────┘       └──────────────┘     └──────────────┘       │
│                                                                        │
│  Each OBJECT contains a hidden pointer (vptr) to its class's vtable: │
│                                                                        │
│  Dog object (Rex):         Memory layout:                            │
│  ┌──────────────────┐      ┌────────────┐                            │
│  │ vptr ────────────┼─────→│ Dog vtable │                            │
│  │ name = "Rex"     │      └────────────┘                            │
│  │ age = 3          │                                                 │
│  │ breed = "Lab"    │                                                 │
│  └──────────────────┘                                                 │
│                                                                        │
│  When you call ptr->speak():                                          │
│  1. Load vptr from the object                                         │
│  2. Look up 'speak' in the vtable                                    │
│  3. Call the function at that address                                │
│  This is ONE extra indirection — the cost of virtual dispatch.       │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 5.9 Abstract Classes and Pure Virtual Functions

```cpp
// Pure virtual: '= 0' — the class is ABSTRACT (cannot be instantiated)
class Shape {
public:
    virtual ~Shape() = default;

    virtual double area() const = 0;     // Pure virtual — MUST override
    virtual double perimeter() const = 0;// Pure virtual

    // Can have non-pure virtual with a default implementation:
    virtual void print() const {
        std::cout << "Area: " << area() << "\n";
    }

    // Can have non-virtual members too:
    void scale(double factor) { /* common scaling logic */ }
};

// Shape s;  // ERROR: Shape is abstract!

class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius{r} {}
    double area() const override { return 3.14159 * radius * radius; }
    double perimeter() const override { return 2 * 3.14159 * radius; }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width{w}, height{h} {}
    double area() const override { return width * height; }
    double perimeter() const override { return 2 * (width + height); }
};

// Polymorphic usage:
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(5.0));
shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));

for (const auto& s : shapes) {
    s->print();  // Calls correct area() and perimeter()
}
```

---

## 5.10 Access Specifiers and Inheritance Modes

```
┌──────────────────────────────────────────────────────────────────────┐
│               ACCESS CONTROL MATRIX                                   │
│                                                                        │
│  Member is:       private    protected    public                      │
│  ─────────────────────────────────────────────────                   │
│  Same class:        YES         YES          YES                      │
│  Derived class:      NO         YES          YES                      │
│  Unrelated class:    NO          NO           YES                     │
│  Friend:            YES         YES          YES                      │
│                                                                        │
│  Inheritance mode effect on inherited members:                        │
│                                                                        │
│                public inherit  protected inherit  private inherit     │
│  public    →      public          protected         private           │
│  protected →      protected       protected         private           │
│  private   →   (inaccessible)  (inaccessible)    (inaccessible)     │
│                                                                        │
│  Use 'public' inheritance 99% of the time.                           │
│  'private' inheritance = "implemented in terms of" (prefer           │
│  composition instead).                                                │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 5.11 `final` and `override` (C++11)

```cpp
class Base {
public:
    virtual void foo();
    virtual void bar() final;  // Cannot be overridden in derived classes
};

class Derived : public Base {
public:
    void foo() override;  // 'override': compiler verifies this overrides
    // void bar() override;  // ERROR: bar is final!
    // void fooo() override; // ERROR: no virtual 'fooo' in Base!
                             // Without 'override', this would silently create
                             // a NEW function (a common bug!)
};

class Leaf final : public Derived {  // Leaf cannot be further inherited
};

// class Child : public Leaf {};  // ERROR: Leaf is final
```

---

## 5.12 Multiple Inheritance and the Diamond Problem

```cpp
class A {
public:
    int value = 0;
    void foo() { std::cout << "A::foo\n"; }
};

class B : public A {};
class C : public A {};

// Diamond problem: D inherits A twice!
class D : public B, public C {};

D d;
// d.value = 5;  // AMBIGUOUS! Which A's value? B's or C's?
d.B::value = 5;  // Must qualify!
d.C::value = 10;

// SOLUTION: Virtual inheritance
class B2 : virtual public A {};
class C2 : virtual public A {};
class D2 : public B2, public C2 {};  // Only ONE copy of A!

D2 d2;
d2.value = 5;  // No ambiguity — only one A!
```

---

## 5.13 Enums and Enum Classes

```cpp
// Old-style enum (avoid — names pollute the enclosing scope)
enum Color { RED, GREEN, BLUE };  // RED, GREEN, BLUE in global scope
int RED = 5;  // ERROR: conflict!

// Modern: enum class (scoped enumerations — ALWAYS USE THIS)
enum class Direction { NORTH, SOUTH, EAST, WEST };
Direction d = Direction::NORTH;  // Must use scope operator
// Direction::NORTH == 0;  // Must explicit cast to get int

// Underlying type (default: int):
enum class Status : uint8_t { OK = 0, ERROR = 1, TIMEOUT = 2 };
static_assert(sizeof(Status) == 1, "Status should be 1 byte");

// Conversion:
int n = static_cast<int>(Direction::EAST);  // n = 2
Direction dir = static_cast<Direction>(1);  // dir = SOUTH

// using enum (C++20) — bring enum values into scope:
{
    using enum Direction;
    Direction d = NORTH;  // No need for Direction:: prefix here
}
```

---

## 5.14 `friend` Functions and Classes

```cpp
class Temperature {
    double celsius;

public:
    Temperature(double c) : celsius{c} {}
    
    // friend function: can access private members
    friend std::ostream& operator<<(std::ostream& os, const Temperature& t) {
        return os << t.celsius << "°C";  // Access private celsius
    }

    friend class Thermometer;  // Thermometer class has full access
};

class Thermometer {
public:
    void calibrate(Temperature& t) {
        t.celsius += 0.5;  // Can access private member!
    }
};
```

---

## 5.15 Static Members

```cpp
class Counter {
    static int total_count;  // Shared across ALL instances (not per-object)
    int id;

public:
    Counter() : id{++total_count} {}
    ~Counter() { --total_count; }

    static int get_total() { return total_count; }  // Static function
    int get_id() const { return id; }
};

// Definition (must be in exactly ONE .cpp file):
int Counter::total_count = 0;

int main() {
    std::cout << Counter::get_total() << "\n";  // 0

    Counter a, b, c;
    std::cout << Counter::get_total() << "\n";  // 3
    std::cout << a.get_id() << "\n";            // 1
    std::cout << b.get_id() << "\n";            // 2
    std::cout << c.get_id() << "\n";            // 3
}
```

---

## 5.16 `dynamic_cast` — Safe Runtime Downcasting

```cpp
Animal* ptr = new Dog("Rex", 3, "Lab");

// Downcast: convert Animal* → Dog*
Dog* dog = dynamic_cast<Dog*>(ptr);
if (dog != nullptr) {
    dog->fetch();  // Safe! It IS a Dog
}

Cat* cat = dynamic_cast<Cat*>(ptr);
if (cat == nullptr) {
    std::cout << "Not a cat!\n";  // Correct
}

// With references (throws std::bad_cast on failure):
try {
    Dog& dog_ref = dynamic_cast<Dog&>(*ptr);
    dog_ref.fetch();
} catch (const std::bad_cast& e) {
    std::cerr << "Not a Dog!\n";
}

// REQUIREMENT: Base class must have at least one virtual function (for RTTI)
// dynamic_cast uses RTTI (Run-Time Type Information)
```

---

## 📝 Module 05 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| Class vs Struct | Only difference: default access (private vs public) |
| Constructors | Use member initializer list; make destructors virtual |
| Big Six | If you define any, define all (or define none) |
| Deep vs Shallow Copy | Always deep copy when class owns heap memory |
| virtual | Enables runtime polymorphism through vtable |
| override / final | Always use override; prevents silent bugs |
| Abstract class | Has `= 0` pure virtual function; cannot instantiate |
| Operator overloading | Give your types natural syntax |
| enum class | Always prefer over plain enum (scoping) |
| dynamic_cast | Safe runtime downcasting; requires virtual |

---

## ➡️ Next: [Module 06: Memory Management](../module_06_memory_management/06_memory.md)
