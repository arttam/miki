### Filesystem path normalizer

***filesystem::canonical*** 
```cpp
path canonical(const path& p, const path& base = current_path());
```
*canonical* accepts a path and as an optional second argument, it accepts another path. \
The second path base is prepended to path *p* if *p* is a relative path. \
After doing that, canonical tries to remove any . and .. path indirections.

*canonical* throws a *filesystem_error* type exception if the path we want to canonicalize does not exist.\
Most filesystem functions have an additional overload that takes the same arguments, but also an *std::error_code* reference.

```cpp
path canonical(const path& p);
path canonical(const path& p, error_code& ec);
```

This way we can choose if we surround our filesystem function calls with *try-catch* constructs or check the errors manually.

```cpp
#include <filesystem>
#include <iostream>

using namespace std;
using namespace filesystem;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <path>\n";
		return 1;
	}

	path dir{argv[1]};

	if (!exists(dir)) {
		cout << "Path " << dir << " does not exist.\n";
		return 1;
	}

	cout << canonical(dir).c_str() << '\n';
}
```
Notes:
- ***filesystem::canonical*** returns us another path object. \
We could print it directly, but the path type overload of the *<< operator* surrounds paths with quotation marks. \
In order to avoid that, we can print a path through its *.c_str()* or *.string()* method.
```cpp
          cout << canonical(dir).c_str() << '\n';”
```

#### More path methods

```cpp
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
	fs::path p{"testdir/foobar.txt"};

	std::cout << "current_path    : " << fs::current_path() << '\n'
	          << "absolute_path   : " << fs::absolute(p) << '\n';

	std::cout << fs::path{"testdir"} / "foobar.txt" << '\n';

	std::cout << "canonical testdir   : "
	          << fs::canonical(fs::current_path() / ".." / "Chapter10" / "foobar.txt") << '\n';

	std::cout << "equivalence: "
	          << fs::equivalent("../Chapter10/foobar.txt", "bin/../../Chapter10/foobar.txt")
	          << '\n';
}
```