### Counting directory size by recursive function

```cpp
#include <iostream>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <filesystem>

using namespace std;
using namespace filesystem;

static size_t entry_size(const directory_entry &entry)
{
    if (!is_directory(entry)) { return file_size(entry); }

    return accumulate(directory_iterator{entry}, {}, 0u,
        [](size_t accum, const directory_entry &e) {
            return accum + entry_size(e);
        });
}

static string size_string(size_t size)
{
    stringstream ss;
    if      (size >= 1000000000) { ss << (size / 1000000000) << 'G'; }
    else if (size >= 1000000)    { ss << (size / 1000000)    << 'M'; }
    else if (size >= 1000)       { ss << (size / 1000)       << 'K'; }
    else { ss << size << 'B'; }

    return ss.str();
}

int main(int argc, char *argv[])
{
    path dir {argc > 1 ? argv[1] : "."};

    if (!exists(dir)) {
        cout << "Path " << dir << " does not exist.\n";
        return 1;
    }

    for (const auto &entry : directory_iterator{dir}) {
        cout << setw(5) << right << size_string(entry_size(entry))
             << " " << entry.path().filename().c_str() << '\n';
    }
}
```

Note:
- ***entry_size*** - recursive size calculation example
- This implementation would crash under the following conditions, because of unhandled exceptions:
  - *file_size* only works on regular files and symbolic links. \
    It throws an exception in any other case.
  - Although *file_size* works on symbolic links, it still throws an exception if we call it on a broken symbolic link.