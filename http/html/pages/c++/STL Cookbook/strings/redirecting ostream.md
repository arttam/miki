### Redirecting ostream 

```cpp
#include <iostream>
#include <fstream>

using namespace std;

class redirect_cout_region {
    using buftype = decltype(cout.rdbuf());

    ofstream ofs;
    buftype buf_backup;

public:
    explicit redirect_cout_region(const string &filename)
        : ofs{filename}, buf_backup{cout.rdbuf(ofs.rdbuf())}
    {}

    redirect_cout_region()
        : ofs{}, buf_backup{cout.rdbuf(ofs.rdbuf())}
    {}

    ~redirect_cout_region() { cout.rdbuf(buf_backup); }
};

void my_output_heavy_function()
{
    cout << "some output\n";
    cout << "this function does really heavy work\n";
    cout << "... and lots of it...\n";
    // ...
}

int main()
{
    cout << "Readable from normal stdout\n";

    {
        redirect_cout_region _ {"output.txt"};
        cout << "Only visible in output.txt\n";
        my_output_heavy_function();
        redirect_cout_region _2 {"fofo.txt"};
        cout << "fofo\n";
    }

    {
		redirect_cout_region _;
		cout << "This output will completely vanish\n";
	}

	cout << "Readable from normal stdout again\n";
}
```
Output
```bash
$ ./log_regions 
Readable from normal stdout
Readable from normal stdout again

$ cat output.txt 
Only visible in output.txt
some output
this function does really heavy work
... and lots of it...
```
#### How it works
Every stream object has an internal buffer for which it acts as a front end. Such buffers are exchangeable. \
If we have a stream object, *s*, and want to save its buffer into a variable, *a*, and install a new buffer, *b*, this looks like the following: 
```cpp
a = s.rdbuf(b);
```
Restoring it can be simply done with:
```cpp
s.rdbuf(a);
```

This is exactly what we did in this recipe. \
Another cool thing is that we can stack those *redirect_cout_region* helpers:

```cpp
{
    cout << "print to standard output\n";

    redirect_cout_region la {"a.txt"};
    cout << "print to a.txt\n";
    
    redirect_cout_region lb {"b.txt"};
    cout << "print to b.txt\n";
}
cout << "print to standard output again\n";
```

This works because objects are destructed in the opposite order of their construction. \
The concept behind this pattern that uses the tight coupling between construction and destruction of objects is called **Resource Acquisition Is Initialization (RAII)**


Notes:\
Feeding a default-constructed file stream buffer into the cout stream buffer leads to cout being kind of *deactivated*. \
It will just *drop* its input we give it for printing.