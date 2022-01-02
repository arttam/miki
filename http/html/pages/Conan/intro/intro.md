## Step one

[step 1](https://docs.conan.io/en/latest/getting_started.html#getting-started)

### Some things to note
1. Create conanfile.txt (best py, but it out of scope)
**conanfile.txt**
```
[requires]
poco/1.9.4

[generators]
cmake
```

2. Install dependencies
```bash
$ conan install <DIR_WHERE_conanfile_LOCATED>
...
```
In case of absense of binary packages:
```bash
$ conan install <DIR_WHERE_conanfile_LOCATED> --build=missing
...
```

3. Try it out
**CMakeLists.txt**
```
cmake_minimum_required(VERSION 3.16)
project(md5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} md5.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
```

**md5.cpp**
```cpp
 #include "Poco/MD5Engine.h"
 #include "Poco/DigestStream.h"

 #include <iostream>

 int main(int argc, char** argv){
     Poco::MD5Engine md5;
     Poco::DigestOutputStream ds(md5);
     ds << "abcdefghijklmnopqrstuvwxyz";
     ds.close();
     std::cout << Poco::DigestEngine::digestToHex(md5.digest()) << std::endl;
     return 0;
 }
```

```bash
$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
$ ./bin/md5
```

### Profile
Default profile: `~/.conan/profile/default`

Override profile settings (command line)
```bash
$ conan install .. --settings os="Linux" --settings compiler="gcc"

### See local packages
```bash
$ conan search "*"
Existing package recipes:

OpenSSL/1.1.1b@conan/stable
activemq_cpp/3.9.5.3@mirriad/testing
apache-apr/1.6.3@jgsogo/testing
apr/1.7.0
...
```

### Search for specific package (@)
```bash
$ conan search poco/1.9.4@
Existing packages for recipe poco/1.9.4:

Package_ID: 645aaff0a79e6036c77803601e44677556109dd9
    [options]
        cxx_14: False
        enable_apacheconnector: False
        enable_cppparser: False
        enable_crypto: True
        enable_data: True
...
```
@ - tells to search for specific package, intead of all having _poco/1.9.4_ in their name

### Inspect current dependencies
```bash
  conan info ..
conanfile.txt
    ID: fdfd8cd3a1e1be41911b62ff437dfafea2006cfe
    BuildID: None
    Context: host
    Requires:
        poco/1.9.4
bzip2/1.0.8
    ID: da606cf731e334010b0bf6e85a2a6f891b9f36b0
    BuildID: None
    Context: host
    Remote: None
    URL: https://github.com/conan-io/conan-center-index
    Homepage: http://www.bzip.org
    License: bzip2-1.0.8
    Description: bzip2 is a free and open-source file compression program that uses the Burrows Wheeler algorithm.
    Topics: conan, bzip2, data-compressor, file-compression
    Provides: bzip2
    Recipe: Cache
    Binary: Cache
    Binary remote: conancenter
    Creation date: 2021-05-03 17:49:26 UTC
    Required by:
        pcre/8.45
expat/2.4.1
    ID: c215f67ac7fc6a34d9d0fb90b0450016be569d86
    BuildID: None
    Context: host
    Remote: None
    URL: https://github.com/conan-io/conan-center-index
    Homepage: https://github.com/libexpat/libexpat
    License: MIT
    Description: Fast streaming XML parser written in C.
    Topics: expat, xml, parsing
    Provides: expat
    Recipe: Cache
    Binary: Cache
    Binary remote: conancenter
    Creation date: 2021-08-28 00:14:25 UTC
    Required by:
        poco/1.9.4
openssl/1.1.1k
    ID: 6af9cc7cb931c5ad942174fd7838eb655717c709
    BuildID: None
    Context: host
    Remote: None
    URL: https://github.com/conan-io/conan-center-index
    Homepage: https://github.com/openssl/openssl
    License: OpenSSL
    Description: A toolkit for the Transport Layer Security (TLS) and Secure Sockets Layer (SSL) protocols
    Topics: conan, openssl, ssl, tls, encryption, security
    Provides: openssl
    Recipe: Cache
    Binary: Cache
    Binary remote: conancenter
    Creation date: 2021-08-26 06:04:05 UTC
    Required by:
        poco/1.9.4
pcre/8.45
    ID: 87087120c448298530c012e627c1a0b8f062586d
    BuildID: None
    Context: host
    Remote: None
    URL: https://github.com/conan-io/conan-center-index
    Homepage: https://www.pcre.org
    License: BSD-3-Clause
    Description: Perl Compatible Regular Expressions
    Topics: regex, regexp, PCRE
    Provides: pcre
    Recipe: Cache
    Binary: Cache
    Binary remote: conancenter
    Creation date: 2021-08-18 09:53:33 UTC
    Required by:
        poco/1.9.4
    Requires:
        bzip2/1.0.8
        zlib/1.2.11
...
```

### Inspect current dependencies as graph
```bash
$   conan info .. --graph=/tmp/poko-depenencies.html
```

### Build with another configurations
```bash
$ conan install .. --profile=<PROFILE_FILE_PATH>
```
if above file located in `~/.conan/profiles` folder - just file name would be sufficient

### Overriding profile settings (command line)
```bash
$ conan install .. --settings arch=x86
```

