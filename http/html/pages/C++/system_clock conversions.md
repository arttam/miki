## system_slock conversions (mine)

```cpp
long time2long(std::chrono::time_point<std::chrono::system_clock>& tp)
{
   auto asMS = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());

   return asMS.count();
}

std::chrono::time_point<std::chrono::system_clock> long2time(long secondsCount)
{
   std::chrono::milliseconds asMS(secondsCount);
   auto asSysClock = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(asMS);

   return asSysClock;
}

int main()
{
   std::cout << "Now conversions" << std::endl;

   auto curr = std::chrono::system_clock::now();
   long iVal = time2long(curr);
   std::cout << "As count(): " << curr.time_since_epoch().count() << ", As LONG: " << iVal << std::endl;
   
   auto back = long2time(iVal);
   std::cout << "Back: " << back.time_since_epoch().count() << std::endl;

   std::cout << "i.e. " << curr.time_since_epoch().count() << " == " << back.time_since_epoch().count() << " ???" << std::endl;

   return 0;
}
```

### Or in one-liner form
```cpp


long time2long(std::chrono::time_point<std::chrono::system_clock>& tp)
{
   return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
}

std::chrono::time_point<std::chrono::system_clock> long2time(long secondsCount)
{
   return std::chrono::time_point<std::chrono::system_clock>(std::chrono::milliseconds(secondsCount));
}
```

