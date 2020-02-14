### Function Adaption

When passing a function as a function argument, the type of the argument must exactly match the expectations \
expressed in the called function’s declaration. \
If the intended argument ‘‘almost matches expectations,’’ we have three good alternatives:
* Use a lambda 
* Use **std::mem_fn()** to make a function object from a member function 
* Define the function to accept a **std::function**

#### Lambdas as Adaptors
```cpp
void draw_all(vector<Shape∗>& v) 
{
	for_each(v.begin(),v.end(),[](Shape∗ p) { p−>draw(); }); 
}
```
Like all standard-library algorithms, **for_each()** calls its argument using the traditional \
function call syntax **f(x)**, but Shape's draw() uses the conventional OO notation x−>f(). \
A lambda easily mediates between the two notations.

#### mem_fn()
Given a member function, the function adaptor **mem_fn(mf)** produces a function object that can be called as a nonmember function. \
For example:
```cpp	
void draw_all(vector<Shape∗>& v) {
	for_each(v.begin(),v.end(),mem_fn(&Shape::draw)); 
}
```

#### function
The standard-library function is a type that can hold any object you can invoke using the call \
**operator ()**. That is, an object of type **function** is a function object.3.2). \
For example: 
```cpp
int f1(double);
function<int(double)> fct1 {f1};
int f2(string); 
function fct2 {f2};
function fct3 = [](Shape∗ p) { p−>draw(); };
```
For **fct2**, I let the type of the function be deduced from the initializer: **int(string)**.