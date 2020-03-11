### Type functions

#### Type Predicates
In **<type_traits>**, the standard library offers simple type functions, \
called *type predicates* that answers a fundamental question about types. \
For example:
```cpp
bool b1 = std::is_arithmetic<int>(); // yes, int is an arithmetic type
bool b2 = std::is_arithmetic<string>(); // no, std::string is not an arithmetic type
```

Other examples are **is_class**, **is_pod**, **is_literal_type**, **has_virtual_destructor**, and **is_base_of**. \
They are most useful when we write templates. \
For example:
```cpp
template<typename Scalar> 
class complex {
	Scalar re, im; 
public:
	static_assert(is_arithmetic<Scalar>(), "Sorry, I only support complex of arithmetic types");
	// ... 
};
```
To improve readability, the standard library defines template aliases. \
For example:
```cpp
template<typename T>
constexpr bool is_arithmetic_v = std::is_arithmetic<T>();
```

#### enable_if
Obvious ways of using type predicates includes conditions for **static_asserts**, compile-time **if**s, and **enable_ifs**. \
The standard-library **enable_if** is a widely used mechanism for conditonally introducing definitions. \
Consider defining a ‘‘smart pointer’’:
```cpp
template<typename T> 
class Smart_pointer {
	// ...
	T& operator∗(); 
	T& operator−>();  // -> should work if and only if T is a class
};
```
The **−>** should be defined if and only if **T** is a class type. \
For example, **Smart_pointer<vector<T>>** should have **−>**, but **Smart_pointer<int>** should not.
We cannot use a compile-time if because we are not inside a function. \
Instead, we write
```cpp
template<typename T> 
class Smart_pointer {
	// ...
	T& operator∗(); 
	std::enable_if<is_class<T>(),T&> operator−>();  // -> is defined if and only if T is a class
};
```
If **is_class<T>()** is true, the return type of **operator−>()** is **T&**; otherwise, the definition of **operator−>()** is ignored. \
**enable_if** is the basis for much current template metaprogramming and for many standard-library components. \
It relies on a subtle language feature called **SFINAE** (*‘‘**S**ubstitution **F**ailure **I**s **N**ot **A**n **E**rror’’*).