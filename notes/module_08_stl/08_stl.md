# Module 08: The Standard Library (STL)

> *"The STL is one of C++'s greatest gifts. Learning it deeply makes you 10x more productive."*

---

## 8.1 STL Architecture

```
┌──────────────────────────────────────────────────────────────────────┐
│                    STL ARCHITECTURE                                    │
│                                                                        │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐      │
│  │   CONTAINERS    │  │   ALGORITHMS    │  │   ITERATORS     │      │
│  │  (store data)   │  │ (operate on     │  │ (connect them)  │      │
│  │                 │  │  containers)    │  │                 │      │
│  │ vector  map     │  │ sort  find      │  │ begin()  end()  │      │
│  │ list    set     │  │ copy  transform │  │ ++it  *it  it-> │      │
│  │ deque   queue   │  │ reduce count_if │  │ rbegin  rend    │      │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘      │
│                                                                        │
│  ALGORITHM works on ITERATOR RANGE, not container directly:          │
│  sort(vec.begin(), vec.end());  // Not: sort(vec)                    │
│  find(v.begin(), v.end(), 42);                                       │
│                                                                        │
│  C++20 RANGES: sort(vec);  // Works directly on containers!          │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 8.2 Sequence Containers

### `std::vector` — The Go-To Container

```cpp
#include <vector>

std::vector<int> v;
v.push_back(1);          // Add to end
v.push_back(2);
v.emplace_back(3);       // Construct in-place (no copy) — prefer this
v.pop_back();            // Remove last

v.insert(v.begin(), 0); // Insert at position (O(n)!)
v.erase(v.begin());      // Remove at position (O(n)!)
v.erase(v.begin(), v.begin() + 2); // Erase range

v.size();      // Number of elements
v.capacity();  // Allocated space
v.empty();     // Is it empty?
v.clear();     // Remove all elements
v.resize(10);  // Change size
v.reserve(100); // Reserve capacity without changing size

// Element access:
v[0];          // No bounds check (UB if out of range)
v.at(0);       // With bounds check (throws std::out_of_range)
v.front();     // First element
v.back();      // Last element
v.data();      // Raw pointer to data

// Iteration:
for (int x : v) std::cout << x;     // Range-based for
for (auto it = v.begin(); it != v.end(); ++it) std::cout << *it;
for (auto it = v.rbegin(); it != v.rend(); ++it) std::cout << *it; // Reverse
```

### `std::deque` — Double-Ended Queue

```cpp
#include <deque>

std::deque<int> dq;
dq.push_back(1);    // O(1) — add to back
dq.push_front(0);   // O(1) — add to FRONT (vector can't do this efficiently!)
dq.pop_back();      // O(1)
dq.pop_front();     // O(1)
dq[2];              // O(1) random access (slightly slower than vector)

// Use deque when you need efficient push/pop at BOTH ends
// Implemented as a sequence of fixed-size buffers
```

### `std::list` — Doubly Linked List

```cpp
#include <list>

std::list<int> lst = {1, 2, 3, 4, 5};

lst.push_front(0);  // O(1)
lst.push_back(6);   // O(1)
lst.pop_front();    // O(1)
lst.pop_back();     // O(1)

// Insert/erase at iterator position: O(1)! (unlike vector's O(n))
auto it = lst.begin();
std::advance(it, 2);       // Move iterator 2 steps
lst.insert(it, 99);        // O(1) insert
lst.erase(it);             // O(1) erase

// Sorting a list (can't use std::sort — no random access!)
lst.sort();                 // Member function sort: O(n log n)
lst.sort(std::greater<int>{}); // Sort descending

lst.unique();     // Remove consecutive duplicates
lst.reverse();    // Reverse in-place
lst.merge(other); // Merge two sorted lists: O(n)

// No random access: lst[2] does NOT exist for list!
// Use only when you need many insertions/deletions in the middle
```

### `std::array` — Fixed-Size, Stack-Allocated

```cpp
#include <array>

std::array<int, 5> arr = {1, 2, 3, 4, 5};
arr[0] = 10;
arr.size();   // Always 5 (compile-time constant!)
arr.fill(0);  // Set all elements to 0

// Can be used with all algorithms:
std::sort(arr.begin(), arr.end());
std::reverse(arr.begin(), arr.end());

// Aggregate initialization:
std::array<int, 5> zeros{};  // All zeros
```

---

## 8.3 Associative Containers

### `std::map` — Sorted Key-Value Store

```cpp
#include <map>

