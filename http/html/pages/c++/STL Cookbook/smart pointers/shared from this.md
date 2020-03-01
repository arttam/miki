### enable_shared_from_this

```cpp
#include <iostream>
#include <memory>
#include <list>

using namespace std;

class Foo : public enable_shared_from_this<Foo> {
public:
    string name;

    Foo(string n)
        : name{n}
    { cout << "CTOR " << name << '\n'; }

    ~Foo() { cout << "DTOR " << name << '\n'; }

    template <typename T>
    void add(T &container) {
        container.push_back(shared_from_this());
    }
};

int main()
{
    list<shared_ptr<Foo>> l;

    for (size_t i {1}; i < 10; ++i) {
        string s (i, 'X');
        make_shared<Foo>(s)->add(l);
    }

    cout << "Items in list: " << l.size() << '\n';
}
```
Output:
```bash
./bin/shared_from_this
CTOR X
CTOR XX
CTOR XXX
CTOR XXXX
CTOR XXXXX
CTOR XXXXXX
CTOR XXXXXXX
CTOR XXXXXXXX
CTOR XXXXXXXXX
Items in list: 9
DTOR X
DTOR XX
DTOR XXX
DTOR XXXX
DTOR XXXXX
DTOR XXXXXX
DTOR XXXXXXX
DTOR XXXXXXXX
DTOR XXXXXXXXX	
```