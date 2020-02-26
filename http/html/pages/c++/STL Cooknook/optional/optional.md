### Optional

The *bool optional::has_value()* function checks whether optional has value or not. \
If it returns *true*, we may access the value. \
Accessing the value of an optional can be done with *T& optional::value()*.

Instead of always writing *if (x.has_value()) {...}* and *x.value()*, \
we can also write *if (x) {...}* and *\*x*. The *std::optional* type defines explicit conversion to *bool* and *operator\** in such a way that dealing with an optional type is similar to dealing with a pointer.

Another handy operator helper that is good to know is the *operator->* overload of optional. \
If we have a struct *Foo { int a; string b; }* type and want to access one of its members through an *optional<Foo>* variable, *x*, \
then we can write *x->a* or *x->b*. \
Of course, we should first check whether x actually has a value.

If we try to access an optional value even though it does not have a value, then it will throw *std::logic_error*.

```cpp
#include <iostream>
#include <optional>

using namespace std;

using oint = optional<int>;

oint read_int()
{
    int i;
    if (cin >> i) { return {i}; }
    return {};
}

oint operator+(oint a, oint b)
{
    if (!a || !b) { return {}; }

    return {*a + *b};
}

oint operator+(oint a, int b)
{
    if (!a) { return {}; }

    return {*a + b};
}

int main()
{
    cout << "Please enter 2 integers.\n> ";

    auto a {read_int()};
    auto b {read_int()};

    auto sum (a + b + 10);

    if (sum) {
        cout << *a << " + " << *b << " + 10 = "
             << *sum << '\n';
    } else {
        cout << "sorry, the input was "
                "something else than 2 numbers.\n";
    }

}
```
