### Avoiding deadlocks by using scoped_lock

***scoped_lock*** is a real help in deadlock situations. \
It came with C++17 and works the same way as *lock_guard* and *unique_lock* work: its constructor performs the locking, and its destructor the unlocking of a mutex. *scoped_lock*'s specialty is that it can do this with multiple mutexes.

*scoped_lock* uses the *std::lock* function, which applies a special algorithm that performs a series of *try_lock* calls on all the mutexes provided, in order to prevent deadlocking. \
Therefore it is perfectly safe to use *scoped_lock* or call *std::lock* on the same set of locks, but in different orders.

```cpp
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
using namespace chrono_literals;

mutex mut_a;
mutex mut_b;

static void deadlock_func_1()
{
    cout << "bad f1 acquiring mutex A..." << endl;
    lock_guard<mutex> la {mut_a};

    this_thread::sleep_for(100ms);

    cout << "bad f1 acquiring mutex B..." << endl;
    lock_guard<mutex> lb {mut_b};

    cout << "bad f1 got both mutexes" << endl;
}

static void deadlock_func_2()
{
    cout << "bad f2 acquiring mutex B..." << endl;
    lock_guard<mutex> lb {mut_b};

    this_thread::sleep_for(100ms);

    cout << "bad f2 acquiring mutex A..." << endl;
    lock_guard<mutex> la {mut_a};

    cout << "bad f2 got both mutexes" << endl;
}

static void sane_func_1()
{
    scoped_lock l {mut_a, mut_b};

    cout << "sane f1 got both mutexes" << endl;
}

static void sane_func_2()
{
    scoped_lock l {mut_b, mut_a};

    cout << "sane f2 got both mutexes" << endl;
}

int main()
{
    {
        thread t1 {sane_func_1};
        thread t2 {sane_func_2};

        t1.join();
        t2.join();
    }

    {
        thread t1 {deadlock_func_1};
        thread t2 {deadlock_func_2};

        t1.join();
        t2.join();
    }
}
```

Output:
```bash
$ ./avoid_deadlock 
sane f1 got both mutexes
sane f2 got both mutexes
bad f2 acquiring mutex B...
bad f1 acquiring mutex A...
bad f1 acquiring mutex B...
bad f2 acquiring mutex A...
```
