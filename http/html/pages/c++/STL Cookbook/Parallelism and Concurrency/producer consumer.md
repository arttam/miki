### std::conditional_variable usage example

***std::conditional_variable*** :
- Waits for specific condition
- Signaling that condition

```cpp
#include <iostream>
#include <queue>
#include <tuple>
#include <condition_variable>
#include <thread>

using namespace std;
using namespace chrono_literals;


queue<size_t>      q;
mutex              mut;
condition_variable cv;
bool               finished {false};

static void producer(size_t items) {
	for(size_t i {0}; i < items; ++i) {
        this_thread::sleep_for(100ms);
		{
			lock_guard<mutex> lk {mut};
			q.push(i);
		}
		cv.notify_all();
	}

	{
		lock_guard<mutex> lk {mut};
		finished = true;
	}
	cv.notify_all();
}

static void consumer() {
	while (!finished) {
		unique_lock<mutex> l {mut};

		cv.wait(l, [] { return !q.empty() || finished; });

		while (!q.empty()) {
			cout << "Got " << q.front() << " from queue.\n";
			q.pop();
		}
	}
}

int main() {

	thread t1 {producer, 10};
	thread t2 {consumer};
	t1.join();
	t2.join();
	cout << "finished!\n";
}
```

Output:
```bash
$ ./producer_consumer
Got 0 from queue.
Got 1 from queue.
Got 2 from queue.
Got 3 from queue.
Got 4 from queue.
Got 5 from queue.
Got 6 from queue.
Got 7 from queue.
Got 8 from queue.
Got 9 from queue.
finished!
```

Note:\
Consumer loop
```cpp
while (!finished) {
    unique_lock<mutex> l {mut};

    cv.wait(l, [] { return !q.empty() || finished; });

    while (!q.empty()) {
        // consume
    }
}
```
This code is somewhat equivalent to the following alternative code. \
We will elaborate soon why it is not really the same:
```cpp
while (!finished) {
    unique_lock<mutex> l {mut};

    while (q.empty() && !finished) {
        l.unlock();
        l.lock();
    }

    while (!q.empty()) {
        // consume
    }
}
```
The real reason why the *cv.wait* line is not equivalent to the *while (q.empty() && ... )* construct is, that we cannot simply loop over a *l.unlock(); l.lock();* cycle. \
If the producer thread is inactive for some time, then this would lead to continuous locking and unlocking of the mutex, which makes no sense because it needlessly burns CPU cycles.

An expression like *cv.wait(lock, predicate)* will wait until *predicate()* returns true. \
But it does not do this by *continuously* unlocking and locking lock. \
In order to wake a thread up that blocks on the wait call of a condition_variable object, another thread has to call the *notify_one()* or *notify_all()* method on the same object. \
Only then the waiting thread(s) is/are kicked out of their sleep in order to check if *predicate()* holds.

The nice thing about the wait call checking the predicate is that if there is a *spurious* wakeup call, the thread will go to sleep immediately again.