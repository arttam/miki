### Using std::unordered_map with custom types

If we use *std::unordered_map* instead of *std::map*, we have a different degree of freedom for the choice of the key type which shall be used. \
*std::map* demands that there is a natural order between all key items. \
This way, items can be sorted. But what if we want, for example, mathematical vectors as a key type? \
There is no meaning in a *smaller <* relation for such types, as a *vector (0, 1)* is not smaller or larger than *(1, 0)*. They just point in different directions. \
This is completely fine for *std::unordered_map* because it will not distinguish items via their *smaller/greater* ordering relationship but via ***hash*** values. \
The only thing we need to do is to implement a *hash function* for our own type, and an *equal to ==* operator implementation, which tells whether two objects are identical.

Usually, when we instantiate a hash-based map implementation like *std::unordered_map*, we write:
```cpp
std::unordered_map<key_type, value_type> my_unordered_map;
```

It is not too obvious that there happens a lot of magic in the background when the compiler creates our *std::unordered_map* specialization. \
So, let's have a look at the complete template-type definition of it:

```cpp
template<
    class Key,
    class T,
    class Hash      = std::hash<Key>,
    class KeyEqual  = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >
> class unordered_map;
```
The STL already contains *std::hash* specializations for a lot of types such as *std::hash&lt;std::string>*, *std::hash&lt;int>*, *std::hash&lt;unique_ptr>*, and many more.

In custom case we would create *another* specialization, the compiler will go through the list of all *std::hash* specializations until it finds custom implementation for custom type.

```cpp
#include <iostream>
#include <unordered_map>

struct coord {
    int x;
    int y;
};

bool operator==(const coord &l, const coord &r)
{
    return l.x == r.x && l.y == r.y;
}

namespace std
{

template <>
struct hash<coord>
{
    using argument_type = coord;
    using result_type   = size_t;

    result_type operator()(const argument_type &c) const
    {
        return static_cast<result_type>(c.x)
             + static_cast<result_type>(c.y);
    }
};

}

int main()
{
    std::unordered_map<coord, int> m {{{0, 0}, 1}, {{0, 1}, 2}, {{2, 1}, 3}};

    for (const auto & [key, value] : m) {
        std::cout << "{(" << key.x << ", " << key.y << "): " << value << "} ";
    }
    std::cout << "\n";
}

```
