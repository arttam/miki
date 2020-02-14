### Copying items

```cpp
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

namespace std {
ostream &operator<<(ostream &os, const pair<int, string> &p)
{
	return os << "(" << p.first << ", " << p.second << ")";
}
}  // namespace std

int main()
{
	vector<pair<int, string>> v{{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}};

	map<int, string> m;

	copy_n(begin(v), 3, inserter(m, begin(m)));

	auto shell_it(ostream_iterator<pair<int, string>>{cout, ", "});
	copy(begin(m), end(m), shell_it);
	cout << '\n';

	m.clear();

	move(begin(v), end(v), inserter(m, begin(m)));

	copy(begin(m), end(m), shell_it);
	cout << '\n';

	copy(begin(v), end(v), shell_it);
	cout << '\n';
}
```
```bash
$ ./copying_items
(1, one), (2, two), (3, three), 
(1, one), (2, two), (3, three), (4, four), (5, five), 
(1, ), (2, ), (3, ), (4, ), (5, ),
```

As std::copy is one of the simplest STL algorithms, its implementation is very short. Let's have a look at how it could be implemented:

```cpp
template <typename InputIterator, typename OutputIterator>
OutputIterator copy(InputIterator it, InputIterator end_it, 
                    OutputIterator out_it)
{
    for (; it != end_it; ++it, ++out_it) {
        *out_it = *it;
    }
    return out_it;
}
```
What is not obvious is the hidden automatic optimization of such STL algorithms. \
If we happen to use **std::copy** with data structures that store their items in contiguous memory (as **std::vector** and **std::array** do), *and* the items themselves are *trivially copy assignable*, then the compiler will select a completely different implementation (which assumes the iterator types to be pointers):

```cpp
template <typename InputIterator, typename OutputIterator>
OutputIterator copy(InputIterator it, InputIterator end_it, 
                    OutputIterator out_it)
{
    const size_t num_items (distance(it, end_it));
    memmove(out_it, it, num_items * sizeof(*it));
    return it + num_items;
}
```
This is a simplified version of how the **memmove** variant of the std::copy algorithm can look in a typical STL implementation. It is faster than the standard loop version. \
But nevertheless, **std::copy** users automatically profit from it if their argument types comply with the requirements of this optimization. \
The compiler selects the fastest implementation possible for the chosen algorithm, while the user code nicely expresses *what* the algorithm does without tainting the code with too many details of the *how*.

