## Usage of anonymous namespace

Unnamed namespaces are a utility to make an identifier translation unit local. \
They behave as if you would choose a unique name per translation unit for a namespace

This means you can have free functions called (for example) help that can exist in multiple translation units, and they won't clash at link time. \
The effect is almost identical to using the **static** keyword used in C which you can put in in the declaration of identifiers. \
Unnamed namespaces are a superior alternative, being able to even make a type translation unit local.

```cpp
namespace { int a1; }
static int a2;
```
Both **a**'s are translation unit local and won't clash at link time. \
But the difference is that the a1 in the anonymous namespace gets a unique name.