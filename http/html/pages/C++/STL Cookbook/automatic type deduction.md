### Automatical type deduction

```cpp
std::pair  my_pair  (123, "abc");       // std::pair<int, const char*>
std::tuple my_tuple (123, 12.3, "abc"); // std::tuple<int, double, 
                                        //            const char*> 
```

Having:
```cpp
template <typename T1, typename T2, typename T3>
class my_wrapper {
    T1 t1;
    T2 t2;
    T3 t3;

public:
    explicit my_wrapper(T1 t1_, T2 t2_, T3 t3_) 
        : t1{t1_}, t2{t2_}, t3{t3_}
    {}

    /* … */
};
```

Before C++17:
```cpp
my_wrapper<int, double, const char *> wrapper {123, 1.23, "abc"};
```

With C++17:
```cpp
my_wrapper wrapper {123, 1.23, "abc"};
```
Before C++17, this was only possible by implementing a *make function helper*:

```cpp
my_wrapper<T1, T2, T3> make_wrapper(T1 t1, T2 t2, T3 t3)
{
    return {t1, t2, t3};
}
```
The STL already comes with a lot of helper functions such as that one: *std::make_shared*, *std::make_unique*, *std::make_tuple*, and so on. \
In C++17, these can now mostly be regarded as ***obsolete***. \
Of course, they will be provided further for compatibility reasons.

#### explicit deduction guide

Having:
```cpp
template <typename T>
struct sum {
    T value;

    template <typename ... Ts>
    sum(Ts&& ... values) : value{(values + ...)} {}
};
```
This struct, *sum*, accepts an arbitrary number of parameters and adds them together using a fold expression.\
Now the question is, what type is *T*? \
If we don't want to specify it explicitly, it surely needs to depend on the types of the values provided in the constructor. \
If we provide *string* instances, it needs to be *std::string*. \
If we provide integers, it needs to be *int*. \
If we provide integers, floats, and doubles, the compiler needs to figure out which type fits all the values without information loss. \
In order to achieve that, we provide an *explicit deduction guide*:
```cpp
template <typename ... Ts>
sum(Ts&& ... ts) -> sum<std::common_type_t<Ts...>>; 
```
This deduction guide tells the compiler to use the ***std::common_type_t*** trait, which is able to find out which common type fits all the values. \
Let's see how to use it:
```cpp
sum s          {1u, 2.0, 3, 4.0f};
sum string_sum {std::string{"abc"}, "def"};

std::cout << s.value          << '\n'
          << string_sum.value << '\n';
```
In the first line we instantiate a *sum* object with constructor arguments of type *unsigned*, *double*, *int*, and *float*. \
The *std::common_type_t* returns double as the common type, so we get a *sum<double>* instance. \
In the second line, we provide an *std::string* instance and a C-style string. \
Following our deduction guide, the compiler constructs an instance of type *sum&lt;std::string&gt;*.