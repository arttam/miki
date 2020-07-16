## new operator vs operator new

```cpp
string *ps = new string("Some value");
```
Using new *operator* - which:
- Allocates enough memory to hold _string_ object
- Calls constructor to initialize an object in memory that was allocated

Name of function the *new* operator calls to allocate memory is *operator new*
The *operator new* function usually declared like this:
```cpp
void  * operator new (size_t size);
```

### Placement new
Construct object in already allocated memory, for example:
```cpp
class Widget{
public:
  Widjet(int widjetSize);
  ...
};

Widget* constructWidjetInBuffer(void *buffer, int widgetSize)
{
  return new (buffer) Widget(windgetSize);
}
```

### operator delete
Deletes memory allocated by operator new:
```cpp
void *buffer = operator new(50 * sizeof(char)); // Allocates memory, calls no ctor
...
operator delete(buffer); // Dealocates memory, call no dtor