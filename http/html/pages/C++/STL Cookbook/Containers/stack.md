### std::stack

The *std::stack* is an adapter class which lets the user push objects *onto* it like on a real stack of objects, and pop objects *down from* it again.

```cpp
#include <iostream>
#include <stack>
#include <iterator>
#include <map>
#include <sstream>
#include <cassert>
#include <vector>
#include <stdexcept>
#include <cmath>

using namespace std;

template <typename IT>
double evaluate_rpn(IT it, IT end)
{
    stack<double> val_stack;

    map<string, double (*)(double, double)> ops {
        {"+", [](double a, double b) { return a + b; }},
        {"-", [](double a, double b) { return a - b; }},
        {"*", [](double a, double b) { return a * b; }},
        {"/", [](double a, double b) { return a / b; }},
        {"^", [](double a, double b) { return pow(a, b); }},
        {"%", [](double a, double b) { return fmod(a, b); }},
    };

    auto pop_stack ([&](){ auto r (val_stack.top()); val_stack.pop(); return r; });

    for (; it != end; ++it) {
        stringstream ss {*it};
        if (double val; ss >> val) {
            val_stack.push(val);
        } else {
            assert(val_stack.size() >= 2);

            const auto r {pop_stack()};
            const auto l {pop_stack()};

            try {
                val_stack.push(ops.at(*it)(l, r));
            } catch (const out_of_range &) {
                throw invalid_argument(*it);
            }
        }
    }

    return val_stack.top();
}

int main()
{
    try {
        cout << evaluate_rpn(istream_iterator<string>{cin}, {}) << '\n';
    } catch (const invalid_argument &e) {
        cout << "Invalid operator: " << e.what() << '\n';
    }

#if 0
    stringstream s {"3 2 1 + * 2 /"};
    cout << evaluate_rpn(istream_iterator<string>{s}, {}) << '\n';

    vector<string> v {"3", "2", "1", "+", "*", "2", "/"};
    cout << evaluate_rpn(begin(v), end(v)) << '\n';
#endif
}

```

Output:
```bash
“$ echo "3 1 2 + * 2 /" | ./rpn_calculator
4.5
```
The input *"3 1 2 + \* 2 /"* represents the expression *( 3 \* (1 + 2) ) / 2* and yields the correct result: *4.5*

We push items onto the stack, simply using the push function of *std::stack*:

```cpp
val_stack.push(val);
```
Pop values lambda:
```cpp
auto pop_stack ([&](){
    auto r (val_stack.top()); // Get top value copy
    val_stack.pop();          // Throw away top value
    return r;                 // Return copy
});
```

This lambda is necessary to get the top value of the stack and remove it from there in one step. \
The interface of *std::stack* is not designed in a way which would allow doing that in a single call. \
However, defining a lambda is quick and easy, so we can now get values like this:

```cpp
double top_value {pop_stack()};
```
***Use iterators wherever it makes sense. This makes your code very composable and reusable.***
```cpp
int main()
{
    stringstream s {"3 2 1 + * 2 /"};
    cout << evaluate_rpn(istream_iterator<string>{s}, {}) << '\n';

    vector<string> v {"3", "2", "1", "+", "*", "2", "/"};
    cout << evaluate_rpn(begin(v), end(v)) << '\n';
}
```