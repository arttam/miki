### std::call_once
*std:call_once* works like a barrier. It maintains access to a function (or a callable object). \
The first thread to reach it gets to execute the function. Until it has finished, any other thread that reaches the call_once line is blocked. \
After the first thread returns from the function, all other threads are released, too.

In order to organize this little choreography, a variable is needed from which the other threads can determine if they must wait and when they are released again. \
This is what our variable once_flag callflag; is for. \
Every *call_once* line also needs a *once_flag* instance as the argument prepending the function that shall be called only once.

Another nice detail is: \
If it happens, that the thread which is selected to execute the function in call_once fails because some *exception* is thrown, \
then the next thread is allowed to execute the function again. \
This happens in the hope that it will not throw an exception the next time.

```cpp

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

once_flag callflag;
mutex cout_mutex;

static void once_print(size_t x)
{
    cout << '!';
}

static void print(size_t x)
{
    std::call_once(callflag, once_print, x);
    cout << x;
}

int main()
{
    vector<thread> v;

    for (size_t i {0}; i < 10; ++i) {
        v.emplace_back(print, i);
    }

    for (auto &t : v) { t.join(); }

    cout << '\n';
}
```