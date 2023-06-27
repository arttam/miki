## CRTP ( Curious Recurring Template Polymorphism )

In short, CRTP is when a class A has a base class which is a template specialization for the class A itself.  
Or  
CRTP is a technique to implement compile-time polymorphism. 

```cpp
template <class T> 
class X{...};
class A : public X<A> {...};
```

It is curiously recurring, isn't it? :)  

Now, what does this give you? This actually gives the X template the ability to be a base class for its specializations.  

For example, you could make a generic singleton class (simplified version) like this  
```cpp
template <class ActualClass> 
class Singleton
{
   public:
     static ActualClass& GetInstance()
     {
       if(p == nullptr)
         p = new ActualClass;
       return *p; 
     }

   protected:
     static ActualClass* p;
   private:
     Singleton(){}
     Singleton(Singleton const &);
     Singleton& operator = (Singleton const &); 
};
template <class T>
T* Singleton<T>::p = nullptr;
```

Now, in order to make an arbitrary class A a singleton you should do this

```cpp
class A: public Singleton<A>
{
   //Rest of functionality for class A
};
```

So you see? The singleton template assumes that its specialization for any type X will be inherited from singleton<X> and thus will have all its (public, protected) members accessible, including the GetInstance!    


Here you can see a great example.   
If you use virtual method the program will know what execute in runtime.   
Implementing CRTP the compiler is which decide in compile time!!! This is a great performance!

```cpp
template <class T>
class Writer
{
  public:
    Writer()  { }
    ~Writer()  { }

    void write(const char* str) const
    {
      static_cast<const T*>(this)->writeImpl(str); //here the magic is!!!
    }
};


class FileWriter : public Writer<FileWriter>
{
  public:
    FileWriter(FILE* aFile) { mFile = aFile; }
    ~FileWriter() { fclose(mFile); }

    //here comes the implementation of the write method on the subclass
    void writeImpl(const char* str) const
    {
       fprintf(mFile, "%s\n", str);
    }

  private:
    FILE* mFile;
};


class ConsoleWriter : public Writer<ConsoleWriter>
{
  public:
    ConsoleWriter() { }
    ~ConsoleWriter() { }

    void writeImpl(const char* str) const
    {
      printf("%s\n", str);
    }
};
```

