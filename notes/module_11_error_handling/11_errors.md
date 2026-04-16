# Module 11: Error Handling & Exceptions

> *"Good error handling is the difference between software that fails gracefully and software that corrupts data silently."*

---

## 11.1 The Three Approaches to Error Handling

```
┌──────────────────────────────────────────────────────────────────────┐
│              C++ ERROR HANDLING APPROACHES                            │
│                                                                        │
│  1. RETURN CODES (C-style)                                           │
│     int result = operation();                                        │
│     if (result < 0) { /* handle error */ }                           │
│     + Fast; no overhead                                              │
│     - Easy to ignore; error type info limited; verbose               │
│                                                                        │
│  2. EXCEPTIONS                                                        │
│     try { operation(); } catch (const Error& e) { /* handle */ }    │
│     + Can't be silently ignored; rich error info; clean code         │
│     - Overhead (small, in error path); not for all environments      │
│                                                                        │
│  3. std::expected / std::optional (C++23 / modern pattern)          │
│     auto result = operation();                                       │
│     if (!result) { /* handle error */ }                              │
│     + Type-safe; can't ignore; zero-overhead; functional             │
│     - Requires C++23 or a library                                    │
│                                                                        │
│  RULE: Use exceptions for exceptional conditions.                    │
│  Use return codes/optional/expected for expected failures.           │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 11.2 Exceptions — Throw, Try, Catch

```cpp
#include <stdexcept>

// THROW an exception:
void divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Cannot divide by zero");
    }
    return a / b;
}

// CATCH an exception:
try {
    int result = divide(10, 0);
} catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << "\n";  // what() returns the message
} catch (const std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << "\n";
} catch (const std::exception& e) {
    // Catch ALL standard exceptions (base class)
    std::cerr << "Some error: " << e.what() << "\n";
} catch (...) {
    // Catch ANY exception (including non-std exceptions)
    std::cerr << "Unknown error!\n";
}

// Rethrow:
try {
    risky_operation();
} catch (const std::exception& e) {
    log_error(e.what());
    throw;   // Rethrow the SAME exception (not throw e; which copies!)
}

// Stack unwinding: when exception is thrown, destructors run for all
// objects on the stack between throw and catch points
// This is WHY RAII works perfectly with exceptions!
```

---

## 11.3 Standard Exception Hierarchy

```
┌──────────────────────────────────────────────────────────────────────┐
│             std::exception HIERARCHY                                  │
│                                                                        │
│  std::exception (base class)                                         │
│  ├── std::logic_error         (programmer errors, could be prevented)│
│  │   ├── std::invalid_argument  (bad argument)                       │
│  │   ├── std::domain_error      (math domain error)                  │
│  │   ├── std::length_error      (exceeds max size)                   │
│  │   ├── std::out_of_range      (index out of range)                 │
│  │   └── std::future_error      (future/promise errors)              │
│  │                                                                     │
│  ├── std::runtime_error       (errors detectable only at runtime)    │
│  │   ├── std::range_error       (computation out of range)           │
│  │   ├── std::overflow_error    (arithmetic overflow)                │
│  │   ├── std::underflow_error   (arithmetic underflow)               │
│  │   └── std::system_error      (OS-level errors)                    │
│  │                                                                     │
│  ├── std::bad_alloc           (new failed to allocate)               │
│  ├── std::bad_cast            (dynamic_cast failed)                  │
│  ├── std::bad_typeid          (typeid on null pointer)               │
│  └── std::bad_exception       (unexpected exception)                 │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 11.4 Custom Exception Classes

```cpp
// Best practice: derive from std::exception or its derivatives
class DatabaseError : public std::runtime_error {
    int error_code;
    std::string query;

public:
    DatabaseError(int code, std::string q, const std::string& msg)
        : std::runtime_error(msg), error_code{code}, query{std::move(q)} {}

    int get_error_code() const { return error_code; }
    const std::string& get_query() const { return query; }
};

class ConnectionError : public DatabaseError {
public:
    ConnectionError(const std::string& host)
        : DatabaseError(1001, "", "Failed to connect to " + host) {}
};

class QueryError : public DatabaseError {
public:
    QueryError(int code, std::string q, const std::string& msg)
        : DatabaseError(code, std::move(q), msg) {}
};

// Usage:
try {
    execute_query("SELECT * FROM users");
} catch (const QueryError& e) {
    std::cerr << "Query failed [" << e.get_error_code() << "]: "
              << e.what() << "\n";
    std::cerr << "Query was: " << e.get_query() << "\n";
} catch (const DatabaseError& e) {
    std::cerr << "DB error: " << e.what() << "\n";
}
```

