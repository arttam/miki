### if constexpr

The new constexpr-if works exactly like usual if-else constructs. \
The difference is that the condition that it tests has to be evaluated at ***compile*** time. \
All runtime code that the compiler creates from our program will not contain any branch instructions from constexpr-if conditionals. \
One could also put it that it works in a similar manner to preprocessor *#if* and *#else* text substitution macros, but for those, \
the code would ****not**** even have to be ***syntactically well-formed***. \
All the branches of a constexpr-if construct ****need**** to be ***syntactically well-formed***, but the branches that are not taken do not need to be semantically valid.

In order to distinguish whether the code should add the value *x* to a vector or not, we use the type trait *std::is_same*. \
An expression *std::is_same<A, B>::value* evaluates to the Boolean value true if *A* and *B* are of the same type. \
The condition used in our recipe is *std::is_same<T, std::vector<U>>::value*, which evaluates to true if the user specialized the \
class on *T = std::vector<X>* and tries to call add with a parameter of type *U* = *X*.

```cpp
#include <cassert>
#include <string>
#include <vector>
#include <cstdio>

template <typename T>
class addable
{
    T val;

public:
    addable(T v) : val{v} {}

    template <typename U>
    T add(U x) const {
        if constexpr (std::is_same_v<T, std::vector<U>>) {
            auto copy = val;
            for (auto &n : copy) {
                n += x;
            }
            return copy;
        } else {
            return val + x;
        }
    }
};

int main()
{
    using namespace std::string_literals;

    assert(addable<int>{2}.add(3) == 5);
    assert(addable<std::string>{"aa"s}.add("bb"s) == "aabb"s);

    std::vector<int> v1 {1, 2, 3};
    std::vector<int> v2 {11, 12, 13};
    assert(addable<std::vector<int>>{v1}.add(10) == v2);

    std::vector<std::string> sv1 {"a", "b", "c"};
    std::vector<std::string> sv2 {"az", "bz", "cz"};
    assert(addable<std::vector<std::string>>{sv1}.add("z"s) == sv2);
}
```

Note:\
Example code uses *std::is_same_v<T, std::vector<U>>* which is same as *std::is_same<T, std::vector<U>>::value* 

#### before C++17

```cpp
template <typename T>
class addable
{
    T val;

public:
    addable(T v) : val{v} {}

    template <typename U>
    std::enable_if_t<!std::is_same<T, std::vector<U>>::value, T>
    add(U x) const { return val + x; }

    template <typename U>
    std::enable_if_t<std::is_same<T, std::vector<U>>::value, 
                     std::vector<U>>
    add(U x) const {
        auto copy (val);
        for (auto &n : copy) { 
            n += x;
        }
        return copy;
    }
};
```
Without using constexpr-if, this class works for all different types we wished for, but it looks super complicated. \
How does it work?

The implementations alone of the two different add functions look simple. \
It's their return type declaration, which makes them look complicated, and which contains a trick--an expression such as \
*std::enable_if_t<condition, type>* evaluates to type if condition is true. \
Otherwise, the *std::enable_if_t* expression does not evaluate to anything. \
That would normally considered an error, but we will see why it is not.

For the second add function, the same condition is used in an *inverted* manner. \
This way, it can only be true at the same time for one of the two implementations.

When the compiler sees different template functions with the same name and has to choose one of them, an important principle comes into play: \
**SFINAE**, which stands for **Substitution Failure is not an Error**. \
In this case, this means that the compiler does not error out if the return value of one of those functions cannot be deduced from \
an erroneous template expression (which *std::enable_if* is, in case its condition evaluates to false). \
It will simply look ***further*** and try the other function implementation. \
That is the trick; that is how this works.

What a hassle. It is nice to see that this became so much easier with C++17.
