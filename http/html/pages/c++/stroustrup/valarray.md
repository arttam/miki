### Vector Arithmetic

The vector was designed to be a general mechanism for holding values, to be flexible, \
and to fit into the architecture of containers, iterators, and algorithms. However, \
it does not support mathematical vector operations. Adding such operations to vector \
would be easy, but its generality and flexibility preclude optimizations that are often \
considered essential for serious numerical work. 

Consequently, the standard library provides (in **<valarray>**) a vector-like template, called valarray, \
that is less general and more amenable to optimization for numerical computation:
```cpp
template<typename T> 
class valarray {
	// ... 
};
```
The usual arithmetic operations and the most common mathematical functions are supported for valarrays. \
For example:
```cpp
void f(valarray<double>& a1, valarray<double>& a2) {
	valarray<double> a = a1∗3.14+a2/a1; // numeric array operators *, +, /, and = a2 += a1∗3.14;
	a = abs(a);
	double d = a2[7];
	// ... 
}
```
In addition to arithmetic operations, valarray offers stride access to help implement multidimen- sional computations.