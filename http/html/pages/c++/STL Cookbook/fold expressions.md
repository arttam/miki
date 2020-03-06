### Fold Expressions

```cpp
template<typename ... Ts>
auto sum(Ts ... ts)
{
    return (ts + ...);
}

int the_sum {sum(1, 2, 3, 4, 5)};   // Value 15

std::string a{"Hello"};
std::string b{"World"};
std::cout << sum(a, b) << '\n'; // Output: HelloWorld

```
What we just did was a simple recursive application of a binary operator (+) to its parameters. \
This is generally called *folding*. \
C++17 comes with fold expressions, which help expressing the same idea with less code.

This kind of expression is called *unary fold*. \
C++17 supports folding parameter packs with the following binary operators: 
```
+, -, *, /, %, ^, &, |, =, <, >, <<, >>, +=, -=, *=, /=, %=, ^=, &=, |=, <<=, >>=, ==, !=, <=, >=, &&, ||, ,, .*, ->*.
```

If the … dots are on the *right-hand* side of the operator, the fold is called a *right* fold. \
If they are on the *left-hand* side, it is a left *fold*.

In our sum example, a unary *left* fold expands to *1 + (2 + (3 + (4 + 5)))*, \
while a unary *right* fold will expand to *(((1 + 2) + 3) + 4) + 5*. \
Depending on the operator in use, this can make a difference. 

In case someone calls *sum()* with no arguments, the variadic parameter pack contains no values that could be folded. \
For most operators, this is an erro. \
We then need to decide if this should stay an error or if an empty sum should result in a specific value. \
The obvious idea is that the sum of nothing is 0.

This is how it’s done:
```cpp
template <typename ... Ts>
auto sum(Ts ... ts)
{
    return (ts + ... + 0);
}
```
This way, *sum()* evaluates to 0, and *sum(1, 2, 3)* evaluates to *(1 + (2 + (3 + 0)))*. \
Such folds with an initial value are called **binary folds**.

Again, it works if we write *(ts + ... + 0)*, or *(0 + ... + ts)*, but this makes the binary fold a *binary right* fold or a *binary left* fold again. 

When using binary folds in order to implement the no-argument case, the notion of an *identity* element is often important - \
in this case, adding a *0* to any number changes nothing, which makes *0* an *identity* element.

The same principle applies to multiplication. \
Here, the identity element is 1:
```cpp
template <typename ... Ts>
auto product(Ts ... ts)
{
    return (ts * ... * 1);
}
```
The result of *product(2, 3)* is *6*, and the result of *product()* without parameters is *1*.

The logical *and* (&&) and *or* (||) operators come with *built-in* identity elements. \
Folding an empty parameter pack with *&&* results in **true**, and folding an empty parameter pack with *||* results in **false**.

Another operator that defaults to a certain expression when applied on empty parameter packs is the comma operator *(,)*, \
which then defaults to *void()*.

#### Match ranges
```cpp
template <typename R, typename ... Ts>
auto matches(const R& range, Ts ... ts)
{
    return (std::count(std::begin(range), std::end(range), ts) + ...);
}

...

“std::vector<int> v {1, 2, 3, 4, 5};

matches(v,         2, 5);          // returns 2
matches(v,         100, 200);      // returns 0
matches("abcdefg", 'x', 'y', 'z'); // returns 0
matches("abcdefg", 'a', 'd', 'f'); // returns 3
```
In our fold expression, we always feed the begin and end iterators of the same parameter range into the *std::count* function. \
However, as the third parameter, each time we put one other parameter from the parameter pack into it. \
In the end, the function sums up all the results and returns it to the caller.

#### Check multiple set insertions

```cpp
template <typename T, typename ... Ts>
bool insert_all(T &set, Ts ... ts)
{
    return (set.insert(ts).second && ...);
}

...

std::set<int> my_set {1, 2, 3};

insert_all(my_set, 4, 5, 6); // Returns true
insert_all(my_set, 7, 8, 2); // Returns false, because the 2 collides
```

Note that if we try to insert, for example, three elements, but the second element can already not be inserted, \
the *&& ...* fold will short-circuit and stop inserting all the other elements:

```cpp
std::set<int> my_set {1, 2, 3};

insert_all(my_set, 4, 2, 5); // Returns false
// set contains {1, 2, 3, 4} now, without the 5!
```

#### Check if all parameters are within cernain range

