### Starting and stopping threads

In order to start a thread, we first need some function that will be executed by it. \
The function does not need to be special, as a thread could execute practically every function.\

For example:
```cpp
void f(int i) { cout << i << '\n'; }

int main()
{
    thread t {f, 123};
    t.join();
}
```
The constructor call of std::thread accepts a function pointer or a callable object, followed by arguments that should be used with the function call.\
If the system has multiple CPU cores, then the threads can run parallel *and* concurrently.\
If the computer has only one CPU core, then there can be a lot of threads that run in parallel but never concurrently because one CPU core can only run one thread at a time. \
The threads are then run in an interleaved way where every thread is executed for some parts of a second, then paused, and then the next thread gets a time slice.

If they do not need to share a CPU core, then they can run concurrently, as *in really at the same time*.

At this point, we have absolutely no control over the following details:

- The *order* in which the threads are interleaved when sharing a CPU core.
- The *priority* of a thread, or which one is more important than the other.
- The fact that threads are really *distributed* among all the CPU cores or if the operating system just pins them to the same core. \
It is indeed possible that all our threads run on only a single core, although the machine has more than 100 cores.

We must always *join* or *detach* threads. \
If we don't do one of the two, the destructor of the *thread* object will call *std::terminate()*, which leads to an abrupt application shutdown.

```cpp
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono_literals;

static void thread_with_param(int i)
{
    this_thread::sleep_for(1ms * i);

    cout << "Hello from thread " << i << '\n';

    this_thread::sleep_for(1s * i);

    cout << "Bye from thread " << i << '\n';
}

int main()
{
    cout << thread::hardware_concurrency()
         << " concurrent threads are supported.\n";

    thread t1 {thread_with_param, 1};
    thread t2 {thread_with_param, 2};
    thread t3 {thread_with_param, 3};

    t1.join();
    t2.join();
    t3.detach();

    cout << "Threads joined.\n";
}
```