std::map<std::string, int> ages;
ages["Alice"] = 30;
ages["Bob"] = 25;
ages.insert({"Charlie", 35});
ages.emplace("Diana", 28);

// Lookup:
ages["Alice"];          // 30 (inserts default if not found!)
ages.at("Alice");       // 30 (throws if not found — safer)
ages.find("Bob");       // Returns iterator (or end() if not found)
ages.count("Alice");    // 1 if exists, 0 if not
ages.contains("Bob");   // true (C++20)

// Iteration (SORTED by key):
for (const auto& [name, age] : ages) {   // Structured binding (C++17)
    std::cout << name << ": " << age << "\n";
}
// Alice: 30, Bob: 25, Charlie: 35, Diana: 28 (alphabetical order)

// Erase:
ages.erase("Bob");
ages.erase(ages.find("Alice"));  // Erase by iterator

// Underlying: Red-Black Tree
// All operations: O(log n)
// Elements stored in sorted order
```

### `std::unordered_map` — Hash-Based Key-Value Store

```cpp
#include <unordered_map>

std::unordered_map<std::string, int> scores;
scores["Alice"] = 100;
scores["Bob"] = 85;

// Same interface as map, but:
// - Average O(1) lookup, insert, erase (vs O(log n) for map)
// - NO ordering guarantee
// - Requires hashable key type (all primitives and std::string are)

// When to choose:
// map: need ordered iteration, or keys not hashable
// unordered_map: maximum lookup performance, don't need ordering

// Reserve to avoid rehashing:
scores.reserve(1000);  // Pre-allocate for 1000 elements
scores.max_load_factor(0.25); // Rehash when 25% full (fewer collisions)
```

### `std::set` and `std::unordered_set`

```cpp
#include <set>
#include <unordered_set>

// set: sorted unique elements
std::set<int> s = {5, 2, 8, 1, 9, 2};  // Duplicates removed!
// s = {1, 2, 5, 8, 9} (sorted)

s.insert(3);
s.erase(5);
s.find(8) != s.end();  // Check if 8 exists
s.count(8);            // 1 if exists, 0 if not
s.contains(8);         // C++20

// unordered_set: O(1) average, no ordering
std::unordered_set<std::string> words = {"hello", "world", "hello"};
// words = {"hello", "world"} (one "hello", order undefined)

// lower_bound / upper_bound (set only):
auto it = s.lower_bound(4);  // First element >= 4
auto it2 = s.upper_bound(4); // First element > 4
```

### `std::multimap` and `std::multiset`

```cpp
// Allow duplicate keys:
std::multimap<int, std::string> mm;
mm.insert({1, "one"});
mm.insert({1, "uno"});   // Duplicate key allowed!
mm.insert({2, "two"});

mm.count(1);  // 2 — number of elements with key 1

// Get all elements with key 1:
auto range = mm.equal_range(1);
for (auto it = range.first; it != range.second; ++it) {
    std::cout << it->second << "\n";  // "one", "uno"
}
```

---

## 8.4 Container Adaptors

```cpp
#include <stack>
#include <queue>
#include <priority_queue>

// stack: LIFO — adapts deque/vector
std::stack<int> stk;
stk.push(1);
stk.push(2);
stk.push(3);
stk.top();   // 3 (look without removing)
stk.pop();   // Remove 3
stk.size();  // 2
stk.empty(); // false

// queue: FIFO — adapts deque
std::queue<int> q;
q.push(1);
q.push(2);
q.push(3);
q.front();  // 1 (next to be popped)
q.back();   // 3 (most recently added)
q.pop();    // Remove 1
q.size();   // 2

// priority_queue: max-heap by default
std::priority_queue<int> pq;
pq.push(3);
pq.push(1);
pq.push(4);
pq.push(1);
pq.push(5);
pq.top();   // 5 (maximum element!)
pq.pop();   // Remove 5

// Min-heap:
std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
// Or: pq.push + negate values for min-heap behavior
```

---

## 8.5 The `<algorithm>` Header

```cpp
#include <algorithm>
#include <numeric>
#include <functional>

std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

// ─── SORTING ────────────────────────────────────────────────────────
std::sort(v.begin(), v.end());                       // Ascending
std::sort(v.begin(), v.end(), std::greater<int>{});  // Descending
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; }); // Lambda
std::stable_sort(v.begin(), v.end()); // Preserves order of equal elements
std::partial_sort(v.begin(), v.begin() + 3, v.end()); // Only sort first 3

