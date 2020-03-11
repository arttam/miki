## Standard(s) features

### Language

#### C++11
1. Uniform and general initialization using {}-lists (§1.4, §4.2.3)
1. Type deduction from initializer: auto (§1.4)
1. Prevention of narrowing (§1.4)
1. Generalized and guaranteed constant expressions: constexpr (§1.6)
1. Range-for-statement (§1.7)
1. Nullpointerkeyword:nullptr(§1.7)
1. Scoped and strongly typed enums: enum class (§2.5) Compile-timeassertions:static_assert(§3.5.5)
1. Language mapping of {}-list to std::initializer_list (§4.2.3)
1. Rvalue references, enabling move semantics (§5.2.2)
1. Nested template arguments ending with >> (no space between the >s)
1. Lambdas(§6.3.2)
1. Variadic templates (§7.4)
1. Type and template aliases (§6.4.2)
1. Unicodecharacters
1. long long integer type
1. Alignmentcontrols:alignasandalignof
1. The ability to use the type of an expression as a type in a declaration: decltype
1. Raw string literals (§9.4)
1. Generalized POD (‘‘Plain Old Data’’)
1. Generalizedunions
1. Local classes as template arguments
1. Suffix return type syntax
1. A syntax for attributes and two standard attributes: [[carries_dependency]] and [[noreturn]] Preventing exception propagation: the noexcept specifier (§3.5.1)
1. Testing for the possibility of a throw in an expression: the noexcept operator.
1. C99 features: extended integral types (i.e., rules for optional longer integer types); con- catenation of narrow/wide strings; \_\_STDC\_HOSTED\_\_; \_Pragma(X); vararg macros and empty macro arguments
1. \_\_func\_\_ as the name of a string holding the name of the current function
1. inline namespaces
1. Delegating constructors
1. In-class member initializers (§5.1.3)
1. Control of defaults: default and delete(§5.1.1)
1. Explicit conversion operators
1. User-defined literals (§5.4.4)
1. More explicit control of template instantiation: extern templates
1. Default template arguments for function templates
1. Inheriting constructors
1. Override controls: override and final(§4.5.1)
1. A simpler and more general SFINAE (Substitution Failure Is Not An Error) rule
1. Memorymodel(§15.1)
1. Thread-local storage:thread_local

#### C++14
1. Function return-type deduction; §3.6.2
2. Improved constexpr functions, e.g., for-loops allowed (§1.6)
3. Variable templates (§6.4.1)
4. Binary literals(§1.4)
5. Digit separators(§1.4)
6. Generic lambdas(§6.3.3)
7. More general lambda capture
8. deprecated.. attribute
9. A few more minor extensions

#### C++17
1. Guaranteed copy elision(§5.2.2)
2. Dynamic allocation of over-aligned types
3. Stricter order of evaluation (§1.4)
4. UTF-8literals(u8)
5. Hexadecimal floating-point literals
6. Fold expressions (§7.4.1)
7. Generic value template arguments (auto template parameters)
8. Class template argument type deduction (§6.2.3)
9. Compile-time if(§6.4.3)
10. Selection statements with initializers (§1.8)
11. constexpr lambdas
12. inline variables
13. Structured bindings(§3.6.3)
14. New standard attributes: fallthrough..,nodiscard..,andmaybe_unused..
15. std::byte type
16. Initialization of an enum by a value of its underlying type (§2.5)
17. A few more minor extensions

### Standard-Library Components

#### C++11
1. initializer_list constructors for containers (§4.2.3)
2. Move semantics for containers (§5.2.2, §11.2)
3. A singly-linked list:forward_list(§11.6)
4. Hash containers:unordered_map,unordered_multimap,unordered_set,and unordered_multiset (§11.6, §11.5)
5. Resource management pointers:unique_ptr,shared_ptr,and weak_ptr(§13.2.1)
6. Concurrency support: thread (§15.2), mutexes (§15.5), locks (§15.5), and condition variables (§15.6)
7. Higher-level concurrency support:packaged_thread,future,promise,and async()(§15.7)
8. tuples (§13.4.3)
9. Regular expressions:regex(§9.4)
10. Random numbers: distributions and engines (§14.5)
11. Integer type names, such as int16_t, uint32_t, and int_fast64_t
12. A fixed-sized contiguous sequence container: array (§13.4.1)
13. Copying and rethrowing exceptions (§15.7.1)
14. Error reporting using error codes: system_error
15. emplace() operations for containers (§11.6)
16. Wide use of constexpr functions
17. Systematic use of noexcept functions
18. Improved function adaptors:function and bind()(§13.8)
19. string to numeric value conversions
20. Scoped allocators
21. Type traits, such as is_integral and is_base_of (§13.9.2)
22. Time utilities:duration and time_point(§13.7)
23. Compile-time rational arithmetic:ratio
24. Abandoning a process:quick_exit
25. More algorithms, such as move(), copy_if(), and is_sorted() (Chapter 12)
26. Garbage collection API (§5.3)
27. Low-level concurrency support:atomics

#### C++14
1. shared_mutex (§15.5)
2. User-defined literals (§5.4.4)
3. Tuple addressing by type (§13.4.3)
4. Associative container heterogenous lookup
5. A few more minor features

#### C++17
1. Filesystem(§10.10)
2. Parallel algorithms (§12.9, §14.3.1)
3. Mathematical special functions (§14.2)
4. string_view (§9.3)
5. any (§13.5.3)
6. variant (§13.5.1)
7. optional (§13.5.2)
8. invoke() Elementarystringconversions:to_charsandfrom_chars Polymorphicallocator(§13.6)
9. A few more minor extensions

### Removed and Deprecated features
- C++17 finally removed exceptions specifications:\
void f() throw(X,Y); // C++98; now an error\
The support facilities for exception specifications, \
**unexcepted_handler**, **set_unexpected()**, **get_unexpected()**, and **unexpected()**, are similarly removed. \
Instead, use **noexcept** (§3.5.1).
- Trigraphs are no longer supported.
- The auto_ptr is deprecated. Instead, use unique_ptr (§13.2.1).
- The use of the storage specifier register is removed.
- The use of ++ on a bool is removed.
- The C++98 export feature was removed because it was complex and not shipped by the
major vendors. Instead, export is used as a keyword for modules (§3.3).
- Generation of copy operations is deprecated for a class with a destructor (§5.1.1).
- Assignment of a string literal to a char∗ is removed. Instead use const char∗ or auto.
- Some C++ standard-library function objects and associated functions are deprecated. Most
relate to argument binding. Instead use lambdas and function (§13.8).