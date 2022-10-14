# overload explained, very nice !!!!!

I'd like to add to the great answers here with a bit of a history lesson.  

There are a lot of layers going on here, so let's peel them back one by one.  

1. variadic templates (C++11)
1. parameter packs
1. pack expansion
1. the using declaration
1. for introducing base class members
1. variadic using declaration (C++17)
1. template deduction guides (C++17)

## Variadic Templates

Pre-C++11, we were limited on the number of template arguments a function could receive by how much the programmers were willing to type.  

For example, if I wanted to write a function to sum up an "arbitrary" number of values for potentially different types,   
I needed to write a whole lot of boilerplate, and even then I was limited:  

```cpp
template<class T>
void foo(T){}

template<class T, class U>
void foo(T, U){}

template<class T, class U, class V>
void foo(T, U, V){}

// ... and so on until I decide enough's enough
```

In C++11 we finally received "variadic templates", which means we can receive an "unlimited" (actual limit determined by your compiler)   
number of template arguments by using an ellipsis (...), so now we can write  

```cpp
template<class... T>
void foo(T... args){}
```
This "unlimited number" of template arguments, class... T, is called a "parameter pack" because it's unsurprisingly representing a pack of parameters.  

To "unpack" those parameters into a comma-separated list, we use the ellipsis again in the function parameter list: void foo(T... args){}.   
This is called pack expansion, again, not a surprising name.  

The result of pack expansion for a function call like this:  
```cpp
int a = /*...*/;
double b = /*...*/;
char c = /*...*/;
foo(a, b, c);
```
Can be thought of like this:
```cpp
template<int, double, char>
void foo(Arguments[3] args){}
```
Where **Arguments** is a kind of heterogeneous array of (**int**, **double**, **char**).  

These variadic templates also apply to class and struct templates, so the analog here is that  
```cpp
template<class... Ts> struct overloaded
```
declares a class **overloaded** that can be templated on an "unlimited" number of types.  

The **: Ts...** portion of it:  
```cpp

template<class... Ts> struct overloaded : Ts...
```

uses pack expansion to declare the class **overloaded** to derive (potentially via multiple inheritance) from each of those types.  

## The using declaration
Pre-C++11 we could declare type aliases with a typedef like so:  
```cpp
typedef unsigned int uint;
```
In C++11 we received the using statement that can do the same thing, perhaps a little more clearly (and so much more! just hang on)  
```cpp
using uint = unsigned int;
```
However, a using statement was originally used for something different (its usage has expanded greatly since the introduction of C++11).  
One of the main reasons it was created was so that we could re-use things from base classes in derived classes without forcing the client to disambiguate:

Without **using**   
```cpp
struct does_a_thing
{
    void do_a_thing(double){}
};

struct also_does_a_thing
{
    void do_a_thing(int){}
};

struct derived : does_a_thing, also_does_a_thing{};

int main(){
    derived d;
    d.do_a_thing(1); // ? which "do_a_thing gets called? Neither, because it's ambiguous, so there's a compiler error
    d.does_a_thing::do_a_thing(42.0);
    d.also_does_a_thing::do_a_thing(1);
    
}

```
Note that the client is forced to write some funky syntax to refer to which base of derived they want to use for the call to do_a_thing.   
This looks nicer if we take advantage of using:

With **using** :
```cpp

struct derived : does_a_thing, also_does_a_thing
{
    using does_a_thing::do_a_thing;
    using also_does_a_thing::do_a_thing;
};

int main(){
    derived d;
    d.do_a_thing(1); // calls also_does_a_thing::do_a_thing
}
```

Cleaner, right?  

##Variadic **using** declaration
So C++11 came out and we were all impressed by these new features, but there was one small gap   
for using statements that wasn't addressed;   
"What if I want to have a using for each base class, where those base classes are template arguments?"

So something like this:  

```cpp
template<class T, class U>
struct derived : T, U
{
    using T::do_a_thing;
    using U::do_a_thing;
};

int main(){
    derived<does_a_thing, also_does_a_thing> d;
    d.do_a_thing(1); // calls also_does_a_thing::do_a_thing
}
```

So far, so good. But since we learned about **variadic templates**, let's make *derived* one:  

```cpp
template<class... Ts>
struct derived : Ts...
{
   //using ?
};
```

At the time, using was handicapped by its lack of variadic support, so we couldn't do this (easily).  

Then C++17 came along and gave us variadic using support so that we could do it:  

```cpp
template<class... Ts>
struct derived : Ts...
{
   using Ts::do_a_thing...;
};

int main(){
    derived<does_a_thing, also_does_a_thing> d;
    d.do_a_thing(1); // calls also_does_a_thing::do_a_thing
    d.do_a_thing(42.0); //calls does_a_thing::do_a_thing
}
```

## We can finally understand the first part of your code!  
So now we can finally understand the entirety of this part of the question:  

```cpp
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...;};
```

We have a class named overloaded that is templated on an "unlimited" number of types.   
It derives from each of those types. And it also allows you to use the operator() method of each of those parent types.   
Convenient, right? (Note that if any of the base class' operator() looked the same, we'd get an error.)

## Template deduction guides
One other thing that has bugged C++ developers for a while is that if you had a templated class that also had a templated constructor,   
you had to explicitly specify template arguments even when you thought it was obvious to yourself and your client what the template type should be.  


For example, I will want to write a lightweight iterator wrapper:  
```cpp
template<class T>
struct IteratorWrapper
{
    template<template<class...> class Container, class... Args>
    IteratorWrapper(const Container<Args...>& c)
    {
        // do something with an iterator on c
        T begin = c.begin();
        T end = c.end();
        while(begin != end)
        {
            std::cout << *begin++ << " ";
        } 
    } 
};
```

Now if I as the caller wanted to create an instance of IteratorWrapper,   
I had to do some extra legwork to disambiguate exactly what T was because it's not included in the signature of the constructor:
```cpp
std::vector<int> vec{1, 2, 3};
IteratorWrapper<typename std::vector<int>::const_iterator> iter_wrapper(vec);
```

Nobody wants to write that monstrosity. So C++17 introduced deduction guides where us as the class writer could do a little extra work so that the client wouldn't have to.   
Now I as the class author can write this:
```cpp
template<template<class...> class Container, class... Args>
IteratorWrapper(const Container<Args...>& c) -> IteratorWrapper<typename Container<Args...>::const_iterator>;
```
Which mimics the signature of IteratorWrappers constructor and then uses a trailing arrow (->) to indicate the type of ItearatorWrapper to deduce.  

So now my clients can write code like this:  
```cpp
std::vector<int> vec{1, 2, 3};
IteratorWrapper iter_wrapper(vec);

std::list<double> lst{4.1, 5.2};
IteratorWrapper lst_wrapper(lst);
```

Beautiful, right?  

## We can now understand the second line of code
```cpp
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
```

Declares a template deduction guide for our class overloaded that says that when its constructor is called with a parameter pack,   
then the class should also be templated on those same types.

That might sound unnecessary, but you might want it if you had a templated class with a templated constructor:  
```cpp
template<class... T>
struct templated_ctor{
    template<class... U>
     overloaded(U...){}
};
```

