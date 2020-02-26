### Applying functions on tuples

The *std::apply* is a compile-time helper that helps us work more agnostic about the types we handle in our code.

Imagine we have a tuple *t* with the values *(123, "abc"s, 456.0)*. \
This tuple has the type, *tuple<int, string, double>*. \
Additionally, assume that we have a function *f* with the signature *int f(int, string, double)* (the types can also be references).

Then, we can write *x = apply(f, t)*, which will result in a function call, *x = f(123, "abc"s, 456.0)*. \
The apply method does even return to us what *f* returns.

```cpp
#include <iostream>
#include <iomanip>
#include <tuple>
#include <functional>
#include <string>
#include <list>

using namespace std;

static void print_student(size_t id, const string &name, double gpa)
{
    cout << "Student " << quoted(name) << ", ID: "
         << id << ", GPA: " << gpa << '\n';
}

int main()
{
    using student = tuple<size_t, string, double>;
    student john {123, "John Doe"s, 3.7};

    {
        const auto &[id, name, gpa] = john;
        print_student(id, name, gpa);
    }
    cout << "-----\n";

    auto arguments_for_later = {
        make_tuple(234, "John Doe"s, 3.7),
        make_tuple(345, "Billy Foo"s, 4.0),
        make_tuple(456, "Cathy Bar"s, 3.5),
    };

    for (const auto &[id, name, gpa] : arguments_for_later) {
        print_student(id, name, gpa);
    }
    cout << "-----\n";

    apply(print_student, john);
    cout << "-----\n";

    for (const auto &args : arguments_for_later) {
        apply(print_student, args);
    }
    cout << "-----\n";
}
```
