### Directory iteration

Traversing a directory is fantastically simple with the filesystem library.
```cpp
for (const directory_entry &e : directory_iterator{dir}) {
    // do something
}
```
However, it might be noticeable that *directory_iterator* seems to be an *iterator*, and an iterable range at the same time. \
Why? In the minimal for loop example we just had a look at, it was used as an iterable range. \
In the actual recipe code, we used it like an iterator:
```cpp
transform(directory_iterator{dir}, {}, back_inserter(items), file_info);
```
The truth is, it is just an iterator class type, but the *std::begin* and *std::end* functions provide overloads for this type. \
This way we can call the begin and end function on this kind of iterator and they return us iterators again.


```cpp
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

static std::tuple<fs::path, fs::file_status, size_t> file_info(const fs::directory_entry &entry)
{
	const auto fs(fs::status(entry));
	return {entry.path(), fs, fs::is_regular_file(fs) ? fs::file_size(entry.path()) : 0u};
}

static char type_char(fs::file_status fs)
{
	if (fs::is_directory(fs)) {
		return 'd';
	}
	else if (fs::is_symlink(fs)) {
		return 'l';
	}
	else if (fs::is_character_file(fs)) {
		return 'c';
	}
	else if (fs::is_block_file(fs)) {
		return 'b';
	}
	else if (fs::is_fifo(fs)) {
		return 'p';
	}
	else if (fs::is_socket(fs)) {
		return 's';
	}
	else if (fs::is_other(fs)) {
		return 'o';
	}
	else if (fs::is_regular_file(fs)) {
		return 'f';
	}

	return '?';
}

static std::string rwx(fs::perms p)
{
	auto check([p](fs::perms bit, char c) {
		return (p & bit) == fs::perms::none ? '-' : c;
	});

	return {check(fs::perms::owner_read, 'r'),
	        check(fs::perms::owner_write, 'w'),
	        check(fs::perms::owner_exec, 'x'),
	        check(fs::perms::group_read, 'r'),
	        check(fs::perms::group_write, 'w'),
	        check(fs::perms::group_exec, 'x'),
	        check(fs::perms::others_read, 'r'),
	        check(fs::perms::others_write, 'w'),
	        check(fs::perms::others_exec, 'x')};
}

static std::string size_string(size_t size)
{
	std::stringstream ss;
	if (size >= 1000000000) {
		ss << (size / 1000000000) << 'G';
	}
	else if (size >= 1000000) {
		ss << (size / 1000000) << 'M';
	}
	else if (size >= 1000) {
		ss << (size / 1000) << 'K';
	}
	else {
		ss << size << 'B';
	}

	return ss.str();
}

int main(int argc, char *argv[])
{
	fs::path dir{argc > 1 ? argv[1] : "."};

	if (!fs::exists(dir)) {
		std::cout << "Path " << dir << " does not exist.\n";
		return 1;
	}

	std::vector<std::tuple<fs::path, fs::file_status, size_t>> items;

	std::transform(fs::directory_iterator{dir}, {}, std::back_inserter(items), file_info);

	for (const auto &[path, status, size] : items) {
		std::cout << type_char(status) << rwx(status.permissions()) << "  " << std::setw(4)
		          << std::right << size_string(size) << "  " << path.filename().c_str() << '\n';
	}
}
```	

Notes:
- Nice way to build string on the fly from characters:
```cpp
static std::string rwx(fs::perms p)
{
	auto check([p](fs::perms bit, char c) {
		return (p & bit) == fs::perms::none ? '-' : c;
	});

	return {check(fs::perms::owner_read, 'r'),
	        check(fs::perms::owner_write, 'w'),
	        check(fs::perms::owner_exec, 'x'),
	        check(fs::perms::group_read, 'r'),
	        check(fs::perms::group_write, 'w'),
	        check(fs::perms::group_exec, 'x'),
	        check(fs::perms::others_read, 'r'),
	        check(fs::perms::others_write, 'w'),
	        check(fs::perms::others_exec, 'x')};
}
```
- Using *transform* to generate vector of usefull information:
```cpp
	std::transform(fs::directory_iterator{dir}, {}, std::back_inserter(items), file_info);
```