### By hand compilation - link libraries order

#### With object file

1. Output binary
1. Object file
1. Library folder (-L)
1. Library dependency

```bash
g++ -o bin/byhand bin/byhand.o -L ./lib -lactivemq-cpp
```

#### Without object file (all in one line)

```bash
g++ -o bin/byhand -I./include/activemq-cpp-3.9.5 src/examples/main.cpp -pthread -L./lib -lactivemq-cpp
```

## OR
```bash
g++ -Wall -std=c++17 real-cpp.cpp -lncurses++w -lformw -lmenuw -lpanelw -lncursesw -o list-picker
```
I.e. comiler options, cpp file, link options, output
