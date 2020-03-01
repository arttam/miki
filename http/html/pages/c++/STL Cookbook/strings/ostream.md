### std::ostream printing

```cpp
#include <iostream>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <algorithm>

using namespace std;

string word_num(int i) {
    unordered_map<int, string> m {
        {1, "one"}, {2, "two"}, {3, "three"},
        {4, "four"}, {5, "five"}, //...
    };

    const auto match (m.find(i));
    if (match == end(m)) { return "unknown"; }
    return match->second;
};

struct bork {
    int borks;

    bork(int i) : borks{i} {}

    void print(ostream& os) const {
        fill_n(ostream_iterator<string>{os, " "}, borks, "bork!"s);
    }
};

ostream& operator<<(ostream &os, const bork &b) {
    b.print(os);
    return os;
}

int main()
{
    const vector<int> v {1, 2, 3, 4, 5};

    ostream_iterator<int> oit {cout};

    for (int i : v) { *oit = i; }
    cout << '\n';

    ostream_iterator<int> oit_comma {cout, ", "};

    for (int i : v) { *oit_comma = i; }
    cout << '\n';

    copy(begin(v), end(v), oit);
    cout << '\n';

    copy(begin(v), end(v), oit_comma);
    cout << '\n';

    transform(begin(v), end(v), ostream_iterator<string>{cout, " "}, word_num);
    cout << '\n';

    copy(begin(v), end(v), ostream_iterator<bork>{cout, "\n"});
}
```

Output:
```bash
$ ./ostream_printing 
      12345
      1, 2, 3, 4, 5, 
      12345
      1, 2, 3, 4, 5, 
      one two three four five 
      bork! 
      bork! bork! 
      bork! bork! bork! 
      bork! bork! bork! bork! 
      bork! bork! bork! bork! bork! 
```
We have seen that *std::ostream_iterator* is really just a *syntax hack*, \
which kind of squeezes the act of printing into the form and syntax of an iterator. \
Incrementing such an iterator does *nothing*. \
Dereferencing it only returns us a proxy object whose assignment operator forwards its argument to an output stream.

Output stream iterators that are specialized on a type *T* (as in *ostream_iterator<T>*) work with all types for which an *ostream& operator<<(ostream&, const T&)* implementation is provided.