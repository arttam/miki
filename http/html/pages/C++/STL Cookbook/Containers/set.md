### std::set

**std::set** is a strange container: It kind of works like std::map, but it contains only keys as values, no key-value pairs. So it can hardly be used as a way to map values of one type to the other. 

```cpp
#include <iostream>
#include <set>
#include <string>
#include <iterator> // for ostream_iterator

using namespace std;

int main()
{
    set<string> s;

    istream_iterator<string> it {cin};
    istream_iterator<string> end;

    copy(it, end, inserter(s, s.end()));

    for (const auto word : s) {
        cout << word << ", ";
    }
    cout << '\n';
}

```
Output:
```bash
$ echo "a a a b c foo bar foobar foo bar bar" | ./program
a, b, bar, c, foo, foobar,
```

It's only ***one*** line of code which does all the work of *tokenizing* the input, putting it into the alphabetically sorted set, and *dropping* all duplicates ;-)

The *std::istream_iterator* takes one template parameter. That is the type of the input we want to have. \
Template parameter can basically be every type for which it is possible to write *cin >> var;*. \
The constructor accepts an *istream* instance. \
The standard input is represented by the global input stream object *std::cin*, which is an acceptable istream parameter in this case.

```cpp
istream_iterator<string> it {cin};
```

The input stream iterator *it* which we have instantiated, is able to do two things: 
- when it is dereferenced *(\*it)*, it yields the current input symbol. \
As we have typed the iterator to *std::string* via its template parameter, that symbol will be a string containing one word.  
- When it is incremented *(++it)*, it will jump to the next word, which we can access by dereferencing again.

We create the end iterator by creating an *std::istream_iterator* instance with its parameterless standard constructor.

```cpp
istream_iterator<string> end;
```

As soon as *std::cin* runs empty, that will make a comparison with *end* returning *true*.

The *std::inserter* it is a function which returns an *std::insert_iterator* that behaves like an iterator but does something else than what usual iterators do. \
When we increment it, it does nothing. When we dereference it and assign something to it, it will take the container it is attached to, and *insert* that value as a *new* item into it!

When instantiating an *std::insert_iterator* via *std::inserter*, two parameters are needed:

```cpp
auto insert_it = inserter(s, s.end());
```

The *s* is our set, and *s.end()* is an iterator that points to where the new item shall be inserted. \
For an empty set which we start with, this makes as much sense as *s.begin()*. \
When used for other data structures as vectors or lists, that second parameter is ***crucial*** for defining where the insert iterator shall insert new items.