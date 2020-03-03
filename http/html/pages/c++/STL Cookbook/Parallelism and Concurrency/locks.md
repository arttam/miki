### Synchronisation 

The term *mutex* stands for **mut**ual **ex**clusion. \
In order to prevent that concurrently running threads alter the same object in a non-orchestrated way that might lead to data corruption, we can use *mutex* objects. \
The STL provides different mutex classes with different specialties. \
They all have in common that they have a *lock* and an *unlock* method.

Whenever a thread is the first one to call *lock()* on a mutex that was not locked before, it owns the mutex. \
At this point, other threads will block on their lock calls, until the first thread calls unlock again. *std::mutex* can do exactly this.

There are many different mutex classes in the STL:
- ***mutex*** - Standard mutex with a lock and an unlock method. Provides an additional nonblocking try_lock method.
- ***timed_mutex*** - Same as mutex, but provides additional *try_lock_for* and *try_lock_until* methods that allow for timing out instead of blocking forever.
- ***recursive_mutex*** - Same as mutex, but if a thread locked an instance of it already, it can call lock multiple times on the same mutex object without blocking. \
It is released after the owning thread called unlock as often as it called lock.
- ***recursive_timed_mutex*** - Provides the features of both timed_mutex and recursive_mutex.
- ***shared_mutex*** - This mutex is special in that regard, that it can be locked in exclusive mode and in shared mode. \
In exclusive mode, it shows the same behavior as the standard mutex class. \
If a thread locks it in shared mode, it is possible for other threads to lock it in shared mode, too. \
It will then be unlocked as soon as the last shared mode lock owner releases it. \
While a lock is locked in shared mode, it is not possible to obtain exclusive ownership. \
This is very similar to the behavior of shared_ptr, only that it does not manage memory, but lock ownership.
- ***shared_timed_mutex*** - Combines the features of shared_mutex and shared mode.

Everything is nice and easy as long as threads do just lock a mutex, access some concurrence protected object and unlock the mutex again. \
As soon as a forgetful programmer misses to unlock a mutex somewhere after locking it, or an exception is thrown while a mutex is still locked, things look ugly pretty quick.\
When regarding memory management, we have *unique_ptr*, *shared_ptr* and *weak_ptr*. \
Those helpers provide very convenient ways to avoid memory leaks. Such helpers exist for mutexes, too. \
The simplest one is *std::lock_guard*. It can be used as follows:

```cpp
void critical_function()
{
    lock_guard<mutex> l {some_mutex};

    // critical section
}
```
*lock_guard* element's constructor accepts a mutex, on which it calls lock immediately. \
The whole constructor call will block until it obtains the lock on the mutex. \
Upon destruction, it unlocks the mutex again. \
This way it is hard to get the lock/unlock cycle wrong because it happens automatically.


The C++17 STL provides the following different RAII lock-helpers. They all accept a template argument that shall be of the same type as the mutex:

- ***lock_guard*** - This class provides nothing else than a constructor and a destructor, which lock and unlock a mutex.
- ***scoped_lock*** - Similar to lock_guard, but supports arbitrarily many mutexes in its constructor. \
Will release them in opposite order in its destructor.
- ***unique_lock*** - Locks a mutex in exclusive mode. \
The constructor also accepts arguments that instruct it to timeout instead of blocking forever on the lock call. \
It is also possible to not lock the mutex at all, or to assume that it is locked already, or to only try locking the mutex. \
Additional methods allow to lock and unlock the mutex during the unique_lock lock's lifetime.
- ***shared_lock*** - Same as unique_lock, but all operations are applied on the mutex in shared mode.


```cpp
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono_literals;

shared_mutex shared_mut;

using shrd_lck = shared_lock<shared_mutex>;
using uniq_lck = unique_lock<shared_mutex>;

void print_exclusive()
{
    uniq_lck l {shared_mut, defer_lock};

    if (l.try_lock()) {
        cout << "Got exclusive lock.\n";
    } else {
        cout << "Unable to lock exclusively.\n";
    }
}

void exclusive_throw()
{
    uniq_lck l {shared_mut};
    throw 123;

}

int main()
{

    {
        shrd_lck sl1 {shared_mut};

        cout << "shared lock once.\n";

        {
            shrd_lck sl2 {shared_mut};

            cout << "shared lock twice.\n";

            print_exclusive();
        }

        cout << "shared lock once again.\n";

        print_exclusive();

    }
    cout << "lock is free.\n";

    try {
        exclusive_throw();
    } catch (int e) {
        cout << "Got exception " << e << '\n';
    }

    print_exclusive();
}
```

Output:
```bash
$ ./shared_lock 
shared lock once.
shared lock twice.
Unable to lock exclusively.
shared lock once again.
Unable to lock exclusively.
lock is free.
Got exception 123
Got exclusive lock.
``` 

Note:
```cpp
void print_exclusive()
{
    uniq_lck l {shared_mut, defer_lock};

    if (l.try_lock()) {
        // ...
    }
}
```
We did not only provide *shared_mut* but also *defer_lock* as constructor arguments for *unique_lock* in this procedure. \
**defer_lock** is an empty global object that can be used to select a different constructor of *unique_lock* that simply does not lock the mutex. \
By doing so, we are able to call *l.try_lock()* later, which does not block. \
In case the mutex is locked already, we can do something else. If it was indeed possible to get the lock, we still have the destructor tidying up after us.