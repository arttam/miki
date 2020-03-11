### Sleep

The *sleep_for* and *sleep_until* functions have been added to C++11 and reside in the *std::this_thread* namespace. \
They block the current thread (not the whole process or program) for a specific amount of time. \
A thread does not consume CPU time while it is blocked. \
It is just put into an inactive state by the operating system.

The timing for waking up is only as accurate as the operating system allows. \
This will be generally accurate enough with most operating systems, but it might become difficult if some application needs nanosecond-granularity.

Another possibility to put a thread to sleep for a short time is *this_thread::yield*. \
It accepts no arguments, which means that we cannot know for how long the execution of a thread is placed back. \
The reason is that this function does not really implement the notion of sleeping or parking a thread. \
It just tells the operating system in a cooperative way that it can reschedule any other thread of any other process. \
If there are none, then the thread will be executed again immediately.

```cpp
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono_literals;

int main()
{
    cout << "Going to sleep for 5 seconds and 300 milli seconds.\n";

    this_thread::sleep_for(5s + 300ms);

    cout << "Going to sleep for another 3 seconds.\n";

    this_thread::sleep_until(chrono::high_resolution_clock::now() + 3s);

    cout << "That's it.\n";
}
```
Output:
```bash

$ time ./sleep 
Going to sleep for 5 seconds and 300 milli seconds.
Going to sleep for another 3 seconds.
That's it.

real 0m8.320s
user 0m0.005s
sys  0m0.003s
```

Notes:\
The *this_thread::sleep_for* function accepts a *chrono::duration* value. \
In the simplest case, this is just *1s* or *5s + 300ms*, just like in our example code. \
In order to get such nice literals for time spans, we need to declare *using namespace std::chrono_literals;* 
