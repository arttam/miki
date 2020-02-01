#### Custom allocator

Default allocator: **std::allocator<T>**.

Or:

```cpp

template <class T> 
    class my_allocator  
    { 
      typedef size_t    size_type; 
      typedef ptrdiff_t difference_type; 
      typedef T*        pointer; 
      typedef const T*  const_pointer; 
      typedef T&        reference; 
      typedef const T&  const_reference; 
      typedef T         value_type; 
 
      pointer allocate(size_type st, const void* hint = 0); 
      void deallocate (pointer p, size_type st); 
      void construct (pointer p, const_reference val); 
      void destroy (pointer p); 
      template <class U>  
      struct rebind { typedef allocator<U> other; }; 
    }; 

```
