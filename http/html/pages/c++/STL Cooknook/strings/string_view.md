### std::string_view

```cpp
#include <iostream>
#include <string_view>

using namespace std;

void print(string_view v)
{
    // note: do not expect a null terminated string
    const auto words_begin (v.find_first_not_of(" \t\n"));
    v.remove_prefix(min(words_begin, v.size()));

    const auto words_end   (v.find_last_not_of(" \t\n"));

    if (words_end != string_view::npos) {
        v.remove_suffix(v.size() - words_end - 1);
    }

    cout << "length: " << v.length()
         << " [" << v << "]\n";
}

int main(int argc, char *argv[])
{
    print(argv[0]);
    print({});
    print("a const char * array");
    print("an std::string_view literal"sv);
    print("an std::string instance"s);

    print("   \t\n foobar  \n  \t ");

    char cstr[] {'a', 'b', 'c'}; // no null termination
    print(string_view(cstr, sizeof(cstr)));
}
```	
Output:
```bash
$ ./string_view 
length: 17 [./string_view]
length: 0 []
length: 20 [a const char * array]
length: 27 [an std::string_view literal]
length: 23 [an std::string instance]
length: 6 [foobar]
length: 3 [abc]
```

We have just seen that we can call a function that accepts a *string_view* argument with basically anything that is string like in the sense that it stores characters in a contiguous way. \
*No copy* of the underlying string was made in any of our print calls.
