### Legacy C structures wrapped in smart pointers

```cpp
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Foo
{
    string name;

    Foo(string n)
        : name{n}
    { cout << "CTOR " << name << '\n'; }

    ~Foo() { cout << "DTOR " << name << '\n';}

public:
    static Foo* create_foo(string s) { return new Foo{move(s)}; }

    static void destroy_foo(Foo *p) { delete p; }
};

static shared_ptr<Foo> make_shared_foo(string s)
{
    return {Foo::create_foo(move(s)), Foo::destroy_foo};
}

static unique_ptr<Foo, void (*)(Foo*)> make_unique_foo(string s)
{
    return {Foo::create_foo(move(s)), Foo::destroy_foo};
}

int main()
{
    auto ps (make_shared_foo("shared Foo instance"));

    auto pu (make_unique_foo("unique Foo instance"));
}
```
Output:
```bash
“$ ./legacy_shared_ptr 
CTOR shared Foo instance
CTOR unique Foo instance
DTOR unique Foo instance
DTOR shared Foo instance
```
Notes:\
Note that *make_shared_foo* returns a usual *shared_ptr<Foo>* instance because giving it a custom deleter did not change its type. \
This is because *shared_ptr* uses virtual function calls to hide such details. \
Unique pointers do not impose any overhead, which makes the same trick unfeasible for them. \
Here, we need to change the type of the unique_ptr. \
As a second template parameter, we give it *void (*)(Foo*)*, which is exactly the type of pointer to the function, *destroy_foo*.

The reason why it is so simple to just provide *shared_ptr* some kind of callable deleter object without altering the shared pointer's type lies in the nature of shared pointers, which maintain a control block. \
The control block of shared pointers is an object with virtual functions. This means that the control block of a standard shared pointer compared with the type of a control block of a shared pointer with a custom deleter is *different!* \
When we want a unique pointer to use a custom deleter, then this changes the type of the unique pointer. \
When we want a shared pointer to use a custom deleter, then this changes the type of the internal *control block*, which is invisible to us because this difference is hidden behind a virtual function interface.