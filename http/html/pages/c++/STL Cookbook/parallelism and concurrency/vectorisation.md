### Vectorisation

Vectorization is a feature that both the CPU and the compiler need to support. \
Let's have a quick glance at a simple example to briefly understand what vectorization is and how it works. \
Imagine we want to sum up numbers from a very large vector. \
A plain implementation of this task can look like this:
```cpp
std::vector<int> v {1, 2, 3, 4, 5, 6, 7 /*...*/};

int sum {std::accumulate(v.begin(), v.end(), 0)};
```

The compiler will eventually generate a loop from the accumulate call, which could look like this:
```cpp
int sum {0};
for (size_t i {0}; i < v.size(); ++i) {
    sum += v[i];
}
```
Proceeding from this point, with vectorization allowed and enabled, the compiler could then produce the following code. \
The loop does four accumulation steps in one loop step and also does four times fewer iterations. \
For the sake of simplicity, the example does not deal with the remainder if the vector does not contain N * 4 elements:
```cpp
int sum {0};
for (size_t i {0}; i < v.size() / 4; i += 4) {
    sum += v[i] + v[i+1] + v[i + 2] + v[i + 3];
}
// if v.size() / 4 has a remainder, 
// real code has to deal with that also.
```
Why should it do this? \
Many CPUs provide instructions that can perform mathematical operations such as *sum += v[i] + v[i+1] + v[i + 2] + v[i + 3];* in just ***one*** step. \
Pressing *as many mathematical operations* into *as few instructions as possible* is the target because this speeds up the program.

Automatic vectorization is hard because the compiler needs to understand our program to some degree in order to make our program faster but without tampering with its correctness. At least, we can help the compiler by using standard algorithms as often as possible because those are easier to grasp for the compiler than complicated handcrafted loops with complex data flow dependencies.