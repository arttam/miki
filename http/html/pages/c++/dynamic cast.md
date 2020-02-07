#### Dynamic cast

C++ will implicitly let you convert a Derived pointer into a Base pointer.This process is sometimes called **upcasting**

Although dynamic casts have a few different capabilities, by far the most common use for dynamic casting is for converting base-class pointers into derived-class pointers. \
This process is called **downcasting**.

```cpp

int main()
{
	Base *b = getObject(true);
 
        Derived *d = dynamic_cast<Derived*>(b); // use dynamic cast to convert Base pointer into Derived pointer
 
        if (d) // make sure d is non-null
            std::cout << "The name of the Derived is: " << d->getName() << '\n';
 
	delete b;
 
	return 0;
}

```

or with references

```cpp

int main()
{
	Derived apple(1, "Apple"); // create an apple
	Base &b = apple; // set base reference to object
	Derived &d = dynamic_cast<Derived&>(b); // dynamic cast using a reference instead of a pointer
 
	std::cout << "The name of the Derived is: " << d.getName() << '\n'; // we can access Derived::getName through d
 
	return 0;
}

```

Also note that there are several cases where downcasting using dynamic_cast will not work:
1) With protected or private inheritance.
2) For classes that do not declare or inherit any virtual functions (and thus don’t have a virtual table).
3) In certain cases involving virtual base classes


#### Downcasting with static_cast

It turns out that downcasting can also be done with static_cast. \
The main difference is that static_cast does no runtime type checking to ensure that what you’re doing makes sense. \
This makes using static_cast faster, but more dangerous. \
If you cast a Base* to a Derived*, it will “succeed” even if the Base pointer isn’t pointing to a Derived object. \
This will result in undefined behavior when you try to access the resulting Derived pointer (that is actually pointing to a Base object).

