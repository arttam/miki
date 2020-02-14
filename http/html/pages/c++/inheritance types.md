### Inheritance types

#### Access Control and Inheritance
A derived class can access all the non-private members of its base class. \
Thus base-class members that should not be accessible to the member functions of derived classes should be declared private in the base class.

We can summarize the different access types according to - who can access them in the following way −

Access                 public  protected  private
- Same class	        yes        yes          yes
- Derived classes	yes        yes           no
- Outside classes	yes         no            no

A derived class inherits all base class methods with the following exceptions −

- Constructors, destructors and copy constructors of the base class.
- Overloaded operators of the base class.
- The friend functions of the base class.
- Type of Inheritance

When deriving a class from a base class, the base class may be inherited through **public**, **protected** or **private** inheritance. \
The type of inheritance is specified by the access-specifier as explained above.

We hardly use **protected** or **private** inheritance, but public inheritance is commonly used. \
While using different type of inheritance, following rules are applied −

- Public Inheritance − When deriving a class from a public base class, public members of the base class become public members of the derived class and protected members of the base class become protected members of the derived class. A base class's private members are never accessible directly from a derived class, but can be accessed through calls to the public and protected members of the base class.
- Protected Inheritance − When deriving from a protected base class, public and protected members of the base class become protected members of the derived class.
- Private Inheritance − When deriving from a private base class, public and protected members of the base class become private members of the derived class.

#### Base pure virtual private function 

Weird, but this one works:
```cpp
#include <iostream>

class A {
	virtual void callMe() = 0;
};

class B : public A {
	virtual void callMe() override;

public:
	void callOverride();
};

void B::callMe()
{
	std::cout << "Calling override" << std::endl;
}

void B::callOverride()
{
	callMe();
}

int main()
{
	B b;
	b.callOverride();

	return 0;
}
```

According to StackOwerflow:
The question in the topic suggest a pretty common confusion. \
The confusion is common enough, that C++ FAQ advocated against using private virtuals, for a long time, because confusion seemed to be a bad thing.

So to get rid of the confusion first: Yes, private virtual functions can be overridden in the derived classes. \
Methods of derived classes can't call virtual functions from the base class, but they can provide their own implementation for them. \
According to Herb Sutter, having public non-virtual interface in the base class and a private implementation that can be customized \
in the derived classes, allows for better "separation of the specification of interface from the specification of the \
implementation's customizable behavior". 
