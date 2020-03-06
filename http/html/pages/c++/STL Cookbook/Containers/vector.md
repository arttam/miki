### std::vector - erase_remove_idiom

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3, 2, 5, 2, 6, 2, 4, 8};

    {
        const auto new_end (remove(begin(v), end(v), 2));
        v.erase(new_end, end(v));
    }

    for (auto i : v) {
        cout << i << ", ";
    }
    cout << '\n';

    {
        const auto odd ([](int i) { return i % 2 != 0; });
        const auto new_end (remove_if(begin(v), end(v), odd));
        v.erase(new_end, end(v));
    }

    v.shrink_to_fit();

    for (auto i : v) {
        cout << i << ", ";
    }
    cout << '\n';
}
```

Note that the vector did more magic than just moving an internal pointer. \
If that vector was a vector of more **complicated** objects, it would have called all the **destructors** of the to-be-removed items.

### Deleting items from an unsorted ***std::vector in O(1)*** time

Deleting items from somewhere in the middle of an *std::vector* takes *O(n)* time. \
This is because the resulting gap from removing an item must be filled by moving all the items which come after the gap one slot to the left.

While moving items around like this, which might be expensive if they are complex and/or very large and include many items, we preserve their order. \
If preserving the order is not important, we can optimize this, as this section shows.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
void quick_remove_at(std::vector<T> &v, std::size_t idx)
{
    if (idx < v.size()) {
        v.at(idx) = std::move(v.back());
        v.pop_back();
    }
}

template <typename T>
void quick_remove_at(std::vector<T> &v, typename std::vector<T>::iterator it)
{
    if (it != std::end(v)) {
        *it = std::move(v.back());
        v.pop_back();
    }
}

int main()
{
    std::vector<int> v {123, 456, 789, 100, 200};

    quick_remove_at(v, 2);

    for (int i : v) {
        std::cout << i << ", ";
    }
    std::cout << '\n';

    quick_remove_at(v, std::find(std::begin(v), std::end(v), 123));

    for (int i : v) {
        std::cout << i << ", ";
    }
    std::cout << '\n';

    std::vector<int> vv;

    quick_remove_at(vv, 0);
}
```

The *quick_remove_at* function removes items pretty quickly without touching too many other items. \
It does this in a relatively creative way: It kind of swaps the actual item,  which shall be removed with the last item in the vector. \
Although the last item has *no connection* to the actually selected item, it is in a *special position*: Removing the last item is ***cheap***!

### Keep vector sorted

```cpp
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <set>
#include <algorithm>
#include <iterator> // for ostream_iterator
#include <cassert>

using namespace std;

template <typename C>
void print_vector(const C &v)
{
    std::cout << "Words: {";
    copy(begin(v), end(v), ostream_iterator<typename C::value_type>(cout, " "));
    std::cout << "}\n";
}

template <typename C, typename T>
void insert_sorted(C &v, const T &word)
{
    const auto it (lower_bound(begin(v), end(v), word));
    v.insert(it, word);
}

int main()
{
    list<string> v {"some", "random", "words", "without", "order", "aaa", "yyy"};

    //assert(false == is_sorted(begin(v), end(v)));

    print_vector(v);

    //sort(begin(v), end(v));

    //assert(true == is_sorted(begin(v), end(v)));

    print_vector(v);

    insert_sorted(v, "foobar");
    insert_sorted(v, "zzz");

    print_vector(v);
}

```
**std::lower_bound** function finds the first item in the range, which is *greater than or equal* to that third parameter and returns an *iterator* pointing to it.

Having the right position at hand, we gave it to the *std::vector* member method insert, which accepts just two arguments. \
The first argument is an iterator, which points to the position in the vector, at which the second parameter shall be inserted.

The insert_sorted function is pretty generic. \
If we generalize the types of its parameters, it will also work on other container payload types, and even on other containers such as *std::set*, *std::deque*, *std::list*, and so on! \
(Note that set has its own *lower_bound* member function that does the same as *std::lower_bound*, but is more efficient because it is specialized for sets.)

```cpp
template <typename C, typename T>
void insert_sorted(C &v, const T &item)
{
    const auto insert_pos (lower_bound(begin(v), end(v), item));
    v.insert(insert_pos, item);
}
```
When trying to switch the type of the vector in the recipe from *std::vector* to something else, note that not all containers support *std::sort*. \
That algorithm requires random access containers, which *std::list*, for example, does not fulfill.