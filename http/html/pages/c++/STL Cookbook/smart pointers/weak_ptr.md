### std::weak_ptr

Weak pointers provide us a way to point at an object maintained by shared pointers without incrementing its use counter. \
Okay, a raw pointer could do the same, but a raw pointer cannot tell us if it is dangling or not. \
A weak pointer can!

When all *shared_ptr* are destroyed - use count == 0, which leads to object to be destroyed.\
But, as *weak_ptr* still needs the control block - control block ***stays*** there.\
Only when the *last* *weak_ptr* that still points to a control block *also* goes out of scope, the control blokc is deleted.

We can also say that a dangling weak pointer has *expired*. \
In order to check for this attribute, we can ask *weak_ptr* pointer's *expired* method, which returns a boolean value. \
If it is *true*, then we cannot dereference the weak pointer because there is no object to dereference any longer.

In order to dereference a weak pointer, we need to call *lock()*. \
This is safe and convenient because this function returns us a shared pointer. \
As long as we hold this shared pointer, the object behind it cannot vanish because we incremented the use counter by locking it. \
If the object is deleted, shortly before the *lock()* call, then the shared pointer it returns is effectively a *null* pointer.

```cpp
#include <iostream>
#include <iomanip>
#include <memory>

using namespace std;

struct Foo {
    int value;

    Foo(int i) : value{i} {}
    ~Foo() { cout << "DTOR Foo " << value << '\n'; }
};

void weak_ptr_info(const weak_ptr<Foo> &p)
{
    cout << "---------" << boolalpha
         << "\nexpired:   " << p.expired()
         << "\nuse_count: " << p.use_count()
         << "\ncontent:   ";

    if (const auto sp (p.lock()); sp) {
        cout << sp->value << '\n';
    } else {
        cout << "<null>\n";
    }
}


int main()
{
    weak_ptr<Foo> weak_foo;

    weak_ptr_info(weak_foo);

    {
        auto shared_foo (make_shared<Foo>(1337));
        weak_foo = shared_foo;

        weak_ptr_info(weak_foo);
    }

    weak_ptr_info(weak_foo);
}
```

Output:
```bash
$ ./weak_ptr 
---------
expired:   true
use_count: 0
content:   <null>
---------
expired:   false
use_count: 1
content:   1337
DTOR Foo 1337
---------
expired:   true
use_count: 0
content:   <null>
```

Notes:
- In order to access the actual object via *weak_ptr* we need to call *lock* function, \
which would return *shared_ptr* to the object.\ 
In case the object does not exist any longer, the shared pointer we got from it is effectively a *null* pointer. \
We need to check that, and then we can access it:
```cpp
          if (const auto sp (p.lock()); sp) {
              cout << sp->value << '\n';
          } else {
              cout << "<null>\n";
          }
      }
```