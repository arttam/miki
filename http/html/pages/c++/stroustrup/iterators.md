### Iterators

Iterators are used to separate algorithms and containers. \
An algorithm operates on its data through iterators and knows nothing about the container in which the elements are stored. 
\
Conversely, a container knows nothing about the algorithms operating on its elements; \
all it does is to supply iterators upon request (e.g., **begin()** and **end()**). \
This model of separation between data storage and algorithm delivers very general and flexible software.

#### istream_iterator, ostream_iterator
Typically, istream_iterators and ostream_iterators are not used directly. Instead, they are provided as arguments to algorithms. For example, we can write a simple program to read a file, sort the words read, eliminate duplicates, and write the result to another file:

```cpp
int main() 
{
    string from, to; 
    cin >> from >> to;
    
    ifstream is {from}; 
    istream_iterator<string> ii {is}; 
    istream_iterator<string> eos {};   // input sentinel
    
    ofstream os {to}; 
    ostream_iterator<string> oo {os,"\n"};
    
    vector<string> b {ii,eos}; 
    sort(b.begin(),b.end());
    
    unique_copy(b.begin(),b.end(),oo); 
    
    return !is.eof() || !os;
}
```

Actually, this program is *longer than it needs to be*. 

```cpp
int main() 
{
    string from, to; 
    cin >> from >> to;
    
    ifstream is{from};
    ofstream os{to};
    
    set<string> b{istream_iterator<string>{is}, istream_iterator<string>{}};
    copy(b.begin(), b.end(), ostream_iterator<string>(os, "\n");
    
    return !is.eof() || !os;
}
```