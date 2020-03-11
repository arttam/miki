### Error sum using inner_product

Calculating numerical *error* between target value and actual value. \
Measuring difference between signals by subtracting data points and squearing difference.\
(negative and positive differences become comparable).

I.e.: For every *i* we calculate *a[i] - b[i]*, and square that difference

```cpp
#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>
#include <iterator>

using namespace std;

int main()
{
    const size_t sig_len {100};
    vector<double> as (sig_len);
    vector<int>    ds (sig_len);

    auto sin_gen ([n{0}] () mutable { return 5.0 * sin(n++ * 2.0 * M_PI / 100); });

    generate(begin(as), end(as), sin_gen);
    copy(begin(as), end(as), begin(ds));

    copy(begin(as), end(as), ostream_iterator<double>{cout, " "});
    cout << '\n';
    copy(begin(ds), end(ds), ostream_iterator<double>{cout, " "});
    cout << '\n';

    cout << inner_product(begin(as), end(as), begin(ds), 0.0,
            std::plus<double>{},
            [](double a, double b) { return pow(a - b, 2); }) << '\n';
}
```

In this recipe, we stuffed the task of looping through two vectors, \
getting the difference between their corresponding values, squaring them, \
and finally summing them up into one *std::inner_product* call. \
On the way, the only code we crafted ourselves was the lambda expression *[](double a, double b) { return pow(a - b, 2); }*, \
which takes the difference of its arguments and squares it.

A glance at a possible implementation of std::inner_product shows us why and how this works:
```cpp
template<class InIt1, class InIt2, class T, class F, class G>
T inner_product(InIt1 it1, InIt1 end1, InIt2 it2, T val,
                F bin_op1, G bin_op2)
{
    while (it1 != end1) {
        val = bin_op1(val, bin_op2(*it1, *it2));
        ++it1;
        ++it2;
    }
    return value;
}
```

The algorithm accepts a pair of begin/end iterators of the first range, and another begin iterator of the second range. \
In our case, they are the vectors from which we want to calculate the error sum. \
The next character is the initial value val. We have initialized it to 0.0. \
Then, the algorithm accepts two binary functions, namely *bin_op1* and *bin_op2*.

At this point, we might realize that this algorithm is really similar to *std::accumulate*. \
The only difference is that *std::accumulate* works on only one range. \
If we exchange the *bin_op2(\*it1, \*it2)* statement with *\*it*, then we have basically restored the accumulate algorithm. \
We can, therefore, regard **std::inner_product** as a version of **std::accumulate** that zips a pair of input ranges.

In our case, the zipper function is *pow(a - b, 2)*, and that's it. \
For the other function, *bin_op1*, we chose *std::plus<double>* because we want all the squares to be summed together.