// ─── SEARCHING ──────────────────────────────────────────────────────
std::sort(v.begin(), v.end());  // Must be sorted for binary_search!
std::binary_search(v.begin(), v.end(), 5);  // true/false
auto it = std::find(v.begin(), v.end(), 5); // Returns iterator
if (it != v.end()) std::cout << "Found: " << *it;

auto it2 = std::lower_bound(v.begin(), v.end(), 5); // First >= 5
auto it3 = std::upper_bound(v.begin(), v.end(), 5); // First > 5

std::find_if(v.begin(), v.end(), [](int x) { return x > 5; }); // First >5
std::count(v.begin(), v.end(), 5);           // Count occurrences of 5
std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }); // Even

// ─── TRANSFORMING ───────────────────────────────────────────────────
std::vector<int> result(v.size());
std::transform(v.begin(), v.end(), result.begin(),
               [](int x) { return x * 2; });  // Double each element

std::for_each(v.begin(), v.end(), [](int& x) { x *= 2; });  // In-place

std::replace(v.begin(), v.end(), 5, 99);  // Replace 5 with 99
std::replace_if(v.begin(), v.end(), [](int x){ return x < 3; }, 0); // Replace <3 with 0

std::fill(v.begin(), v.end(), 0);     // Fill with 0
std::iota(v.begin(), v.end(), 1);     // 1, 2, 3, 4, ... (from <numeric>)
std::generate(v.begin(), v.end(), rand); // Fill with random numbers

// ─── NUMERIC ────────────────────────────────────────────────────────
int sum = std::accumulate(v.begin(), v.end(), 0);        // Sum (from <numeric>)
int product = std::accumulate(v.begin(), v.end(), 1,
              [](int acc, int x) { return acc * x; });   // Product

std::vector<int> prefix_sums(v.size());
std::partial_sum(v.begin(), v.end(), prefix_sums.begin()); // Running sum

// ─── COPY / MOVE ────────────────────────────────────────────────────
std::vector<int> dest(v.size());
std::copy(v.begin(), v.end(), dest.begin());
std::copy_if(v.begin(), v.end(), dest.begin(),
             [](int x) { return x > 3; });  // Copy only if > 3

std::move(v.begin(), v.end(), dest.begin()); // Move elements

// ─── REMOVE ─────────────────────────────────────────────────────────
// "Erase-remove idiom" — the standard way to remove elements:
v.erase(std::remove(v.begin(), v.end(), 5), v.end()); // Remove all 5s

v.erase(std::remove_if(v.begin(), v.end(),
        [](int x) { return x % 2 == 0; }), v.end()); // Remove evens

// C++20: std::erase and std::erase_if (cleaner):
std::erase(v, 5);                              // Remove all 5s
std::erase_if(v, [](int x) { return x > 3; }); // Remove all > 3

// ─── SET OPERATIONS (on sorted ranges) ─────────────────────────────
std::vector<int> a = {1, 2, 3, 4, 5};
std::vector<int> b = {3, 4, 5, 6, 7};
std::vector<int> intersection, union_v, difference;

std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                      std::back_inserter(intersection)); // {3, 4, 5}
std::set_union(a.begin(), a.end(), b.begin(), b.end(),
               std::back_inserter(union_v));             // {1,2,3,4,5,6,7}
std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                    std::back_inserter(difference));     // {1, 2}

// ─── MIN/MAX ────────────────────────────────────────────────────────
auto [min_it, max_it] = std::minmax_element(v.begin(), v.end());
std::cout << *min_it << " " << *max_it;

std::max(3, 5);      // 5
std::min(3, 5);      // 3
std::clamp(15, 0, 10); // 10 (clamp to [0, 10])
```

---

## 8.6 Lambdas — Deep Dive

```cpp
// Lambda syntax: [capture](parameters) -> return_type { body }

// Basic lambda:
auto square = [](int x) { return x * x; };
std::cout << square(5);  // 25

// Capture by value (copy):
int factor = 3;
auto multiply = [factor](int x) { return x * factor; };
multiply(5);  // 15

// Capture by reference:
int count = 0;
auto increment = [&count]() { count++; };
increment(); increment(); increment();
std::cout << count;  // 3

// Capture all by value: [=]
// Capture all by reference: [&]
// Mix: [=, &x] (all by value, except x by reference)