---

## 11.5 Exception Safety Guarantees

```
┌──────────────────────────────────────────────────────────────────────┐
│            EXCEPTION SAFETY LEVELS                                    │
│                                                                        │
│  NO GUARANTEE                                                         │
│  If exception is thrown: program state is UNKNOWN                    │
│  Memory may leak, data may corrupt → Never write code like this!     │
│                                                                        │
│  BASIC GUARANTEE (minimum acceptable)                                 │
│  If exception is thrown: no resources are leaked, invariants hold    │
│  But the state may have changed (partial operation)                   │
│                                                                        │
│  STRONG GUARANTEE ("commit or rollback")                             │
│  If exception is thrown: state is EXACTLY as before the operation    │
│  The operation either fully succeeds or has NO effect                │
│                                                                        │
│  NOTHROW GUARANTEE                                                    │
│  The operation NEVER throws an exception                              │
│  Marked with noexcept. Destructors, moves, swap should be nothrow.  │
└──────────────────────────────────────────────────────────────────────┘
```

```cpp
// Providing the STRONG guarantee with copy-and-swap idiom:
class DataContainer {
    std::vector<int> data;
public:
    // Strong guarantee: either completes fully, or no change
    DataContainer& operator=(const DataContainer& other) {
        DataContainer temp(other);     // Make copy (can throw — no effect yet)
        std::swap(data, temp.data);    // Swap (noexcept — cannot fail)
        return *this;
        // If copy throws: temp dies, *this unchanged → strong guarantee!
    }
};

// NOEXCEPT functions:
void swap_safe(std::vector<int>& a, std::vector<int>& b) noexcept {
    a.swap(b);  // vector::swap is noexcept
}

// noexcept specifier can be conditional:
template<typename T>
void my_swap(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                   std::is_nothrow_move_assignable_v<T>) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}
```

---

## 11.6 `std::expected` (C++23) — Error Handling Without Exceptions

```cpp
#include <expected>  // C++23

// Return either a value OR an error — no exceptions needed!
std::expected<int, std::string> safe_divide(int a, int b) {
    if (b == 0) {
        return std::unexpected("Division by zero");  // Error case
    }
    return a / b;  // Success case
}

// Usage:
auto result = safe_divide(10, 2);
if (result) {
    std::cout << "Result: " << *result << "\n";  // 5
} else {
    std::cerr << "Error: " << result.error() << "\n";
}

// Chaining with value_or:
int val = safe_divide(10, 0).value_or(0);  // 0 (fallback)

// Monadic operations (C++23):
auto result2 = safe_divide(10, 2)
    .and_then([](int x) -> std::expected<int, std::string> {
        return x * 2;  // Transform the value if successful
    })
    .transform([](int x) { return x + 1; })  // Like map
    .value_or(0);

// Custom error type:
enum class ParseError { InvalidFormat, Overflow, EmptyInput };

std::expected<int, ParseError> parse_int(std::string_view s) {
    if (s.empty()) return std::unexpected(ParseError::EmptyInput);
    // ... parsing logic ...
    return 42;
}
```

---

## 11.7 Error Codes — `std::error_code`

```cpp
#include <system_error>

// Low-level, OS-compatible error reporting:
std::error_code ec;
std::filesystem::create_directory("/tmp/test", ec);
if (ec) {
    std::cerr << "Error: " << ec.message() << "\n";
    std::cerr << "Value: " << ec.value() << "\n";
    std::cerr << "Category: " << ec.category().name() << "\n";
}

// Create custom error categories:
enum class NetworkError { ConnectionRefused = 1, Timeout = 2, HostNotFound = 3 };

class NetworkErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override { return "network"; }
    std::string message(int code) const override {
        switch (static_cast<NetworkError>(code)) {
            case NetworkError::ConnectionRefused: return "Connection refused";
            case NetworkError::Timeout:           return "Connection timed out";
            case NetworkError::HostNotFound:      return "Host not found";
            default:                              return "Unknown network error";
        }
    }
};

const NetworkErrorCategory& network_error_category() {
    static NetworkErrorCategory instance;
    return instance;
}

std::error_code make_error_code(NetworkError e) {
    return {static_cast<int>(e), network_error_category()};
}

// Usage:
std::error_code err = make_error_code(NetworkError::Timeout);
std::cerr << err.message();  // "Connection timed out"
```

