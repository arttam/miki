### Creating strings and string views

```cpp
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>

using namespace std;

int main()
{
    string a { "a"  };
    auto   b ( "b"s );

    string_view c { "c"   };
    auto        d ( "d"sv );

    cout << a << ", " << b << '\n';
    cout << c << ", " << d << '\n';

    cout << a + b << '\n';
    cout << a + string{c} << '\n';

    ostringstream o;

    o << a << " " << b << " " << c << " " << d;
    auto concatenated (o.str());

    cout << concatenated << '\n';

    transform(begin(concatenated), end(concatenated), begin(concatenated), ::toupper);
    cout << concatenated << '\n';
}
```

Note:
- Creating string with *s* suffix
- Creating string_view with *sv* suffix;
- Need of conversion to string from string_view in concatination

**std::string_view** is useful, but be *cautious* when mixing it with strings and string functions. \
We cannot assume that they are zero-terminated, which breaks things quickly in a standard string environment.

If we want to do complex string concatenation with formatting and so on, we should however not do that piece by piece on string instances. \
The *std::stringstream*, *std::ostringstream*, and *std::istringstream* classes are much better suited for this, as they enhance the memory management while appending, and provide all the formatting features we know from streams in general. \
The *std::ostringstream* class is what we chose in this section because we were going to create a string instead of parsing it. \
An *std::istringstream* instance could have been instantiated from an existing string, which we could have then comfortably parsed into variables of other types. \
If we want to combine both, *std::stringstream* is the perfect all-rounder.