### Own STL algorithm

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <list>

using namespace std;

template <typename InIt, typename OutIt, typename T, typename F>
InIt split(InIt it, InIt end_it, OutIt out_it, T split_val, F bin_func)
{
    while (it != end_it) {
        auto slice_end (find(it, end_it, split_val));
        *out_it++ = bin_func(it, slice_end);

        if (slice_end == end_it) { return end_it; }
        it = next(slice_end);
    }
    return it;
}

int main()
{
    const string s {"a-b-c-d-e-f-g"};

    auto binfunc ([](auto it_a, auto it_b) {
        return string(it_a, it_b);
    });

    list<string> l;

    split(begin(s), end(s), back_inserter(l), '-', binfunc);

    copy(begin(l), end(l), ostream_iterator<string>{cout, "\n"});

    return 0;
}
```
Output:
```bash
./bin/split
a
b
c
d
e
f
g
```

The loop demands to iterate until the end of the input range. \
During each iteration, an *std::find* call is used to find the next element in the input range, which equals to *split_val*. \
The next dash position is now saved in *slice_end*. After the loop iteration, the *it* iterator is put on the next item past that split position. \
This way, the loop jumps directly from dash to dash, instead of over every individual item.

In this constellation, the iterator *it* points to the beginning of the last slice, while *slice_end* points to the end of the last slice. \
Both these iterators, in combination, mark the beginning and end of the subrange that represents exactly one slice between two dash symbols. \
In a string, "foo-bar-baz", this would mean that we have three loop iterations and we get a pair of iterators every time, which surround one word. \
But we do not actually want iterators but substrings. \
The binary function, *bin_func*, does just that for us. When we called split, we gave it the following binary function:

```cpp
[](auto it_a, auto it_b) {
    return string(it_a, it_b);
}
```

The split function throws every pair of iterators through *bin_func*, before feeding it into the output iterator. \
And we actually get string instances out of *bin_func*, which results in "foo", "bar", and "baz".