### Recursive iteration with grep simulation

***recursive_directory_iterator*** has some interesting member functions:

- **depth()**: This tells us how many levels the iterator has currently descended down into subdirectories.
- **recursion_pending()**: This tells us if the iterator is going to descend down after the element it currently points to.
- **disable_recursion_pending()**: This can be called to keep the iterator from descending into the next subdirectory if it is currently pointing to a directory into which it would descend. \
This means that calling this method has no effect if we call it too early.
- **pop()**: This aborts the current recursion level and goes one level up in the directory hierarchy to continue from there.

Another thing to know about is the *directory_options* enum class. \
The constructor of *recursive_directory_iterator* does indeed accept a value of this type as a second argument. \
The default value which we have been implicitly using is *directory_options::none*. \
The other values are:

- ***follow_directory_symlink***: This allows the recursive iterator to follow symbolic links to directories
- ***skip_permission_denied***: This tells the iterator to skip directories that would otherwise result in errors because permission to access is denied by the filesystem

These options can be combined with the | operator.

```cpp
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

static vector<pair<size_t, string>> matches(const fs::path &p, const regex &re)
{
	vector<pair<size_t, string>> d;
	ifstream                     is{p.c_str()};

	string s;
	for (size_t line{1}; getline(is, s); ++line) {
		if (regex_search(begin(s), end(s), re)) {
			d.emplace_back(line, move(s));
		}
	}

	return d;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <pattern>\n";
		return 1;
	}

	regex pattern;

	try {
		pattern = regex{argv[1]};
	}
	catch (const regex_error &e) {
		cout << "Invalid regular expression provided.\n";
		return 1;
	}

	for (const auto &entry : fs::recursive_directory_iterator{fs::current_path()}) {
		auto ms(matches(entry.path(), pattern));

		for (const auto &[number, content] : ms) {
			cout << entry.path().c_str() << ":" << number << " - " << content << '\n';
		}
	}
}
```

Output:
```bash
$ ./bin/grepper recursive_directory_iterator
/Users/arturstamanis/Sources/Books/C++/C++ 17 Cookbook/CodeSamples/Chapter10/grepper.cpp:43 - 	for (const auto &entry : fs::recursive_directory_iterator{fs::current_path()}) {
/Users/arturstamanis/Sources/Books/C++/C++ 17 Cookbook/CodeSamples/Chapter10/dupe_compress.cpp:32 -     for (const auto &entry : recursive_directory_iterator{dir}) {
/Users/arturstamanis/Sources/Books/C++/C++ 17 Cookbook/CodeSamples/Chapter10/file_type.cpp:25 -     for (const auto &entry : recursive_directory_iterator{dir}) {
```
Without binary matches of course ;-)
