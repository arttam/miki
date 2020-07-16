## YouCompleteMe on Windows

Have version "Visual Studio 15 2017", when tried standrad build got error:
```bash

PS C:\Users\arturs.tamanis\.vim\bundle\YouCompleteMe> python .\install.py --clang-completer
Searching Python 3.8 libraries...
Found Python library: C:\Python38\libs\python38.lib
Found Python headers folder: C:\Python38\Include
CMake Error at CMakeLists.txt:26 (project):
  Generator

    Visual Studio 16 2019

  could not find any instance of Visual Studio.



-- Configuring incomplete, errors occurred!
See also "C:/Users/arturs.tamanis/AppData/Local/Temp/ycm_build_px9c2ajk/CMakeFiles/CMakeOutput.log".
ERROR: the build failed.
```


When issued as follows, everything works:
```bash
python install.py --clang-completer --msvc=15
```