---

## 11.8 Exceptions vs Error Codes: When to Use Which

```
┌──────────────────────────────────────────────────────────────────────┐
│            EXCEPTIONS vs ERROR CODES DECISION GUIDE                  │
│                                                                        │
│  USE EXCEPTIONS WHEN:                                                 │
│  ✓ Error is truly exceptional (rare, unexpected)                    │
│  ✓ Error must not be ignorable                                       │
│  ✓ Error propagates up many layers                                   │
│  ✓ Constructor fails (constructors can't return values)              │
│  ✓ Rich error context needed                                         │
│  Examples: file not found, memory allocation, type mismatch         │
│                                                                        │
│  USE ERROR CODES / std::expected WHEN:                               │
│  ✓ Error is an expected, common outcome                              │
│  ✓ In performance-critical code paths                                │
│  ✓ Code runs with -fno-exceptions (embedded systems, games)          │
│  ✓ Interoperating with C code                                        │
│  ✓ Function returns frequently-failing status                        │
│  Examples: parse attempt, connection attempt, cache miss             │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 11.9 `noexcept` Specification

```cpp
// noexcept: this function promises never to throw
void always_safe() noexcept {
    // If an exception DOES escape: std::terminate() is called!
}

// noexcept(expr): conditionally noexcept
template<typename T>
void swap(T& a, T& b) noexcept(noexcept(T(std::move(a)))) {
    // noexcept if T's move constructor is noexcept
}

// Check if expression is noexcept:
static_assert(noexcept(std::vector<int>{}.size()));   // true
static_assert(!noexcept(std::vector<int>{}.at(0)));   // false (can throw out_of_range)

// WHERE noexcept MATTERS MOST:
// 1. Move constructors/assignments (vector reallocation uses copies if not noexcept!)
// 2. Destructors (should NEVER throw — undefined behavior if they do!)
// 3. swap functions
// 4. operator[] (vs at() for bounds checking)

// Destructors are implicitly noexcept by default:
class Widget {
    ~Widget() {  // Implicitly noexcept!
        cleanup();  // If cleanup() throws: std::terminate()!
    }
};

// If you need a throwing destructor (unusual): must opt in:
class Unusual {
    ~Unusual() noexcept(false) { /* can throw */ }
};
```

---

## 11.10 RAII and Exception Safety Together

```cpp
// RAII makes exception safety automatic!

class Transaction {
    Database& db;
    bool committed = false;

public:
    explicit Transaction(Database& db) : db{db} {
        db.begin_transaction();
    }

    ~Transaction() {
        if (!committed) {
            db.rollback();  // Automatic rollback if not committed!
        }
    }

    void commit() {
        db.commit();
        committed = true;
    }
};

void transfer_money(Database& db, int from, int to, int amount) {
    Transaction txn(db);  // Start transaction

    db.debit(from, amount);    // If this throws: ~Transaction() rolls back
    db.credit(to, amount);     // If this throws: ~Transaction() rolls back

    txn.commit();  // Only if both succeed
    // ~Transaction() called: committed=true, no rollback
}
// RAII + exceptions = automatic cleanup in all code paths!
```

---

## 📝 Module 11 — Summary

| Concept | Key Takeaway |
|---------|-------------|
| Exceptions | For exceptional, unexpected, unignorable errors |
| `throw` | Throw any type (prefer std::exception derivatives) |
| `try`/`catch` | Structured error handling with stack unwinding |
| Exception hierarchy | Derive from `std::runtime_error` or `std::logic_error` |
| Basic guarantee | No leaks, invariants hold, state may change |
| Strong guarantee | All-or-nothing; state unchanged on failure |
| Nothrow guarantee | Never throws; mark with `noexcept` |
| `noexcept` | Critical for moves, destructors, swaps |
| `std::expected` | Modern error return (C++23); value or error |
| RAII + exceptions | Destructors run = automatic cleanup |

---

## ➡️ Next: [Module 12: Performance & Modern C++ Patterns](../module_12_performance/12_performance.md)