```cpp
template <typename T, typename ... Ts>
bool within(T min, T max, Ts ...ts)
{
    return ((min <= ts && ts <= max) && ...);
}

...

within( 10,  20,  1, 15, 30);    // --> false
within( 10,  20,  11, 12, 13);   // --> true
within(5.0, 5.5,  5.1, 5.2, 5.3) // --> true

std::string aaa {"aaa"};
std::string bcd {"bcd"};
std::string def {"def"};
std::string zzz {"zzz"};

within(aaa, zzz,  bcd, def); // --> true
within(aaa, def,  bcd, zzz); // --> false 
```
The expression, *(min <= ts && ts <= max)*, does tell for every value of the parameter pack if it is between min and max (including min and max). \
We choose the **&&** operator to reduce all the Boolean results to a *single* one, which is only true if all the individual results are true.

#### Pushing multiple items into a vector

```cpp
template <typename T, typename ... Ts>
void insert_all(std::vector<T> &vec, Ts ... ts)
{
    (vec.push_back(ts), ...);
}

int main()
{
    std::vector<int> v {1, 2, 3};
    insert_all(v, 4, 5, 6);
}
```
Note that we use the *comma (,)* operator in order to expand the parameter pack into individual *vec.push_back(...)* calls without folding the actual result. \
This function also works nicely with an empty parameter pack because the comma operator has an implicit identity element, *void()*, which translates to *do nothing*.

#### Examples all-in-one

```cpp
#include <stdio.h>
#include <string>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <set>
#include <cassert>

// C++11 style recursive sum function
template <typename T>
auto sum_rec(T t)
{
    return t;
}

template <typename T, typename ... Ts>
auto sum_rec(T t, Ts ... ts)
{
    return t + sum_rec(ts...);
}

// C++17 style fold expression
template <typename ... Ts>
auto sum(Ts ... ts)
{
    return (0 + ... + ts);
}

// C++17 style fold expression with constexpr if addition to make it more generic
template <typename ... Ts>
auto sum_generic(Ts ... ts)
{
    if constexpr ((std::is_same<Ts, const char*>::value && ...)) {
        // Works with const char* parameters
        return (std::string{} + ... + std::string{ts});
    } else if constexpr ((std::is_same<Ts, std::string>::value && ...)) {
        // Works with std::string parameters
        return (std::string{} + ... + ts);
    } else {
        // Works with integral/floating point types
        return (0 + ... + ts);
    }
}

template <typename ... Ts>
auto logical_or(Ts ... ts)
{
    return (... || ts);
}


template <typename ... Ts>
auto bitwise_or(Ts ... ts)
{
    return (0 | ... | ts);
}

template <typename T, typename ... Ts>
bool within(T min, T max, Ts ...ts)
{
    return ((min <= ts && ts <= max) && ...);
}

template <typename C, typename ... Ts>
auto matches(const C &a, Ts ... ts)
{
    return (std::count(std::begin(a), std::end(a), ts) + ...);
}

template <typename C, typename ... Ts>
bool insert_all(C &s, Ts ... ts)
{
    return (s.insert(ts).second && ...);
}

int main() {
    assert( sum_rec(1, 2, 3, 4, 5)  == 15);
    assert( sum(1, 2, 3, 4, 5)      == 15);
    assert( sum(1, 2, 3, 4, 5, 6.5) == 21.5);

    std::string hello {"Hello"};
    std::string world {" world"};

    assert( sum_generic(hello, world) == "Hello world");
    assert( sum_generic(std::string("a"), std::string("b"), std::string("c")) == "abc");
    assert( sum_generic("a", "b", "c") == "abc");

    assert( logical_or()            == false);
    assert( logical_or(false, true) == true);

    assert( bitwise_or(1, 2, 4) == 7);
    assert( bitwise_or() == 0);

    assert(within(0, 9, 1, 2, 3, 4, 5)  == true);
    assert(within(0, 9, 1, 2, 3, 4, 20) == false);

    std::string aaa {"aaa"};
    std::string bcd {"bcd"};
    std::string def {"def"};
    std::string zzz {"zzz"};

    assert( within(aaa, zzz, bcd, def));
    assert(!within(aaa, def, bcd, zzz));


    std::set<int> s;
    assert( insert_all(s, 1, 2, 3)    == true);
    assert( insert_all(s, 4, 5, 6, 6) == false);

    assert( 1 == matches(std::vector<int>{1, 2, 3, 4}, 2, 5) );
    assert( 1 == matches(std::initializer_list<int>{1, 2, 3, 4}, 10, 2) );
    assert( 1 == matches("abcdefg", 'x', 'y', 'z', 'd') );
    assert( 3 == matches("abcdefg", 'a', 'c', 'e', 'z') );
}
```