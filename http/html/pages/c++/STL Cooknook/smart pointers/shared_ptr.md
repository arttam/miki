### std::shared_ptr

The major difference from *unique_ptr* is that we can copy the *shared_ptr* instances because shared pointers maintain a so-called *control block* together with the object they manage. \
The *control block* contains a pointer to the payload object and a reference counter or use counter. \
If there are *N* number of *shared_ptr* instances pointing to the object, then the use counter also has the value *N*. \
Whenever a *shared_ptr* instance is destructed, then its destructor decrements this internal use counter. \
The last shared pointer to such an object will hit the condition that it decrements the use counter to *0* during its destruction. \
This is, then, the shared pointer instance, which calls the *delete* operator on the payload object!

There is, however, one important caveat to consider - imagine we have two objects on the heap that contain shared pointers to each other, \
and some other shared pointer points to one of them from somewhere else. \
If that external shared pointer goes out of scope, then both objects still have the use counters with nonzero values because they reference each other. \
This leads to a memory leak. Shared pointers should not be used in this case because such cyclic reference chains prevent the use counter of such objects to ever reach 0.

```cpp
#include <iostream>
#include <memory>

using namespace std;

class Foo
{
public:
    string name;

    Foo(string n)
        : name{move(n)}
    { cout << "CTOR " << name << '\n'; }

    ~Foo() { cout << "DTOR " << name << '\n'; }
};

void f(shared_ptr<Foo> sp)
{
    cout << "f: use counter at " << sp.use_count() << '\n';
}

int main()
{
    shared_ptr<Foo> fa;

    {
        cout << "Inner scope begin\n";

        shared_ptr<Foo> f1 {new Foo{"foo"}};
        auto            f2 (make_shared<Foo>("bar"));

        cout << "f1's use counter at " << f1.use_count() << '\n';
        fa = f1;
        cout << "f1's use counter at " << f1.use_count() << '\n';

    }
    cout << "Back to outer scope\n";


    cout << fa.use_count() << '\n';

    cout << "first f() call\n";
    f(fa);
    cout << "second f() call\n";
    f(move(fa));
    cout << "end of main()\n";
}
```
Output:
```bash
$ ./shared_ptr
Inner scope begin
CTOR foo
CTOR bar
f1's use counter at 1
f1's use counter at 2
DTOR bar
Back to outer scope
1
first f() call
f: use counter at 2
second f() call
f: use counter at 1
DTOR foo
end of main()
```