## Workflows

Having:
```bash
$  tree .
.
└── src
    ├── CMakeLists.txt
    ├── conanfile.py
    └── md5.cpp
```

Following command:
```bash
$  conan install ./src -s build_type=Debug --install-folder md5_build_debug 
# Or with shorter --install-folder option
$  conan install ./src -s build_type=Release -if md5_build_release
```

Produces:
```bash
$ tree .
.
├── md5_build_debug
│   ├── conanbuildinfo.cmake
│   ├── conanbuildinfo.txt
│   ├── conaninfo.txt
│   ├── conan.lock
│   └── graph_info.json
├── md5_build_release
│   ├── conanbuildinfo.cmake
│   ├── conanbuildinfo.txt
│   ├── conaninfo.txt
│   ├── conan.lock
│   └── graph_info.json
└── src
    ├── CMakeLists.txt
    ├── conanfile.py
    └── md5.cpp
```

Then builds as simple as:
```bash
$  cd md5_build_debug; cmake ../src; cmake --build .; cd ..
$  cd md5_build_release; cmake ../src; cmake --build .; cd ..

# Produces
$ find . -type f -name md5 -exec ls -lhA {} \;
-rwxrwxr-x 1 art art 556K Oct  8 16:23 ./md5_build_debug/bin/md5
-rwxrwxr-x 1 art art 203K Oct  8 16:23 ./md5_build_release/bin/md5
```
