### Variadic templates (variadics)

The strength of variadic templates (sometimes just called *variadics*) is that they can accept any arguments you care to give them. 

Weaknesses include:
* The recursive implementations can be tricky to get right.
* The recursive implementations can be surprisingly expensive in compile time.
* The type checking of the interface is a possibly elaborate template program.

#### Fold expressions

```cpp
template<Number... T> 
int sum(T... v)
{
    return (v + ... + 0); // add all elements of v starting with 0 
}

// Usage
int x = sum(1, 2, 3, 4, 5); // x becomes 15
int y = sum('a', 2.4, x); // y becomes 114 (2.4 is truncated and the value of ’a’ is 97)
```

##### Right fold
```cpp
return (v + ... + 0); // add all elements of v to 0
```

Here, **(v+...+0)** means add all the elements of **v** starting with the initial value **0**.

The first element to be added is the ‘‘rightmost’’ (the one with the highest index): 

**(v[0]+(v[1]+(v[2]+(v[3]+(v[4]+0)))))**. 

That is, starting from the right where the 0 is. It is called a **right fold**. 

##### Left fold
```cpp
template<Number... T> int sum2(T... v)
{
return (0 + ... + v); // add all elements of v to 0 
}
```

Now, the first element to be added is the ‘‘leftmost’’ (the one with the lowest index): 

**(((((0+v[0])+v[1])+v[2])+v[3])+v[4])**. 

That is, starting from the left where the 0 is.

#### Fold

```cpp
template<typename ...T> void print(T&&... args)
{
    (std::cout << ... << args) << '\n';
}
print("Hello!"s,' ',"World ",2017);
// print all arguments
// (((((std::cout << "Hello!"s) << ’ ’) << "World ") << 2017) << ’\n’);
```

Many use cases simply involve a set of values that can be converted to a common type. 
In such cases, simply copying the arguments into a vector or the desired type often simplifies further use:
```cpp
template<typename Res, typename... Ts> 
vector<Res> to_vector(Ts&&... ts)
{
    vector<Res> res; 
    (res.push_back(ts) ...); 
    return res;
}
```
We can use to_vector like this:
```cpp
auto x = to_vector<double>(1,2,4.5,'a');
```
