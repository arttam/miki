### Execution policies

C++17 came with one really *major* extension for parallelism: *execution policies* for standard algorithms. \
Sixty nine algorithms were extended to accept execution policies in order to run parallel on multiple cores, and even with enabled vectorization.

Sample invocation might look like:
```cpp
generate(execution::par, begin(d), end(d), rand_num);
sort(    execution::par, begin(d), end(d));
reverse( execution::par, begin(d), end(d));

auto odds (count_if(execution::par, begin(d), end(d), odd));
``` 
Not much has changed in terms of their interface - they all got an additional *ExecutionPolicy&&* policy argument, and that's it. \
This does not mean that we *always* have to provide an execution policy argument. \
It is just that they *additionally* support accepting an execution policy as their first argument.

These are the 69 upgraded standard algorithms. \
There are also the seven new ones that support execution policies from the beginning (highlighted in bold):

```cpp
std::adjacent_difference
std::adjacent_find
std::all_of
std::any_of
std::copy
std::copy_if
std::copy_n
std::count
std::count_if
std::equal
std::exclusive_scan
std::fill
std::fill_n
std::find
std::find_end
std::find_first_of
std::find_if
std::find_if_not
std::for_each
std::for_each_n
std::generate
std::generate_n
std::includes
std::inclusive_scan
std::inner_product

std::inplace_merge
std::is_heap
std::is_heap_until
std::is_partitioned
std::is_sorted
std::is_sorted_until
std::lexicographical_compare
std::max_element
std::merge
std::min_element
std::minmax_element
std::mismatch
std::move
std::none_of
std::nth_element
std::partial_sort
std::partial_sort_copy
std::partition
std::partition_copy
std::remove
std::remove_copy
std::remove_copy_if
std::remove_if
std::replace
std::replace_copy
std::replace_copy_if

std::replace_if
std::reverse
std::reverse_copy
std::rotate
std::rotate_copy
std::search
std::search_n
std::set_difference
std::set_intersection
std::set_symmetric_difference
std::set_union
std::sort
std::stable_partition
std::stable_sort
std::swap_ranges
std::transform
std::transform_exclusive_scan
std::transform_inclusive_scan
std::transform_reduce
std::uninitialized_copy
std::uninitialized_copy_n
std::uninitialized_fill
std::uninitialized_fill_n
std::unique
std::unique_copy

```

The more our old programs utilize STL algorithms, the easier we can add parallelism to them retroactively. \
Note that this does not mean that such changes make *every* program automatically *N* times faster because multiprogramming is quite a bit more complex than that.

However, instead of designing our own complicated parallel algorithms using *std::thread*, *std::async*, or by including external libraries, we can now parallelize standard tasks in a very elegant, operating system-independent way.

The execution policy tells which *strateg*y we allow for the automatic parallelization of our standard algorithm calls.\
The following three policy types exist in the *std::execution* namespace:
- *sequenced_policy* - The algorithm has to be executed in a sequential form similar to the original algorithm without an execution policy. \
The globally available instance has the name ***std::execution::seq***.
- *parallel_policy* - The algorithm may be executed with multiple threads that share the work in a parallel fashion. \
The globally available instance has the name ***std::execution::par***.
- *parallel_unsequenced_policy* - The algorithm may be executed with multiple threads sharing the work. \
In addition to that, it is permissible to vectorize the code. \
In this case, container access can be interleaved between threads and also within the same thread due to vectorization. \
The globally available instance has the name ***std::execution::par_unseq***.

The execution policies imply specific constraints for us. \
The stricter the specific constraints, the more parallelization strategy measures we can allow:
- All element access functions used by the parallelized algorithm ***must not cause*** *deadlocks* or *data races*
- In the case of parallelism and vectorization, all the access functions ***must not*** use any kind of *blocking synchronization*

All element access functions used by the parallelized algorithm must not cause deadlocks or data races \
In the case of parallelism and vectorization, all the access functions must not use any kind of blocking synchronization