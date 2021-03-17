## Simple Makefile reminder

```
$@ - target name
$< - first of target dependency
$^ - all target dependencies
```

### Having

```bash
╭─art@pop-os ~/Archive/Projects/tests/protobuf
╰─➤  ll
total 104K
-rw-rw-r-- 1 art art  36K Mar 15 19:44 addressbook.pb.cc
-rw-rw-r-- 1 art art  39K Mar 15 19:44 addressbook.pb.h
-rw-rw-r-- 1 art art  411 Mar 15 19:32 addressbook.proto
drwxrwxr-x 2 art art 4.0K Mar 16 16:24 bin
-rw-rw-r-- 1 art art  456 Mar 16 16:24 Makefile
drwxrwxr-x 2 art art 4.0K Mar 16 16:24 obj
-rw-rw-r-- 1 art art 2.0K Mar 15 22:23 reader.cpp
-rw-rw-r-- 1 art art 2.5K Mar 15 19:28 writer.cpp
```

### Makefile like below

```bash
CXX=g++
CFLAGS=-Wall -I/usr/local/include -pthread
LFLAGS=-L/usr/local/lib -lprotobuf

all: writer reader

reader: obj/reader.o obj/addressbook.o
	$(CXX) -o bin/$@ $^ $(LFLAGS)

writer: obj/writer.o obj/addressbook.o
	$(CXX) -o bin/$@ $^ $(LFLAGS)

obj/writer.o: writer.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

obj/reader.o: reader.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

obj/addressbook.o: addressbook.pb.cc
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr bin/* obj/*

```

### Produces

```bash
╭─art@pop-os ~/Archive/Projects/tests/protobuf
╰─➤  make
g++ -Wall -I/usr/local/include -pthread -c -o obj/writer.o writer.cpp
g++ -Wall -I/usr/local/include -pthread -c -o obj/addressbook.o addressbook.pb.cc
g++ -o bin/writer obj/writer.o obj/addressbook.o -L/usr/local/lib -lprotobuf
g++ -Wall -I/usr/local/include -pthread -c -o obj/reader.o reader.cpp
g++ -o bin/reader obj/reader.o obj/addressbook.o -L/usr/local/lib -lprotobuf
```

### Or even better
```bash
CXX=g++
CFLAGS=-Wall -I/usr/local/include -pthread
LFLAGS=-L/usr/local/lib -lprotobuf

all: writer reader

reader: obj/reader.o obj/addressbook.o
	$(CXX) -o bin/$@ $^ $(LFLAGS)

writer: obj/writer.o obj/addressbook.o
	$(CXX) -o bin/$@ $^ $(LFLAGS)

obj/%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

obj/%.o: addressbook.pb.cc
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr bin/* obj/*
```

