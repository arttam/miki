### inner_product

```cpp
#include <numeric>
#include <iostream>
#include <vector>
#include <functional>
int main()
{
    std::vector<int> a{0, 1, 2, 3, 4};
    std::vector<int> b{5, 4, 2, 3, 1};
 
    int r1 = std::inner_product(a.begin(), a.end(), b.begin(), 0);
    std::cout << "Inner product of a and b: " << r1 << '\n';
 
    int r2 = std::inner_product(a.begin(), a.end(), b.begin(), 0,
                                std::plus<>(), std::equal_to<>());
    std::cout << "Number of pairwise matches between a and b: " <<  r2 << '\n';
}
```
Gives:
```bash
Inner product of a and b: 21
Number of pairwise matches between a and b: 2
```

### Possible implementations

#### First version
```cpp
template<class InputIt1, class InputIt2, class T>
constexpr // since C++20
T inner_product(InputIt1 first1, InputIt1 last1,
                InputIt2 first2, T init)
{
    while (first1 != last1) {
         init = std::move(init) + *first1 * *first2; // std::move since C++20
         ++first1;
         ++first2;
    }
    return init;
}
```

#### Second version
```cpp
template<class InputIt1, class InputIt2,
         class T,
         class BinaryOperation1, class BinaryOperation2>
constexpr // since C++20
T inner_product(InputIt1 first1, InputIt1 last1,
                InputIt2 first2, T init,
                BinaryOperation1 op1
                BinaryOperation2 op2)
{
    while (first1 != last1) {
         init = op1(std::move(init), op2(*first1, *first2)); // std::move since C++20
         ++first1;
         ++first2;
    }
    return init;
}
```