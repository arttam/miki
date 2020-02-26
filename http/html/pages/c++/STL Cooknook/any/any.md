### std::any

The *std::any* type is similar in one regard to *std::optional* - it has a *has_value()* method that tells if an instance carries a value or not. \
But apart from that, it can contain literally anything, so it is more complex to handle compared with optional.

Before accessing the content of an any variable, we need to find out what type it carries and, then, cast it to that type.

Finding out if an any instance holds a type *T* value can be done with a comparison: *x.type() == typeid(T)*. \
If this comparison results in *true*, then we can use any_cast to get at the content.

Note that *any_cast<T>(x)* returns a copy of the internal T value in x. \
If we want a reference in order to avoid copying of complex objects, we need to use *any_cast<T&>(x)*.

```cpp
#include <iostream>
#include <iomanip>
#include <list>
#include <any>
#include <iterator>

using namespace std;

using int_list = list<int>;

void print_anything(const std::any &a)
{
    if (!a.has_value()) {
        cout << "Nothing.\n";
    } else if (a.type() == typeid(string)) {
        cout << "It's a string: "
             << quoted(any_cast<const string&>(a)) << '\n';
    } else if (a.type() == typeid(int)) {
        cout << "It's an integer: "
             << any_cast<int>(a) << '\n';
    } else if (a.type() == typeid(int_list)) {
        const auto &l (any_cast<const int_list&>(a));

        cout << "It's a list: ";
        copy(begin(l), end(l), ostream_iterator<int>{cout, ", "});
        cout << '\n';
    } else {
        cout << "Can't handle this item.\n";
    }
}


int main()
{
    print_anything({});
    print_anything("abc"s);
    print_anything(123);
    print_anything(int_list{1, 2, 3});
    print_anything(any(in_place_type_t<int_list>{}, {1, 2, 3}));
}
```
Notes:\
If we are going to put objects that are really expensive to copy into an any variable, we can also perform an **in-place construction**. \
The *in_place_type_t<int_list>{}* expression is an empty object that gives the constructor of *any* enough information to know what we are going to construct. \
The second parameter, *{1, 2, 3}*, is just an initializer list that will be fed to the *int_list* embedded in the any variable for construction.