// Generic lambda (C++14):
auto print = [](const auto& x) { std::cout << x << "\n"; };
print(42);       // Works for int
print(3.14);     // Works for double
print("hello");  // Works for string

// Mutable lambda (modify captured values):
int n = 0;
auto counter = [n]() mutable {
    return ++n;  // Modifies the COPY, not original n
};
counter(); // 1
counter(); // 2
n;         // Still 0!

// Lambda returning lambda (closure):
auto make_adder = [](int x) {
    return [x](int y) { return x + y; };
};
auto add5 = make_adder(5);
add5(3);   // 8
add5(10);  // 15

// Immediately invoked lambda:
int result = [](int a, int b) { return a + b; }(3, 4);  // 7

// Using lambda with algorithms:
std::vector<std::string> words = {"banana", "apple", "cherry", "date"};
std::sort(words.begin(), words.end(),
    [](const std::string& a, const std::string& b) {
        return a.size() < b.size();  // Sort by length
    });
// "date", "apple", "banana", "cherry"
```

---

## 8.7 C++20 Ranges

```cpp
#include <ranges>

std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Range algorithms — work directly on containers:
std::ranges::sort(v);
std::ranges::reverse(v);
std::ranges::find(v, 5);

// Views — lazy, composable, zero-copy transformations:
auto even = v | std::views::filter([](int x) { return x % 2 == 0; });
auto doubled = v | std::views::transform([](int x) { return x * 2; });
auto first5 = v | std::views::take(5);
auto last5 = v | std::views::drop(5);

// Compose multiple views:
auto result = v
    | std::views::filter([](int x) { return x % 2 == 0; })    // Keep even
    | std::views::transform([](int x) { return x * x; })        // Square
    | std::views::take(3);                                       // First 3

for (int x : result) std::cout << x;  // 4, 16, 36

// LAZY EVALUATION: views don't compute until iterated!
// No intermediate containers created

// std::views functions:
// views::filter      — keep elements matching predicate
// views::transform   — apply function to each
// views::take        — first N elements
// views::drop        — skip first N elements
// views::take_while  — take while predicate true
// views::drop_while  — skip while predicate true
// views::reverse     — reverse order
// views::keys        — keys of map
// views::values      — values of map
// views::zip         — zip two ranges (C++23)
// views::enumerate   — (index, value) pairs (C++23)
// views::iota        — integer sequence

// Generate ranges:
for (int i : std::views::iota(1, 11)) {  // 1 to 10
    std::cout << i << " ";
}
```

---

## 8.8 `std::optional`, `std::variant`, `std::any`

```cpp
#include <optional>
#include <variant>
#include <any>

// ─── std::optional: value or nothing ──────────────────────────────
std::optional<int> find_age(const std::string& name) {
    if (name == "Alice") return 30;  // Has value
    return std::nullopt;             // No value
}

auto age = find_age("Alice");
if (age.has_value()) std::cout << *age;  // 30
if (age) std::cout << *age;              // Same
age.value_or(0);  // 30 if has value, else 0

auto missing = find_age("Unknown");
// *missing;  // Throws std::bad_optional_access!
missing.value_or(0);  // 0

// ─── std::variant: type-safe union ────────────────────────────────
std::variant<int, double, std::string> v;
v = 42;           // Holds int
v = 3.14;         // Now holds double
v = "hello";      // Now holds string

std::get<int>(v);      // Throws if not int!
std::get<double>(v);   // OK (currently holds double)

std::holds_alternative<double>(v);  // true

// Pattern matching with visit:
std::visit([](auto& value) {
    std::cout << value << "\n";
}, v);

std::visit(overloaded{  // Needs a helper 'overloaded' struct
    [](int i)    { std::cout << "int: " << i; },
    [](double d) { std::cout << "double: " << d; },
    [](const std::string& s) { std::cout << "string: " << s; },
}, v);

// ─── std::any: type-erased value ──────────────────────────────────
std::any a = 42;
a = 3.14;
a = std::string("hello");

std::any_cast<std::string>(a);     // "hello"
std::any_cast<int>(a);             // Throws! Currently holds string

try {
    auto& s = std::any_cast<std::string&>(a);
    std::cout << s;
} catch (const std::bad_any_cast& e) {
    std::cerr << "Wrong type!\n";
}

a.type() == typeid(std::string);  // true
a.has_value();                    // true
a.reset();                        // Clear the value
```

---

## 8.9 `std::tuple` and Structured Bindings

```cpp
#include <tuple>

