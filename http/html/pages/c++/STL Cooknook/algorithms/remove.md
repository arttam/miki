### Remove

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

void print(const vector<int> &v)
{
    copy(begin(v), end(v), ostream_iterator<int>{cout, ", "});
    cout << '\n';
}

int main()
{
    vector<int> v {1, 2, 3, 4, 5, 6};

    print(v);

    {
        const auto new_end (remove(begin(v), end(v), 2));
        v.erase(new_end, end(v));
    }
    print(v);

    {
        auto odd_number ([](int i) { return i % 2 != 0; });
        const auto new_end (remove_if(begin(v), end(v), odd_number));
        v.erase(new_end, end(v));
    }
    print(v);

    replace(begin(v), end(v), 4, 123);
    print(v);

    v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    vector<int> v2;
    vector<int> v3;

    auto odd_number  ([](int i) { return i % 2 != 0; });
    auto even_number ([](int i) { return i % 2 == 0; });

    remove_copy_if(begin(v), end(v), back_inserter(v2), odd_number);
    copy_if(       begin(v), end(v), back_inserter(v3), even_number);

    print(v2);
    print(v3);
}
```
```bash
$ ./removing_items_from_containers 
1, 2, 3, 4, 5, 6, 
1, 3, 4, 5, 6, 
4, 6, 
123, 6, 
2, 4, 6, 8, 10, 
2, 4, 6, 8, 10, 
```
- **std::remove** - Accepts a range and a value as arguments and removes any occurrence of the value. Returns a new end iterator of the modified range. 
- **std::replace** - Accepts a range and two values as arguments and replaces all the occurrences of the first value with the second value.
- **std::remove_copy** - Accepts a range, an output iterator, and a value as arguments and copies all the values that are not equal to the given value from the range to the output iterator.
- **std::replace_copy** - Works similar to std::replace but analogous to std::remove_copy. The source range is not altered.
- **std::copy_if** - Works like std::copy but additionally accepts a predicate function as an argument in order to copy only the values that the predicate accepts, which makes it a filter function.

For every one of the listed algorithms, there also exists an ***_if** version, which accepts a predicate function instead of a value, which then decides which values are to be removed or replaced.

