## Terminolory used by B.Stroustrup

*resource handle* -when the class is responsible for an object accessed through a pointer.Resource handles, such as Vector and thread, are superior alternatives to direct use of built-in point- ers in many cases. We can make pointers disappear into resource handles.

*RAII (Resource Acquisition Is Initialization)* - each resource have an owner in some scope and by default be released at the end of its owners scope. RAII integrated with error handling if form of exceptions. In the C++ standard library, RAII is pervasive: for example, memory (string, vector, map, unordered_map, etc.), files (ifstream, ofstream, etc.), threads (thread), locks (lock_guard, unique_lock, etc.), and general objects (through unique_ptr and shared_ptr). The result is implicit resource man- agement that is invisible in common use and leads to low resource retention durations.

*template* - is a class or a function that we parameterize with a set of types or values.

*function object* (sometimes called a *functor*), which is used to define objects that can be called like functions. The beauty of these function objects is that they carry the value to be compared against with them.

*policy objects* - Function objects used to specify the meaning of key operations of a general algorithm (such as Less_than for count()).

*generic lambda* - lambda with an **auto** parameter.

*parameter pack*
```cpp
void print() 
{
    // what we do for no arguments: nothing 
}

template<typename T, typename... Tail> 
void print(T head, Tail... tail)
{
    // what we do for each argument, e.g., 
    cout << head << ' ';
    print(tail...);
}
```

Example: /code/c++/strostrup/examples/parameter pack.cpp

The typename... indicates that Tail is a sequence of types. The Tail... indicates that tail is a sequence of values of the types in Tail. A parameter declared with a ... is called a **parameter pack**.

*duck typing* - The instantiation-time type checking provided for templates checks the use of arguments in the template definition; i.e. we operate on values, and the presence and meaning of an operation depend solely on its operand values. (‘If it walks like a duck and it quacks like a duck, it’s a duck’’)