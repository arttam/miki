### Reference wrapper

(Right from cppreference site)

**std::reference_wrapper** is a class template that wraps a reference in a copyable, assignable object. \
It is frequently used as a mechanism to store references inside standard containers (like **std::vector**) which cannot normally hold references.

Specifically, **std::reference_wrapper** is a *CopyConstructible* and *CopyAssignable* wrapper around a reference to object or reference to function of type T. \
Instances of **std::reference_wrapper** are objects (they can be copied or stored in containers) but they are implicitly convertible to T&, so that they can be used as arguments with the functions that take the underlying type by reference.

If the stored reference is *Callable*, **std::reference_wrapper** is callable with the same arguments.

Helper functions **std::ref** and **std::cref** are often used to generate **std::reference_wrapper** objects.

**std::reference_wrapper** is also used to pass objects by reference to **std::bind**, the constructor of **std::thread**, \
or the helper functions **std::make_pair** and **std::make_tuple**.