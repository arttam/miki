### Visitor idea explained (for myself)

Uses multiple inheritence and virtual function call

#### Plain implementation 
```cpp
#include <iostream>
#include <variant>

struct A {
    void operator()()
    {
	std::cout << "A Stuct" << std::endl;
    };
};

struct B {
    void operator()()
    {
	std::cout << "B Stuct" << std::endl;
    };
};

template<class... TBases>
struct derivirations : TBases... {
    using TBases::operator()...;
};

int main()
{
    derivirations<A> da;
    derivirations<B> db;

    const auto runVariant([](std::variant<A, B>& vab) {
	if (std::holds_alternative<A>(vab)) {
	    A& av = std::get<A>(vab);
	    av();
	}
	else if (std::holds_alternative<B>(vab)) {
	    B& bv = std::get<B>(vab);
	    bv();
	}
	else {
	    std::cerr << "Unknown variant type" << std::endl;
	}
    });

    std::variant<A, B> vab;
    vab = da;
    runVariant(vab);

    vab = db;
    runVariant(vab);

    return 0;
}
```

Produces
```bash
./visitor
A Stuct
B Stuct
```

#### Elaborated implementation, using visitor pattern
```cpp
#include <iostream>
#include <variant>

struct A {
    void operator()()
    {
	std::cout << "A Stuct" << std::endl;
    };
};

struct B {
    void operator()()
    {
	std::cout << "B Stuct" << std::endl;
    };
};

template<typename... TBases>
struct derivirations : TBases... {
    using TBases::operator()...;
};

template<typename... TBases>
derivirations(TBases...)->derivirations<TBases...>;

int main()
{
    derivirations<A> da;
    derivirations<B> db;

    const auto abVisit([](std::variant<A, B>& vab) {
	std::visit(derivirations {
	    [](A& a) { a(); },
	    [](B& b) { b(); }
	}, vab);
    });

    std::variant<A, B> vab;
    vab = da;
    abVisit(vab);

    vab = db;
    abVisit(vab);

    return 0;
}
```

Produces same result

Without:
```cpp
template<typename... TBases>
derivirations(TBases...)->derivirations<TBases...>;
```
Compiler does not know which one to use
Called ***deduction guide***
More details in:https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
