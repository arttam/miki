### Ratio conversions

#### Clock types
- **system_clock** - This represents the system-wide real-time "wall" clock. \
It is the right choice if we want to obtain the local time.
- **steady_clock** - This clock is promised to be monotonic. \
This means that it will never be set back by any amount of time. \
This can happen to other clocks when their time is corrected by minimal amounts, \
or even when the time is switched between winter and summer time.
- **high_resolution_clock** - This is the clock with the most fine-grained clock tick period the STL implementation can provide.

template type interface of duration:
```cpp
template<
    class Rep, 
    class Period = std::ratio<1> 
> class duration;
```
While seconds is the simplest unit to describe, as it works with *Period = ratio<1>*, the others have to be adjusted. \
As one millisecond is a thousandth of a second, we multiply the *seconds::period* (which is just a getter function to the Period parameter) with *milli*, \
which is a type alias for *std::ratio<1, 1000>* (std::ratio<a, b> represents the fractional value a/b). \
The ratio_multiply type is basically a *compile time function*, which represents the type that results from multiplying one ratio type with another.

Maybe this sounds too complicated, so let's have a look at an example: *ratio_multiply<ratio<2, 3>, ratio<4, 5>>* results in *ratio<8, 15>* because *(2/3) \* (4/5) = 8/15*.


```cpp
#include <iostream>
#include <chrono>
#include <ratio>
#include <cmath>
#include <iomanip>
#include <optional>

using namespace std;

using seconds      = chrono::duration<double>;
using milliseconds = chrono::duration<double, ratio_multiply<seconds::period, milli>>;
using microseconds = chrono::duration<double, ratio_multiply<seconds::period, micro>>;

static pair<string, seconds> get_input()
{
    string s;

    const auto tic (chrono::steady_clock::now());
    if (!(cin >> s)) {
        return {{}, {}};
    }
    const auto toc (chrono::steady_clock::now());

    return {s, toc - tic};
}

int main()
{
    while (true) {
        cout << "Please type the word \"C++17\" as fast as you can.\n> ";

        const auto [user_input, diff] = get_input();

        if (user_input == "") { break; }

        if (user_input == "C++17") {
            cout << "Bravo. You did it in:\n" << fixed << setprecision(2)
                  << setw(12) <<               diff.count() << " seconds.\n"
                  << setw(12) << milliseconds(diff).count() << " milliseconds.\n"
                  << setw(12) << microseconds(diff).count() << " microseconds.\n";
            break;
        } else {
            cout << "Sorry, your input does not match. You may try again.\n";
        }
    }
}
```
Notes:
- The *diff.count()* method returns the number of seconds as a floating point number. \
If we had used the original STL *seconds* duration type, then we would have got a rounded integer value, not a fraction. \
By feeding the *milliseconds* or *microseconds* **constructor** with our *diff* variable before calling *count()*, we get the same value transformed to a different unit.
