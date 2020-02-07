### C-style I/O

The C++ standard library also supports the C standard-library I/O, including printf() and scanf(). \
Many uses of this library are unsafe from a type and security point-of-view, \
so I don’t recommend its use. In particular, it can be difficult to use for safe and convenient input. \
It does not support user-defined types. 

If you don’t use C-style I/O and care about I/O performance, call

```cpp
ios_base::sync_with_stdio(false); // avoid significant overhead
```
Without that call, iostreams can be **significantly** slowed down to be compatible with the C-style I/O