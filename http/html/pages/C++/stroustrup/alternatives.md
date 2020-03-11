### Alternatives

The standard library offers three types to express alternatives:
- **variant** to represent one of a specified set of alternatives (in <variant>)
- **optional** to represent a value of a specified type or no value (in <optional>)
- **any** to represent one of an unbounded set of alternative types (in <any>)

#### variant
A **variant<A,B,C>** is often a safer and more convenient alternative to explicitly using a **union**

```cpp
using Node = variant<Expression,Statement,Declaration,Type>;
void check(Node∗ p) {
    if (holds_alternative<Expression>(∗p)) { 
	Expression& e = get<Expression>(∗p); // ...
    }
    else if (holds_alternative<Statement>(∗p)) {
	Statement& s = get<Statement>(∗p); // ... 
    }
    // ... Declaration and Type ... }
```

Instead visitor pattern can be applied:
```cpp
void check(Node∗ p) 
{
    visit(overloaded { 
	[](Expression& e) { /* ... */ }, 
	[](Statement& s) { /* ... */ }, 
	// ... Declaration and Type ...
    }, ∗p); 
}
```
This is basically equivalent to a virtual function call, but potentially faster. \
Unfortunately, the overloaded is necessary and not standard. \
It’s a ‘‘piece of magic’’ that builds an overload set from a set of arguments (usually lambdas):

```cpp
template<class... Ts> 
struct overloaded : Ts... {
    using Ts::operator()...; 
};

template<class... Ts>
overloaded(Ts...) −> overloaded<Ts...>; // deduction guide
```
The ‘‘visitor’’ **visit** then applies () to the **overload**, which selects the most appropriate lambda to call according to the overload rules.  \
A *deduction guide* is a mechanism for resolving subtle ambiguities, primarily for constructors of class templates in foundation libraries.

#### optional
An **optional<A>** can be seen as a special kind of variant (like a **variant<A,nothing>**) or as a generalization of the idea of an **A**∗ either pointing to an object or being **nullptr**.\
An optional can be useful for functions that may or may not return an object.

#### any
An **any** can hold an arbitrary type and know which type (if any) it holds. \
It is basically an unconstrained version of **variant**:
```cpp
any compose_message(istream& s) {
    string mess;
    // ... read from s and compose message ...
    if (no_problems)
	return mess; // return a string
    else
	return error_number; // return an int
}
```
When you assign or initialize an any with a value, it remembers the type of that value. \
Later, we can inquire what type the any holds and extract the value. \
For example:
```cpp
    auto m = compose_message(cin)); 
    string& s = any_cast<string>(m); 
    cout << s;
```
If we try to access an any holding a different type than the expected one, **bad_any_access** is thrown.