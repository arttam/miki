# Boost ASIO remarks
## Completion handlers
Completion events queue is linked list of completion handlers.
Async completion handlers can be written as in single threaded environment.

## Threading
Multiple threads may call io\_conrol::run() to setup pool of threads from which completion hanlers may be invoked.
This approach also mayh be used by calling post().

**strands** allows execution of code in multithreaded program without the need for explicit locking (e.g. using mutexes).
Strong guarantee that next handler will be not called before previous one finished task.

## Buffers
```cpp
mutable_buffer => std::pair<void*, size_t>;
const_buffer   => std::pair<const void*, size_t>;
```

mutable\_buffer to const\_buffer allowed, const\_buffer to mutable\_buffer don't.

Boost buffer auto determining size from an array, boost::array, std::vector of POD elements or from std::string.

```
**POD**: Plain Old Data; class without 
- user defined constructors 
- copy assignment operators 
- destructors
- virtual member functions. 

Also known as aggregate class

Scatter/Gather: Access non-continuous memory location as continuous one.
For example - large structures in memory, fragmented file on the disk.
Gather - calculate indexes and made them continuous
Scatter - populate gathered values back to original locations
```

** boost::asio::basic\_streambuf **
<table border='1'>
	<tr>
		<th>Method</th><th>Return type</th>
	</tr>
<tr><td>data()</td><td>ConstBufferSequence</td></tr>
<tr><td>prepare()</td><td>MutableBufferSequence</td></tr>
<tr><td>commit()</td><td>Transfer from front of output to back of input&nbsp;</td></tr>
<tr><td>consule()&nbsp;</td><td>Remove from front of the input</td></tr>
</table>

** Iterators **

buffer\_iterator, buffers\_begin(), buffers\_end() 
Allows consecutive access to basic\_streambuf members


