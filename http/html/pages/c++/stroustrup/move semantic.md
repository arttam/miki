### move() and forward()

Confusingly, **std::move()** doesn’t move anything. Instead, it casts its argument to an *rvalue* reference, \
thereby saying that its argument will not be used again and therefore may be moved. \
It should have been called something like *rvalue_cast*. \
Like other casts, it’s error-prone and best avoided. \
It exists to serve a few essential cases. 

B.Stroustrup considers use of std::move() sometimes to be too error-prone for widespread use. \
Don’t use it unless you can demonstrate significant and necessary performance improvement. \
Later maintenance may accidentally lead to unanticipated use of the moved-from object.

The state of a moved-from object is in general unspecified, \
but all standard-library types leave a moved-from object in a state where it can be destroyed and assigned to.

Forwarding arguments is an important use case that requires moves. \
We sometimes want to transmit a set of arguments on to another function \
without changing anything (to achieve **‘‘perfect forwarding’’**):

```cpp
template<typename T, typename... Args> 
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>{new T{std::forward<Args>(args)...} // Forward each argument
};
```

The standard-library **forward()** differs from the simpler **std::move()** by correctly handling \
subtleties to do with *lvalue* and *rvalue*. \
Use **std::forward()** exclusively for forwarding and don’t **forward()** something twice; \
once you have forwarded an object, it’s not yours to use anymore.
