### std::unique_ptr

If we assign some new pointer to a unique pointer, then it will always first delete the old object it pointed to and then store the new pointer. \
On a unique pointer variable, *x*, we can also call *x.reset()* to just delete the object it points to immediately without assigning a new pointer. \
Another equivalent alternative to reassigning via *x = new_pointer* is *x.reset(new_pointer)*.

There is indeed one *single way* to release an object from the management of *unique_ptr* without deleting it. \
The *release* function does that, but using this function is not advisable in most situations.

Dereferencing a unique pointer can be done via the *get()* function, which returns a raw pointer to the object that can be dereferenced, \
or directly via *operator\**, which again mimics raw pointers.

One important characteristic of *unique_ptr* is that its instances cannot be copied but can be **moved** from one *unique_ptr* variable to the other.

Since there are data structures, such as *unique_ptr* and *shared_ptr*, \
there is rarely any reason to create heap objects directly with *new* and *delete* them manually. \
Use such classes wherever you can! Especially *unique_ptr* imposes **no overhead at runtime**.

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

void process_item(unique_ptr<Foo> p)
{
    if (!p) { return; }

    cout << "Processing " << p->name << '\n';
}

int main()
{
    {
        unique_ptr<Foo> p1 {new Foo{"foo"}};
        auto            p2 (make_unique<Foo>("bar"));
    }

    process_item(make_unique<Foo>("foo1"));

    auto p1 (make_unique<Foo>("foo2"));
    auto p2 (make_unique<Foo>("foo3"));

    process_item(move(p1));

    cout << "End of main()\n";
}
```

Output:
```bash
$ ./unique_ptr 
CTOR foo
CTOR bar
DTOR bar
DTOR foo
CTOR foo1
Processing foo1
DTOR foo1
CTOR foo2
CTOR foo3
Processing foo2
DTOR foo2
End of main()
DTOR foo3
```

Notes:
- objects are destroyed in the *opposite* order of their creation.
