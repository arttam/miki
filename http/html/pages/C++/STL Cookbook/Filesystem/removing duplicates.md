### Removing duplicates by creating symlinks

#### Symlinks gotcha

When using *create_symlink* or *create_hard_link*. The following lines contain a bug. Can you spot it immediately?
```cpp
path a {"some_dir/some_file.txt"};
path b {"other_dir/other_file.txt"};
remove(b);
create_symlink(a, b);
```
Nothing bad happens when executing this program, but the symlink will be broken. \
The symlink points to "some_dir/some_file.txt", which is wrong. \
The problem is that it should really either point to "/absolute/path/some_dir/some_file.txt", or "../some_dir/some_file.txt". \
The create_symlink call uses a correct absolute path if we write it as follows:
```cpp
create_symlink(absolute(a), b);
```
*create_symlink* **__does not check__** whether the path we are linking to is correct.

```cpp
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <filesystem>

using namespace std;
using namespace filesystem;

using hash_map = unordered_map<size_t, path>;

static size_t hash_from_path(const path &p)
{
    ifstream is {p.c_str(), ios::in | ios::binary};
    if (!is) { throw errno; }

    string s;

    is.seekg(0, ios::end);
    s.reserve(is.tellg());
    is.seekg(0, ios::beg);

    s.assign(istreambuf_iterator<char>{is}, {});

    return hash<string>{}(s);
}

static size_t reduce_dupes(const path &dir)
{
    hash_map m;
    size_t count {0};

    for (const auto &entry : recursive_directory_iterator{dir}) {
        const path        p  {entry.path()};

        if (is_directory(p)) { continue; }

        const auto &[it, success] = m.try_emplace(hash_from_path(p), p);

        if (!success) {
            cout << "Removed " << p.c_str()
                 << " because it is a duplicate of "
                 << it->second.c_str() << '\n';

            remove(p);
            create_symlink(absolute(it->second), p);
            ++count;
        }
    }

    return count;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <path>\n";
        return 1;
    }

    path dir {argv[1]};

    if (!exists(dir)) {
        cout << "Path " << dir << " does not exist.\n";
        return 1;
    }

    const size_t dupes {reduce_dupes(dir)};

    cout << "Removed " << dupes << " duplicates.\n";
}
```