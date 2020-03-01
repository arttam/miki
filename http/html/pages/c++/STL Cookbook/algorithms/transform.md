### Transforming container contents

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3, 4, 5};

    transform(begin(v), end(v), ostream_iterator<int>{cout, ", "},
              [] (int i) { return i * i; });
    cout << '\n';

    auto int_to_string ([](int i) {
        stringstream ss;
         ss << i;
         ss << "^2 = ";
         ss << i * i;
         return ss.str();
    });

    vector<string> vs;

    transform(begin(v), end(v), back_inserter(vs), int_to_string);

    copy(begin(vs), end(vs), ostream_iterator<string>{cout, "\n"});
}
```

```bash
$ ./transforming_items_in_containers 
1, 4, 9, 16, 25, 
1^2 = 1
2^2 = 4
3^2 = 9
4^2 = 16
5^2 = 25
```
*Interesting*: **int_to_string** application shows how to *map* from integers to strings.
