### Shared members

```cpp
#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct person {
    string name;
    size_t age;

    person(string n, size_t a)
        : name{move(n)}, age{a}
    { cout << "CTOR " << name << '\n'; }

    ~person() { cout << "DTOR " << name << '\n'; }
};

int main()
{
    shared_ptr<string> shared_name;
    shared_ptr<size_t> shared_age;

    {
        auto sperson (make_shared<person>("John Doe", 30));

        shared_name = shared_ptr<string>(sperson, &sperson->name);
        shared_age  = shared_ptr<size_t>(sperson, &sperson->age);
    }


    cout << "name: "   << *shared_name
         << "\nage:  " << *shared_age << '\n';
}
```
Output:
```bash
$ ./shared_members 
CTOR John Doe
name: John Doe
age:  30
DTOR John Doe
```

Note:\
*shared_ptr1* points to the person object directly, while *shared_name* and *shared_age* point to the name and the age members of the same object.\
Apparently, they still manage the object's entire lifetime. \
This is possible because the internal control block pointers still point to the *same* control block, no matter what sub-object the individual shared pointers point to.

In this scenario, the use count of the control block is 3. \
This way, the *person* object is not destroyed when *shared_ptr1* is destroyed because the other shared pointers still own the object.

When creating such shared pointer instances that point to members of the shared object, the syntax looks a bit strange. \
In order to obtain a *shared_ptr<string>* that points to the name member of a shared person, we need to write the following:
```cpp
auto sperson (make_shared<person>("John Doe", 30));
auto sname   (shared_ptr<string>(sperson, &sperson->name)); 
```
In order to get a specific pointer to a member of a shared object, we instantiate a shared pointer with a type specialization of the member we want to access. \
This is why we write *shared_ptr<string>*. Then, in the constructor, we **first** provide the *original* shared pointer that maintains the *person* object and, as a **second** argument, the *address* of the object the new shared pointer will use when we dereference it.