// Create a tuple:
std::tuple<int, std::string, double> t = {1, "hello", 3.14};
std::tuple<int, std::string, double> t2 = std::make_tuple(2, "world", 2.72);

// Access by index:
std::get<0>(t);   // 1
std::get<1>(t);   // "hello"
std::get<2>(t);   // 3.14

// Structured bindings (C++17) — decompose any aggregate/pair/tuple:
auto [id, name, score] = t;
std::cout << id << " " << name << " " << score;

// With map:
std::map<std::string, int> ages = {{"Alice", 30}, {"Bob", 25}};
for (const auto& [name, age] : ages) {
    std::cout << name << ": " << age << "\n";
}

// Return multiple values from function:
std::tuple<int, int, int> divide_and_remainder(int a, int b) {
    return {a / b, a % b, b};
}

auto [quotient, remainder, divisor] = divide_and_remainder(17, 5);
// quotient=3, remainder=2, divisor=5

// std::pair (2-element tuple):
std::pair<int, std::string> p = {42, "answer"};
p.first;   // 42
p.second;  // "answer"
auto [num, str] = p;  // Structured binding
```

---

## 8.10 I/O Streams

```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// Console I/O:
std::cin >> x;           // Read from stdin
std::cout << x;          // Write to stdout
std::cerr << "Error!";   // Write to stderr (unbuffered)
std::clog << "Log";      // Write to stderr (buffered)

// Format output:
std::cout << std::setw(10) << std::left << "Name" << std::setw(5) << 42 << "\n";
std::cout << std::fixed << std::setprecision(2) << 3.14159;  // 3.14
std::cout << std::hex << 255 << "\n";   // ff
std::cout << std::oct << 255 << "\n";   // 377
std::cout << std::boolalpha << true;    // "true"

// File I/O:
std::ofstream out_file("output.txt");
out_file << "Hello, file!\n";
out_file.close();

std::ifstream in_file("input.txt");
std::string line;
while (std::getline(in_file, line)) {
    std::cout << line << "\n";
}
in_file.close();

// String stream — parse/build strings:
std::stringstream ss;
ss << "Hello " << 42 << " " << 3.14;
std::string result = ss.str();  // "Hello 42 3.14"

// Parse from string:
std::stringstream parser("42 3.14 hello");
int i; double d; std::string s;
parser >> i >> d >> s;  // i=42, d=3.14, s="hello"

// C++20: std::format (much better!)
#include <format>
std::string formatted = std::format("Hello, {}! You are {} years old.", "Alice", 30);
std::cout << std::format("{:.2f}\n", 3.14159);  // 3.14
std::cout << std::format("{:>10}\n", "right");   // Right-align
```

---

## 8.11 `<chrono>` — Time

```cpp
#include <chrono>

// Get current time:
auto now = std::chrono::steady_clock::now();

// Measure elapsed time:
auto start = std::chrono::high_resolution_clock::now();

// ... some work ...

auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
std::cout << "Took " << duration.count() << " ms\n";

// Duration literals (C++14):
using namespace std::chrono_literals;
auto one_second = 1s;
auto half_second = 500ms;
auto one_minute = 1min;

std::this_thread::sleep_for(100ms);  // Sleep 100ms

// Convert between durations:
std::chrono::seconds s(60);
std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(s);
m.count();  // 1
```

---

## 📝 Module 08 — Summary

| Component | What It Does |
|-----------|-------------|
| `vector` | Dynamic array; the default container choice |
| `deque` | Double-ended queue; efficient front/back |
| `list` | Linked list; O(1) mid-insert/erase, no random access |
| `map` | Sorted key-value; O(log n); use when order matters |
| `unordered_map` | Hash map; O(1) average; use for max performance |
| `set` / `unordered_set` | Unique element collections |
| `stack`, `queue`, `priority_queue` | Container adaptors |
| `<algorithm>` | 100+ generic algorithms |
| Lambdas | Anonymous functions; power tool with algorithms |
| Ranges (C++20) | Lazy, composable operations on sequences |
| `optional` | Value or nothing; better than nullptr |
| `variant` | Type-safe tagged union |
| `tuple` | Fixed-size, typed heterogeneous collection |
| `<chrono>` | High-precision timing |

---

## ➡️ Next: [Module 09: Move Semantics & Perfect Forwarding](../module_09_move_semantics/09_move.md)
