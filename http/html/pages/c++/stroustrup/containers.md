### Containers

#### Vector
A vector is a sequence of elements of a given type. \
The elements are stored contiguously in memory. \
A typical implementation of vector will consist of a handle holding pointers to the first element, \
one-past-the-last element, and one-past-the-last allocated space.\
In addition, it holds an allocator (here, alloc), from which the vector can acquire memory for its elements. \
The default allocator uses new and delete to acquire and release memory.

#### List
The standard library offers a doubly-linked list called list.\
We use a list for sequences where we want to insert and delete elements without moving other ele- ments. 

A **forward_list** differs from a list by only allowing forward iteration. \
The point of that is to save space. There is no need to keep a predecessor pointer in each link and the size of an empty for- ward_list is just one pointer. \
A forward_list doesn’t even keep its number of elements. \
If you need the element count, count. If you can’t afford to count, you probably shouldn’t use a forward_list.

#### Map
The standard library offers a balanced binary search tree (usually, a red-black tree) called **map**.\
In other contexts, a map is known as an associative array or a dictionary. It is implemented as a balanced binary tree.\
The cost of a map lookup is **O(log(n))** where n is the number of elements in the map. That’s pretty good.

**unordered_map** - The standard-library hashed containers are referred to as ‘‘unordered’’ because they don’t require an ordering function.\
The standard library provides a default hash function for strings as well as for other built-in and standard-library types. \
If necessary, you can provide your own:
```cpp
struct Record { 
    string name;
    int product_code;
// ... };

struct Rhash {     // a hash function for Record 
    size_t operator()(const Record& r) const 
    {
        return hash<string>()(r.name) ˆ hash<int>()(r.product_code); 
    }
};

unordered_set<Record,Rhash> my_set;       // set of Records using Rhash for lookup
```
Creating a new hash function by combining existing hash functions using exclusive-or (ˆ) is simple and often very effective.\
We can avoid explicitly passing the hash operation by defining it as a specialization of the stan- dard-library hash:
```cpp
// make a hash function for Record
namespace std 
{     

template<> 
struct hash<Record> 
{ 
    using argument_type = Record; 
    using result_type = std::size_t;

    size_t operator()(const Record& r) const 
    {
        return hash<string>()(r.name) ˆ hash<int>()(r.product_code); 
    }
}; 

}
```

#### Container adaptors
In addition, the standard library provides container adaptors **queue<T>**, **stack<T>**, and **priority_queue<T>**.\
**TODO Add more details of those**

### Specialized Containers


- **T[N]**              Built-in array: a fixed-size contiguously allocated sequence of N elements of type T; implicitly converts to a T∗
- **array<T,N>**        A fixed-size contiguously allocated sequence of N elements
                        of type T; like the built-in array, but with most problems solved 
- **bitset<N>**         A fixed-size sequence of N bits
- **vector\<bool\>**      A sequence of bits compactly stored in a specialization of vector 
- **pair\<T,U\>**         Two elements of types T and U
- **tuple\<T...\>**	A sequence of an arbitrary number of elements of arbitrary types 
- **basic_string\<C\>**   A sequence of characters of type C; provides string operations
- **valarray\<T\>**       An array of numeric values of type T; provides numeric operations


They serve common but different (often overlapping) needs. \
If the standard library didn’t provide them, many people would have to design and implement their own. \
For example:
- **pair** and **tuple** are heterogeneous; all other containers are homogeneous (all elements are of the same type).
- **array**, **vector**, and **tuple** elements are contiguously allocated; **forward_list** and **map** are linked structures.
- **bitset** and **vector\<bool\>** hold bits and access them through proxy objects; \
all other standard-library containers can hold a variety of types and access elements directly.
- **basic_string** requires its elements to be some form of character and to provide string manip- ulation, such as concatenation and locale-sensitive operations.
- **valarray** requires its elements to be numbers and to provide numerical operations.

#### array
An **array**, defined in **\<array\>**, is a fixed-size sequence of elements of a given type where the number of elements is specified at compile time. \
Thus, an **array** can be allocated with its elements on the stack, in an object, or in static storage. \
The elements are allocated in the scope where the **array** is defined. \
The number of elements in the initializer must be equal to or less than the number of elements specified for the **array**.

Why would we use an **array** when vector is so much more flexible? \
An **array** is less flexible so it is simpler. \
Occasionally, there is a significant performance advantage to be had by directly accessing elements allocated on the stack \
rather than allocating elements on the free store, accessing them indirectly through the vector (a handle), and then deallocating them. \
On the other hand, the stack is a limited resource (especially on some embedded systems), and stack overflow is nasty.

However, BS main reason to prefer **array** is that it saves BS from surprising and nasty conversions to pointers. \
Consider:
```cpp
void h() {
    Circle a1[10]; 
    array<Circle,10> a2;
    // ...
    Shape∗ p1 = a1;  // OK: disaster waiting to happen
    Shape∗ p2 = a2;  // error: no conversion of array<Circle,10> to Shape*
    p1[3].draw();    //disaster
    
}
```
The ‘‘disaster’’ comment assumes that **sizeof(Shape)<sizeof(Circle)**, so subscripting a **Circle[]** \
through a Shape∗ gives a wrong offset. \
All standard containers provide this advantage over built-in arrays.

#### bitset
Class **bitset**<N> generalizes this notion by providing operations on a sequence of N bits [0:N), where N is known at compile time. For sets of bits that don’t fit into a long long int, using a **bitset** is much more convenient than using integers directly. For smaller sets, **bitset** is usually optimized. If you want to name the bits, rather than numbering them, you can use a set (§11.4) or an enumeration (§2.5).

A **bitset** can be initialized with an integer or a string: 

```cpp	
    bitset<9> bs1 {"110001111"};  
    bitset<9> bs2 {0b1'1000'1111};  // binary literal using digit separators (§1.4) 
```

The usual bitwise operators (§1.4) and the left and right-shift operators (<< and >>) can be applied:
```cpp
    bitset<9> bs3 =  ̃bs1;     // complement: bs3=="001110000"  
    bitset<9> bs4 = bs1&bs3;  // all zeros
    bitset<9> bs5 = bs1<<2;   // shift left: bs5 = "000111100"
```

The shift operators (here, <<) ‘‘shift in’’ zeros.\
The operations **to_ullong()** and **to_string()** provide the inverse operations to the constructors. \
For example, we could write out the binary representation of an int:
```cpp
void binary(int i) {
    bitset<8∗sizeof(int)> b = i; // assume 8-bit byte (see also §14.7)
    cout << b.to_string() << '\n'; // write out the bits of i 
}
```
For this example, it is simpler to directly use the bitset output operator:
```cpp
void binary2(int i)
{
    bitset<8∗sizeof(int)> b = i;    // assume 8-bit byte
    cout << b << '\n';              // write out the bits of i
}
```