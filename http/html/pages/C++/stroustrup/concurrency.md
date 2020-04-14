### Concurrency

#### Tasks and threads
We call a computation that can potentially be executed concurrently with other computations a *task*. \
A *thread* is the system-level representation of a task in a program. \
A task to be executed concurrently with other tasks is launched by constructing a **std::thread** (found in **<thread>**) with the task as its argument. \
A task is a function or a function object:
```cpp
void f();

struct F {
	void operator()();
};

void user() {
	thread t1 {f};   // f() executes in separate thread
	thread t2 {F()}; // F()() executes in separate thread
	
	t1.join(); 
	t2.join();
}
```
The **join()** ensure that we don’t exit user() until the threads have completed. \
To ‘‘join’’ a thread means to ‘‘wait for the thread to terminate.’’

```cpp
void f(vector<double>& v); // function: do something with v

int main()
{
	vector<double> some_vec {1,2,3,4,5,6,7,8,9};

	thread t1 {f,ref(some_vec)}; // f(some_vec) executes in a separate thread
}
```
The initialization with **{f,ref(some_vec)}** uses a thread variadic template constructor that can accept an arbitrary sequence of arguments. \
The **ref()** is a type function from **<functional>** that unfortunately is needed to tell the variadic template to treat some_vec as a reference, \
rather than as an object. \
Without that **ref()**, some_vec would be passed by value.

#### Sharing data
The fundamental element of the solution is a mutex, a ‘‘mutual exclusion object.’’ A thread acquires a mutex using a lock() operation:
```cpp
mutex m; // controlling mutex 
int sh; // shared data

void f() {
	scoped_lock lck {m};  // acquire mutex
	sh += 7;              // manipulate shared data
} // release mutex implicitly
```
The **scoped_lock’s** constructor acquires the mutex (through a call m.lock()). \
If another thread has already acquired the mutex, the thread waits (‘‘blocks’’) until the other thread completes its access. \
Once a thread has completed its access to the shared data, the scoped_lock releases the mutex (with a call m.unlock()). \
When a mutex is released, threads waiting for it resume executing (‘‘are woken up’’). \
The mutual exclusion and locking facilities are found in **<mutex>**.

It is not uncommon to need to simultaneously access several resources to perform some action. \
This can lead to deadlock. \
For example, if thread1 acquires mutex1 and then tries to acquire mutex2 while thread2 acquires mutex2 and then tries to acquire mutex1, then neither task will ever proceed further. \
The **scoped_lock** helps by enabling us to acquire several locks simultaneously:
```cpp
void f() {
	scoped_lock lck {mutex1,mutex2,mutex3};  // acquire all three locks
	// ... manipulate shared data ... }      // implicitly release all mutexes
```
This **scoped_lock** will proceed only after acquiring all its mutexes arguments and will never block (‘‘go to sleep’’) while holding a mutex. \
The destructor for scoped_lock ensures that the mutexes are released when a thread leaves the scope.

One of the most common ways of sharing data is among many readers and a single writer. \
This ‘‘reader-writer lock’’ idiom is supported be **shared_mutex**. \
A reader will acquire the mutex ‘‘shared’’ so that other readers can still gain access, whereas a writer will demand exclusive access. \
For example:
```cpp
shared_mutex mx; // a mutex that can be shared 

void reader()
{
	shared_lock lck(mx};
	// ... read ... 
}

void writer() {
	unique_lock lck {mx};
	// ... write ... 
}
```

#### Events (conditional_variable)
The basic support for communicating using external events is provided by **condition_variables** found in **<condition_variable>**. \
A condition_variable is a mechanism allowing one thread to wait for another. \
In particular, it allows a thread to wait for some *condition* (often called an *event*) to occur as the result of work done by other threads.

Classical example message exchange:
```cpp
class Message { // object to be communicated 
	// ...
};

queue<Message> mqueue; // the queue of messages 
condition_variable mcond; // the variable communicating events 
mutex mmutex; // for synchronizing access to mcond
```
Consumer
```cpp
void consumer() {
	while(true) {
		unique_lock lck {mmutex}; // acquire mmutex 
		mcond.wait(lck,[] { return !mqueue.empty(); });  // release lck and wait 
		 //re-acquire lck upon wakeup
		 // don’t wake up unless mqueue is non-empty 

		auto m = mqueue.front(); // get the message
		mqueue.pop();
		lck.unlock(); // release lck 
		// ... process m ...	
	} 
}
```

Producer
```cpp
void producer() {
	while(true) { 
		Message m;
		// ... fill the message ...
		scoped_lock lck {mmutex}; 
		mqueue.push(m);
		mcond.notify_one(); //notify
						  // release lock (at end of scope) 
	}
} 
```

#### future and promise
The important point about future and promise is that they enable a transfer of a value between two tasks without explicit use of a lock; \
‘‘the system’’ implements the transfer efficiently. \
The basic idea is simple: when a task wants to pass a value to another, it puts the value into a promise. \
Somehow, the implementation makes that value appear in the corresponding future, from which it can be read (typically by the launcher of the task).

If we have a **future<X>** called fx, we can **get()** a value of type X from it: 
```cpp
X v = fx.get(); // if necessary, wait for the value to get computed
```
If the value isn’t there yet, our thread is blocked until it arrives. \
If the value couldn’t be computed, get() might throw an exception.

The main purpose of a **promise** is to provide simple ‘‘put’’ operations (called **set_value()** and **set_exception()**) to match future’s **get()**.

#### packaged_task
How do we get a **future** into the task that needs a result and the corresponding **promise** into the thread that should produce that result? \
The **packaged_task** type is provided to simplify setting up tasks connected with futures and promises to be run on threads. \
A **packaged_task** provides wrapper code to put the return value or exception from the task into a promise.

If you ask it by calling **get_future**, a **packaged_task** will give you the future corresponding to its promise. \
For example, we can set up two tasks to each add half of the elements of a **vector<double>** using the standard-library **accumulate()** 
```cpp
double accum(double∗ beg, double∗ end, double init) // compute the sum of [beg:end) starting with the initial value init
{
	return accumulate(beg,end,init);
}

double comp2(vector<double>& v) {
	using Task_type = double(double∗,double∗,double); 
// type of task
	
	packaged_task<Task_type> pt0 {accum}; // package the task (i.e., accum)
	packaged_task<Task_type> pt1 {accum};
	
	future<double> f0 {pt0.get_future()}; // get hold of pt0’s future 
	future<double> f1 {pt1.get_future()}; // get hold of pt1’s future
	
	double∗ first = &v[0];
	thread t1 {move(pt0),first,first+v.size()/2,0};          // start a thread for pt0 
	thread t2 {move(pt1),first+v.size()/2,first+v.size(),0}; // start a thread for pt1
	// ...
	
	return f0.get()+f1.get(); // get the results
}
```
The **packaged_task** template takes the type of the task as its template argument (here **Task_type**, an alias for **double(double∗,double∗,double)**) \
and the **task** as its constructor argument (here, **accum**). \
The **move()** operations are needed because a **packaged_task** cannot be copied. \
The reason that a **packaged_task** cannot be copied is that it is a resource handle: it owns its promise and is (indirectly) responsible for whatever resources its task may own.
