## How to use __declspec(dllexport) and __declspec(dllimport)

When compiling the DLL you have to write __declspec(dllexport) as you did. This tells the compiler you want it to be exported. When using the DLL you want __declspec(dllimport) in your included files. The compiler then knows that this functions and classes are in a DLL-file and need to be imported. Because you don't want to change your header-files that much, you should define a macro e.g. BUILD_DLL.

```cpp
    #ifdef BUILD_DLL
    #define DLL_PORTING __declspec(dllexport)
    #else
    #define DLL_PORTING __declspec(dllimport)
    #endif
```

Now you write in example.h:

```cpp
    class DLL_PORTING example_class { … };
```
In your .exe file just include the header files you need and everything will work.
