### Containers

All containers the STL provides can be categorized as follows:

- Contiguous storage
- List storage
- Search trees
- Hash tables
- Container adapters
 #### Contiguous storage

The simplest way to store objects is to put them just next to each other in one large chunk of memory. \
Such memory can be accessed in a random access manner in **O(1)** time.

The easiest way to do that is using *std::array*, which is just a wrapper around normal C-style arrays.

The *std::vector* comes into play when array-like storage is needed, but with varying sizes. \
It uses memory from the **heap** to store objects. \
Whenever new objects are pushed into the vector exceeding its current size, it will automatically move all items to a larger chunk of newly allocated memory, and delete the old chunk.

If lots of objects are inserted/removed at the front and/back of an *std::vector* that leads to a lot of new memory allocations in order to gain space, with potentially costly object move, *std::deque* offers an interesting trade off here. \
The objects are stored in fixed-size chunks of contiguous memory, but these chunks are independent of each other. \
This makes it very simple and quick to arbitrarily grow the deque, because objects in existing chunks can stay where they are, whenever a new chunk is allocated and put at the front or the end of the container. \
Deque stands for **double-ended queue**.

#### List storage

The *std::list* is a classical doubly-linked list. Not less, and not more. \
If only uni-directional list traversal is needed, *std::forward_list* may be more performant in both space and maintenance complexity, because it maintains only list item pointers in one direction. \
Lists can only be traversed linearly with **O(n)** time. \
Inserting and removing items at specific positions can be done in **O(1)** time.

#### Search trees

Whenever objects have a natural order so that they can be sorted using some notion of the mathematical *smaller <* relation, they can be maintained in that order using search trees. \
As the name suggests, search trees can easily be searched for specific items using a search key, which allows **O(log(n))** search times.

STL Search trees are:
- std::set
- std::map
- std::multimap
- std::multiset

#### Hash tables

When talking about associative containers, search trees are not the only way to implement them. \
With hash tables, items can be found in **O(1)** time, but they ignore the natural order of the items, so they can't be easily traversed in a sorted manner. \
The size of the hash table can be manipulated by the user, and the *hash function* can also be chosen individually, which is important, because the performance versus space consumption characteristics depend on that.

***std::unordered_set*** and ***std::unordered_map*** have so much interface in common with their ***std::set*** and ***std::map*** pendants, that they can be used almost interchangeably.

Just as for the search tree implementations, both containers have their multipendants: ***std::unordered_multiset*** and ***std::unordered_multimap*** both drop the constraint on the uniqueness of the objects/keys, so we can store multiple elements with the same key.

#### Container adapters

Arrays, lists, trees, and hash tables are not the only way to store and access data, as there are also stacks, queues, and so on. \
Similar, more sophisticated structures, however, can be implemented using the more primitive ones, and the STL does that with the following ones in the form of container adapters: ***std::stack***, ***std::queue***, and ***std::priority_queue***.

Adapter can be easily switched, for example the implementation of an *std::stack* instance from *std::vector* to *std::deque*.
