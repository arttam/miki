### std::async

**std::async(std::launch::async, ...** starts new threads and lets them execute the functions concurrently

**future** object is kind of an empty *placeholder* until the thread that executes the histogram function returns. \
The resulting map is then placed into the future object so we can finally access it. \
The get function then gives us access to the encapsulated result.

Let's have a look at another minimal example. Consider the following code snippet:
```cpp
auto x (f(1, 2, 3));
cout << x;
```
Instead of writing the preceding code, we can also do the following:
```cpp
auto x (async(launch::async, f, 1, 2, 3));
cout << x.get();
```
That's basically it.\
There are two policy flags which allow for three constellations:
- ***launch::async*** - The function is guaranteed to be executed by another thread.
- ***launch::deferred*** - The function is executed by the same thread, but later (lazy evaluation). \
Execution then happens when get or wait is called on the future. If none of both happens, the function is not called at all.
- ***launch::async | launch::deferred*** - Having both flags set, the STL's async implementation is free to choose which policy shall be followed. \
This is the default choice if no policy is provided.

*Note:*\
By just calling *async(f, 1, 2, 3)* without a policy argument, we automatically select both policies. \
The implementation of *async* is then free to choose which policy to employ. \
This means that we cannot be sure that another thread is started at all, or if the execution is just deferred in the current thread.

There is indeed one last thing we should know about. Suppose, we write code as follows:

```cpp
async(launch::async, f);
async(launch::async, g);
```
This might have the motivation of executing functions f and g (we do not care about their return values in this example) in concurrent threads and then doing different things at the same time. \
While running such code, we will notice that the code **blocks** on this calls, which is most probably not what we want.

So why does it block? Isn't async all about nonblocking asynchronous calls? \
Yes it is, but there is one special peculiarity: if a future was obtained from an async call with the *launch::async* policy, then its destructor performs a **blocking** wait.

This means that both the async calls from this short example are blocking because the lifetime of the futures they return ends in the same line! \
We can fix this by capturing their return values in variables with a longer lifetime.


```cpp

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <future>

using namespace std;

static map<char, size_t> histogram(const string &s)
{
    map<char, size_t> m;

    for (char c : s) { m[c] += 1; }

    return m;
}

static string sorted(string s)
{
    sort(begin(s), end(s));
    return s;
}

static bool is_vowel(char c)
{
    char vowels[] {"aeiou"};
    return end(vowels) != find(begin(vowels), end(vowels), c);
}

static size_t vowels(const string &s)
{
    return count_if(begin(s), end(s), is_vowel);
}

int main()
{
    cin.unsetf(ios::skipws);
    string input {istream_iterator<char>{cin}, {}};
    input.pop_back();

    auto hist        (async(launch::async, histogram, input));
    auto sorted_str  (async(launch::async, sorted, input));
    auto vowel_count (async(launch::async, vowels, input));

    for(const auto &[c, count] : hist.get()) {
        cout << c << ": " << count << '\n';
    }

    cout << "Sorted string: " << quoted(sorted_str.get()) << '\n';
    cout << "Total vowels: " << vowel_count.get() << '\n';
}
```

Note: \
In the main function, we read the whole standard input into a string. \
In order to not segment the input into words, we deactivate ios::skipws. \
This way we get one large string, no matter how much white space the input contains. \
We use *pop_back* on the resulting string afterward because we got **one string terminating '\0' character too much** this way.