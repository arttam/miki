### Sorting containers

```cpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <random>

using namespace std;

static void print(const vector<int> &v)
{
    copy(begin(v), end(v), ostream_iterator<int>{cout, ", "});
    cout << '\n';
}

int main()
{
    vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    random_device rd;
    mt19937 g {rd()};

    cout << is_sorted(begin(v), end(v)) << '\n';

    shuffle(begin(v), end(v), g);

    cout << is_sorted(begin(v), end(v)) << '\n';
    print(v);

    sort(begin(v), end(v));

    cout << is_sorted(begin(v), end(v)) << '\n';
    print(v);

    shuffle(begin(v), end(v), g);

    partition(begin(v), end(v), [](int i) { return i < 5; });

    print(v);

    shuffle(begin(v), end(v), g);

    auto middle (next(begin(v), int(v.size()) / 2));
    partial_sort(begin(v), middle, end(v));

    print(v);

    struct mystruct {
        int a;
        int b;
    };

    vector<mystruct> mv {{5, 100}, {1, 50}, {-123, 1000}, {3, 70}, {-10, 20}};

    sort(begin(mv), end(mv),
        [] (const mystruct &lhs, const mystruct &rhs) {
            return lhs.b < rhs.b;
        });

    for (const auto &[a, b] : mv) {
        cout << "{" << a << ", " << b << "} ";
    }
    cout << '\n';
}
```

```bash
$ ./sorting_containers 
1
0
7, 1, 4, 6, 8, 9, 5, 2, 3, 10, 
1
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
1, 2, 4, 3, 5, 7, 8, 10, 9, 6, 
1, 2, 3, 4, 5, 9, 8, 10, 7, 6,
{-10, 20} {1, 50} {3, 70} {5, 100} {-123, 1000}
```
We have used different algorithms, which have to do with sorting:

- **std::sort** - Accepts a range as arguments and simply sorts it.
- **std::is_sorted** - Accepts a range as argument and tells if that range is sorted.
- **std::shuffle** - This is, kind of, the reverse operation to sorting; it accepts a range as arguments and shuffles its items around.
- **std::partial_sort** - Accepts a range as arguments and another iterator, which tells until where the input range should be sorted. Behind that iterator, the rest of the items appear unsorted.
- **std::partition** - Accepts a range and a predicate function. All items for which the predicate function returns true are moved to the front of the range. The rest is moved to the back.

For objects that do not have a comparison operator < implementation, it is possible to provide custom comparison functions. These should always have a signature such as bool function_name(const T &lhs, const T &rhs) and should not have any side effects during execution.

There are also other algorithms such as std::stable_sort, which also sort but preserve the order of items with the same sort key and std::stable_partition.
