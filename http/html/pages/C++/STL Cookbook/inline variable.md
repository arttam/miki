### Inline variable

C++ programs do often consist of multiple C++ source files. \
These are individually compiled to modules/object files. \
Linking all the modules/object files together into a single executable or shared/static library is then the last step.

At the link stage, it is considered an error if the linker can find the definition of one specific symbol multiple times. \
Let's say, for example, we have a function with a signature such as int *foo();*. \
If two modules define the same function, which is the right one? The linker can't just roll the dice. \

The traditional way to provide globally available functions is to declare them in the header files, \
which will be included by any C++ module that needs to call them. \
The definition of every of those functions will be then put once into separate module files. \
These are then linked together with the modules that desire to use these functions. \
This is also called the *One Definition Rule (ODR)*

However, if this were the only way, then it would not have been possible to provide header-only libraries. \
Header-only libraries are very handy because they only need to be included using #include into any C++ program file and then are immediately available.

For such cases, the ***inline*** keyword can be used to make an exception in order to allow multiple definitions of the same symbol in different modules. \
If the linker finds multiple symbols with the same signature, but they are declared inline, \
it will just choose the *first one* and trust that the other symbols have the *same* definition. \
That all equal inline symbols are defined completely equal is basically a *promise* from the programmer.

Problem:\
The *process_monitor* class should both contain a static member and be globally accessible itself, which would produce double-defined symbols when included from multiple translation units:
```cpp
   // foo_lib.hpp 
   
   class process_monitor { 
   public: 
       static const std::string standard_string 
           {"some static globally available string"}; 
   };
   
   process_monitor global_process_monitor;”
       
```

Solution:\
If we now include this in multiple *.cpp* files in order to compile and link them, this would fail at the linker stage. \
In order to fix this, we add the ***inline*** keyword:
```cpp
   // foo_lib.hpp 

   class process_monitor { 
   public: 
       static const inline std::string standard_string 
           {"some static globally available string"}; 
   };

   inline process_monitor global_process_monitor;
```
Voila, that's it!

#### Pre C++17 workarounds

One possible workaround before C++17 was providing a static function, which returns a reference to a static object:

```cpp
class foo {
public:
    static std::string& standard_string() {
        static std::string s {"some standard string"};
        return s;
    }
};
```
This way, it is completely legal to include the header file in multiple modules but still getting access to exactly the same instance everywhere. \
However, the object is **not** constructed **immediately** at the start of program but only on the first call of this getter function. \
For some use cases, this is indeed a problem.