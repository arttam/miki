### Generate permutqtions

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

int main()
{
```
*Potential* way to populate container from std::cin
```cpp	
    vector<string> v {istream_iterator<string>{cin}, {}};
    sort(begin(v), end(v));

    do {
        copy(begin(v), end(v), ostream_iterator<string>{cout, ", "});
        cout << '\n';
    } while (next_permutation(begin(v), end(v)));
}
```
```bash
echo "a b X" | ./bin/input_permutations
X, a, b,
X, b, a,
a, X, b,
a, b, X,
b, X, a,
b, a, X,
```
The **std::next_permutation** algorithm is a bit weird to use. \
This is because it accepts only a begin/end pair of iterators and then returns true if it is able to find the next permutation. \
Otherwise, it returns false. But what does the next permutation even mean?

The algorithm with which **std::next_permutation** finds the next lexicographical order of the items, works as follows:

1. Find the largest index i such that v[i - 1] < v[i]. If there is none, then return false.
1. Now, find the largest index j such that j >= i and v[j] > v[i - 1].
1. Swap the items at position j and position i - 1.
1. Reverse the order of the items from position i to the end of the range.
1. Return true.

The individually permuted orders we get out of this will always appear in the same sequence. \
In order to see all the possible permutations, we sorted the array first, \
because if we entered "c b a", for example, the algorithm would terminate immediately, as this already is the last lexicographic order of the elements.
