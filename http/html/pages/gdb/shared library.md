## Debug shared libraries

Session from [gdb](https://visualgdb.com/gdbreference/commands/sharedlibrary)
```bash

(gdb) set auto-solib-add off
(gdb) break main
Breakpoint 1 at 0x80484ed: file main.cpp, line 7.
(gdb) run
Starting program: /home/testuser/libtest/testApp

Breakpoint 1, main () at main.cpp:7
7 printf("In main()\n");
(gdb) info sources
Source files for which symbols have been read in:

/home/testuser/libtest/main.cpp

Source files for which symbols will be read in on demand:


(gdb) info sharedlibrary
From To Syms Read Shared Object Library
0xb7fde820 0xb7ff6b9f No /lib/ld-linux.so.2
0xb7fd83a0 0xb7fd84c8 No /home/testuser/libtest/libTest.so
0xb7e30f10 0xb7f655cc No /lib/i386-linux-gnu/libc.so.6
(gdb) sharedlibrary libTest
Reading symbols from /home/testuser/libtest/libTest.so...done.
Loaded symbols for /home/testuser/libtest/libTest.so
(gdb) info sources
Source files for which symbols have been read in:

/home/testuser/libtest/main.cpp

Source files for which symbols will be read in on demand:

/home/testuser/libtest/lib.cpp
(gdb) break lib.cpp:5
Breakpoint 2 at 0xb7fd846e: file lib.cpp, line 5.
(gdb) continue
Continuing.
In main()

Breakpoint 2, func () at lib.cpp:5
5 printf("In func()\n");
```
