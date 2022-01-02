## Profiles

Out-of-source builds are also supported. Let’s look at a simple example:

```bash
$ git clone git@github.com:conan-io/examples
$ cd libraries/poco
$ conan install ./md5 --install-folder=md5_build
```
This will result in the following layout:

```
md5_build
    conaninfo.txt
    conanbuildinfo.txt
    conanbuildinfo.cmake
md5
    CMakeLists.txt  # If using cmake, but can be Makefile, sln...
    README.md
    conanfile.txt
    md5.cpp
```

Now you are ready to build:
```bash
$ cd md5_build
$ cmake ../md5 -G "Visual Studio 15 Win64"  # or other generator
$ cmake --build . --config Release
$ ./bin/md5
> c3fcd3d76192e4007dfb496cca67e13b
```

We have created a separate build configuration of the project without affecting the original source directory in any way. \
The benefit is that we can freely experiment with the configuration: \
We can clear the build folder and build another. 

## Multiconfiguration
```bash
$ git clone git@github.com:conan-io/examples
$ cd libraries/poco
$ conan install md5 -s build_type=Debug -if md5_build_debug
$ conan install md5 -s build_type=Release -if md5_build_release

$ cd md5_build_debug && cmake ../md5 -G "Visual Studio 15 Win64" && cd ../..
$ cd md5_build_release && cmake ../md5 -G "Visual Studio 15 Win64" && cd ../..
```

So the layout will be:
```
md5_build_debug
    conaninfo.txt
    conanbuildinfo.txt
    conanbuildinfo.cmake
    CMakeCache.txt # and other cmake files
md5_build_release
    conaninfo.txt
    conanbuildinfo.txt
    conanbuildinfo.cmake
    CMakeCache.txt # and other cmake files
example-poco-timer
    CMakeLists.txt  # If using cmake, but can be Makefile, sln...
    README.md
    conanfile.txt
    md5.cpp
```

