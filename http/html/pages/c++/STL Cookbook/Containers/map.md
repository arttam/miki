### std::map - try_emplace example

```cpp
#include <iostream>
#include <functional>
#include <list>
#include <map>

using namespace std;

struct billionaire {
    string name;
    double dollars;
    string country;
};

int main()
{
    list<billionaire> billionaires {
        {"Bill Gates", 86.0, "USA"},
        {"Warren Buffet", 75.6, "USA"},
        {"Jeff Bezos", 72.8, "USA"},
        {"Amancio Ortega", 71.3, "Spain"},
        {"Mark Zuckerberg", 56.0, "USA"},
        {"Carlos Slim", 54.5, "Mexico"},
        // ...
        {"Bernard Arnault", 41.5, "France"},
        // ...
        {"Liliane Bettencourt", 39.5, "France"},
        // ...
        {"Wang Jianlin", 31.3, "China"},
        {"Li Ka-shing", 31.2, "Hong Kong"}
        // ...
    };

    map<string, pair<const billionaire, size_t>> m;

    for (const auto &b : billionaires) {
        auto [iterator, success] = m.try_emplace(b.country, b, 1);

        if (!success) {
            iterator->second.second += 1;
        }
    }


    for (const auto & [key, value] : m) {
        const auto &[b, count] = value;

        cout << b.country << " : " << count << " billionaires. Richest is "
             << b.name << " with " << b.dollars << " B$\n";
    }
}

```

Note:\
Both the *insert* and *emplace* functions of *std::map* work exactly the same way. \
A crucial difference is that ***try_emplace*** **will not construct** the object associated with the key if the key **already exists**. \
This boosts the performance in case objects of that type are expensive to create.

### Knowing the new insertion hint semantics of std::map::insert

Looking up items in an *std::map* takes *O(log(n))* time. \
This is the same for inserting new items, because the position where to insert them must be looked up. \
Naive insertion of M new items would thus take *O(M \* log(n))* time.

In order to make this more efficient, *std::map* insertion functions accept an optional insertion hint parameter. \
The insertion hint is basically an iterator, which points near the future position of the item that is to be inserted. \
If the hint is correct, then we get *amortized O(1)* insertion time.

```cpp
#include <iostream>
#include <map>
#include <string>

using namespace std;

using map_type = map<string, size_t>;

int main()
{
    map_type m {{"b", 1}, {"c", 2}, {"d", 3}};

    auto insert_it (end(m));
    for (const auto &s : {"z", "y", "x", "w", "v"}) {
        insert_it = m.insert(insert_it, {s, 1});
    }

    // In this example line, the hint is wrong.
    // It points past the last element, but the "0" string will be inserted
    // at the front. Therefore it does not have a better runtime performance
    // than insert without a hint parameter!
    m.insert(end(m), {"a", 1});

    for (const auto & [key, value] : m) {
        std::cout << "\"" << key << "\": " << value << ", ";
    }
    std::cout << '\n';
}
```
The only difference to normal map insertions in this recipe was the additional hint iterator. \
And we spoke about *correct* and *wrong* hints.

A correct hint will point to an existing element, which is *greater* than the element to be inserted so that the newly inserted key will be just *before* the hint. \
If this does not apply for the hint the user provided during an insertion, the insert function will fall back to a nonoptimized insertion, yielding *O(log(n))* performance again.

Interestingly, a wrong hint does not even destroy or disturb the order of the items in the map, so how does that even work, and what did that mean, that the insertion time is *amortized O(1)*?

The *std::map* is usually implemented using a binary search tree. \
When inserting a new key into a search tree, it is compared against the keys of the other nodes, beginning from the top. \
If the key is smaller or larger than the key of one node, then the search algorithm branches left or right to go down to the next deeper node. \
While doing that, the search algorithm will stop at the point where it reached the maximum depth of the current tree, where it will put the new node with its key. \
It is possible that this step destroyed the tree's balance, so it will also correct that using a re-balancing algorithm afterward as a housekeeping task.

When we insert items into a tree with key values which are direct neighbors of each other (just as the integer 1 is a neighbor of the integer 2, because no other integer fits between them), they can often also be inserted just next to each other in the tree, too. \
It can easily be checked if this is true for a certain key and an accompanying hint. \
And if this situation applies, the search algorithm step can be omitted, which spares some crucial runtime. \
Afterward, the re-balancing algorithm might nevertheless have to be run. \
When such an optimization can often be done, but not always, this can still lead to an average performance gain. \
It is possible to show a resulting runtime complexity which settles down after multiple insertions, and then it's called *amortized complexity*.

If the insertion hint is wrong, the insertion function will simply *waive* the hint and start over using the search algorithm. \
This works correctly but is obviously *slower*.

### Efficiently modifying the keys of std::map items

Since the *std::map* data structure maps from keys to values in a way that the keys are always unique and sorted, it is of crucial value that users cannot modify the keys of map nodes that are already inserted. \
In order to prevent the user from modifying the key items of perfectly sorted map nodes, the **const** qualifier is added to the key type.

This kind of restriction is perfectly sane because it makes it harder for the user to use *std::map* the wrong way. \
But what shall we do if we really need to change the keys of some map items?

Prior to C++17, we had to remove the items of which we need to change the key value from the tree, in order to reinsert them. \
The downside of this approach is that this always needlessly reallocates some memory, which sounds bad in terms of performance.

Since C++17, we can remove and reinsert map nodes without any reallocation of memory.

The *extract* function is a new C++17 feature. \
It removes items from a map without any allocation-related side effects.\
It comes in two flavors:
```cpp
node_type extract(const_iterator position);
node_type extract(const key_type& x);”
```

If we try to extract an item that doesn't exist with the second method (the one that searches using a key), it returns an empty node_type instance. \
The *empty()* member method returns us a Boolean value that tells whether a node_type instance is empty or not. \
Accessing any other method on an ***empty*** instance leads to **undefined behavior**.

After extracting nodes, we were able to modify their keys using the *key()* method, which gives us *nonconst* access to the key, although keys are usually *const*.

Note that in order to reinsert the nodes into the map again, we ***had to move*** them into the insert function. \
This makes sense because extract is all about avoiding unnecessary copies and allocations. \
Note that while we *move* a node_type instance, this does not result in actual moves of any of the container values.

Map nodes that have been extracted using the *extract* method are actually very versatile (flexible). \
We can extract nodes from a *map* instance and insert it into any other *map* or even *multimap* instance. \
It does also work between *unordered_map* and *unordered_multimap* instances, as well as with *set/multiset* and respective *unordered_set/unordered_multiset*.

In order to move items between different *map/set* structures, the types of key, value, and allocator need to be ***identical***. \
Note that even if that is the case, we **cannot** move nodes from a *map* to an *unordered_map*, or from a *set* to an *unordered_set*.

```cpp
#include <iostream>
#include <map>

using namespace std;

template <typename M>
void print(const M &m)
{
    cout << "Race placement:\n";
    for (const auto &[placement, driver] : m) {
        cout << placement << ": " << driver << '\n';
    }
}

int main()
{
    map<int, string> race_placement {
        {1, "Mario"}, {2, "Luigi"}, {3, "Bowser"},
        {4, "Peach"}, {5, "Yoshi"}, {6, "Koopa"},
        {7, "Toad"}, {8, "Donkey Kong Jr."}
    };

    print(race_placement);

    {
        auto a (race_placement.extract(3));
        auto b (race_placement.extract(8));

        swap(a.key(), b.key());

        race_placement.insert(move(a));
        race_placement.insert(move(b));
    }


    print(race_placement);
}
```
