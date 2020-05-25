### Conan - forcing compiler version

On machine with default gcc version 9, in order to force version 7 (assuming it is installed) 

./Conan/Linux_Release.profile
```
...
[env]
CC=/usr/bin/gcc-7
CXX=/usr/bin/g++-7
```
