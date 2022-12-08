# Profile with gcc and gprof

1. Build with generating profile information option
```
cmake -DCMAKE_CXX_FLAGS=-pg -B build .
cmake --build ./build
```

2. Run once
```
./build/<app>
```
Notice that **gmon.out** file was generated in running from folder

3. Examine
```
gprof ./build/<app> gmon.out
```