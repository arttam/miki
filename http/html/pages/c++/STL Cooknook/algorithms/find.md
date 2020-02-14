### Finding items in ordered and unorered vactors

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>

using namespace std;

struct city {
    string   name;
    unsigned population;
};

bool operator==(const city &a, const city &b) {
    return a.name == b.name && a.population == b.population;
}

ostream& operator<<(ostream &os, const city &city) {
    return os << "{" << city.name << ", " << city.population << "}";
}

template <typename C>
static auto opt_print (const C &container)
{
    return [end_it (end(container))] (const auto &item) {
        if (item != end_it) {
            cout << *item << '\n';
        } else {
            cout << "<end>\n";
        }
    };
}

int main()
{
    const vector<city> c {
        {"Aachen",        246000},
        {"Berlin",       3502000},
        {"Braunschweig",  251000},
        {"Cologne",      1060000}
    };

    auto print_city (opt_print(c));

    {
        auto found_cologne (find(begin(c), end(c), city{"Cologne", 1060000}));
        print_city(found_cologne);
    }

    {
        auto found_cologne (find_if(begin(c), end(c), [] (const auto &item) {
            return item.name == "Cologne";
        }));
        print_city(found_cologne);
    }

    {
        auto population_more_than ([](unsigned i) {
            return [=] (const city &item) { return item.population > i; };
        });

        auto found_large (find_if(begin(c), end(c), population_more_than(2000000)));
        print_city(found_large);
    }

    const vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto print_int (opt_print(v));

    bool contains_7 {binary_search(begin(v), end(v), 7)};
    cout << contains_7 << '\n';

    auto [lower_it, upper_it] (equal_range(begin(v), end(v), 7));
    print_int(lower_it);
    print_int(upper_it);

    print_int(lower_bound(begin(v), end(v), 7));
    print_int(upper_bound(begin(v), end(v), 7));
}
```

```bash
$ ./finding_items 
{Cologne, 1060000}
{Cologne, 1060000}
{Berlin, 3502000}
1
7
8
7
8
```

**find** is linear algorithm and have a runtime complexity of *O(n)*. \
The STL also has binary search functions, which work within *O(log(n))*

The **std::binary_search** function returns boolean values and just tells us if it found an item, but it does not return the item itself. \
It is important that the container we are searching in is sorted.\
In order to get the items we are searching for, we need other STL functions. One of them is **std::equal_range**. \
It does not return an iterator for the item we found, but a pair of iterators. \
The first iterator points to the first item that is *not smaller* than the value we've been looking for. \
The second iterator points to the first item that is *larger* than it.

If we just need one iterator; we can use **std::lower_bound** or **std::upper_bound**.

- **std::find** Accepts a search range and a comparison value as arguments. Returns an iterator that points to the first item equal to the comparison value. Searches linearly.
- **std::find_if** Works like std::find but uses a predicate function instead of a comparison value.
- **std::binary_search** Accepts a search range and a comparison value as arguments. Performs a binary search and returns true if the range contains that value.
- **std::lower_bound** Accepts a search range and a comparison value, and then performs a binary search for the first item that is not smaller than the comparison value. Returns an iterator pointing to that item.
- **std::upper_bound** Works like std::lower_bound but returns an iterator to the first item that is larger than the comparison value. 
- **std::equal_range** Accepts a search range and a comparison value and, then, returns a pair of iterators. The first iterator is the result of std::lower_bound and the second iterator is the result of std::upper_bound.

Note that data structures such as std::map,  std::set, and so on have their ownfind functions. \
These are, of course, faster than the more general algorithms because they are tightly coupled with the data structure's implementation and data representation.


