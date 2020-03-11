### Remove consequtive spaces ( std::unique)

```cpp
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

template <typename It>
It remove_multi_whitespace(It it, It end_it)
{
    return unique(it, end_it, [](const auto &a, const auto &b) {
        return isspace(a) && isspace(b);
    });
}

int main()
{
    string s {"fooo     bar    \t   baz"};

    cout << s << '\n';

    s.erase(remove_multi_whitespace(begin(s), end(s)), end(s));

    cout << s << '\n';

    return 0;
}
```
How does this interesting combination work in detail? \
Let's have a look at a possible implementation of *std::unique* first:

```cpp
template<typename It, typename P>
It unique(It it, It end, P p)
{
    if (it == end) { return end; }

    It result {it};
    while (++it != end) {
        if (!p(*result, *it) && ++result != it) {
            *result = std::move(*it);
        }
    }
    return ++result;
}
```

The loop steps over the range items, while they do not satisfy the predicate condition. \
At the point where an item satisfies the predicate, it moves such an item one item past the old position, \
where the predicate fired the last time. \
The version of *std::unique* that does not accept an additional predicate function checks whether two neighbor items are equal. \
This way, it wipes out repeated characters as it can , for example, transform "abbbbbbc" to "abc".
