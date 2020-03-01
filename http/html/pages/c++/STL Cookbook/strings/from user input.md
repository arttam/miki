### Strings input

```cpp
#include <iostream>

using namespace std;

int main()
{
    cout << "Please Enter two numbers:\n> ";
    int    x;
    double y;
    if (cin >> x >> y) {
        cout << "You entered: " << x << " and " << y << '\n';
    } else {
        cout << "Oh no, that did not go well!\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    cout << "now please enter some comma-separated names:\n> ";

    for (string s; getline(cin >> ws, s, ',');) {
        if (s.empty()) { break; }
        cout << "name: \"" << s << "\"\n";
    }
}
```

Note: clearing stream on error: 
```cpp
	cin.clear(); 
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
```
After error cin is in *fail* state.
*cin.clear()* puts it back to working state, but internal cursor still on errored input.\
In order to drop errored input we clearing input by calling *cin.ignore*.\
Which would stop either on max of streamsize or first occurrance of '\n'.
