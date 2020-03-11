#### const ref lifespan

Having:

```cpp

std::string ReturnStringValue() 
{ 
    return "string value"; 
} 

```

Results of:

```cpp

1. std::string& val = ReturnStringValue();
2. const std::string& val = ReturnStringValue();

```

Are:

1. Undefined behaviour - temporary object on stack to reference 
2. Valid until val is valid, because of **const** on stack.
Will become invalid as soon as reference goes out of scope


