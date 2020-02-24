### stable_partition

Reorders the elements in the range *[first, last)* in such a way that all elements for which the predicate *p* returns **true** \
precede the elements for which predicate *p* returns **false**. \
Relative order of the elements is preserved.

```cpp
#include <iostream>
#include <algorithm>
#include <vector>
 
int main()
{
    std::vector<int> v{0, 0, 3, 0, 2, 4, 5, 0, 7};
    std::stable_partition(v.begin(), v.end(), [](int n){return n>0;});
    for (int n : v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}
```
Output:
```bash

3 2 4 5 7 0 0 0 0
```

