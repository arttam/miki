## Conan install remote package

Following scenario to install remote package on system \
First attempt - **wrong** one, as it uses default profile
```bash
D:\builds\Release-vs2019>conan install apr/1.7.0@_/_
Configuration:
[settings]
arch=x86_64
arch_build=x86_64
build_type=Release
compiler=Visual Studio
compiler.runtime=MD
compiler.version=15
os=Windows
os_build=Windows
[options]
[build_requires]
[env]

apr/1.7.0: Not found in local cache, looking in remotes...
apr/1.7.0: Trying with 'mirriad-artifactory'...
Downloading conanmanifest.txt completed [0.60k]
Downloading conanfile.py completed [5.82k]
Downloading conan_export.tgz completed [0.37k]
Decompressing conan_export.tgz completed [0.00k]
apr/1.7.0: Downloaded recipe revision 0
Installing package: apr/1.7.0
Requirements
    apr/1.7.0 from 'mirriad-artifactory' - Downloaded
Packages
    apr/1.7.0:e0817053b6cce1afc89704f330a2df1cc1023e30 - Missing

Installing (downloading, building) binaries...
ERROR: Missing binary: apr/1.7.0:e0817053b6cce1afc89704f330a2df1cc1023e30

apr/1.7.0: WARN: Can't find a 'apr/1.7.0' package for the specified settings, options and dependencies:
- Settings: arch=x86_64, build_type=Release, compiler=Visual Studio, compiler.runtime=MD, compiler.version=15, os=Windows
- Options: force_apr_uuid=True, shared=False
- Dependencies:
- Requirements:
- Package ID: e0817053b6cce1afc89704f330a2df1cc1023e30

ERROR: Missing prebuilt package for 'apr/1.7.0'
Try to build from sources with "--build=apr"
Or read "http://docs.conan.io/en/latest/faq/troubleshooting.html#error-missing-prebuilt-package"
```

**Correct way to do it**
```bash
C:\Users\arturs.tamanis>conan install apr/1.7.0@_/_ -pr c:\ZoneSense\zonesense\Conan\Windows_vs2017_Release.conan.profile --build=apr
Configuration:
[settings]
arch=x86_64
arch_build=x86_64
build_type=Release
compiler=Visual Studio
compiler.runtime=MD
compiler.version=15
os=Windows
os_build=Windows
...

apr/1.7.0: Forced build from source
Installing package: apr/1.7.0
Requirements
    apr/1.7.0 from 'mirriad-artifactory' - Cache
Packages
    apr/1.7.0:e0817053b6cce1afc89704f330a2df1cc1023e30 - Build

Installing (downloading, building) binaries...
apr/1.7.0: WARN: Build folder is dirty, removing it: D:\ConanData\apr\1.7.0\_\_\build\e0817053b6cce1afc89704f330a2df1cc1023e30
apr/1.7.0: Copying sources to build folder
apr/1.7.0: Building your package in D:\ConanData\apr\1.7.0\_\_\build\e0817053b6cce1afc89704f330a2df1cc1023e30
apr/1.7.0: Generator cmake created conanbuildinfo.cmake
apr/1.7.0: Calling build()
-- Selecting Windows SDK version 10.0.19041.0 to target Windows 10.0.19042.
-- The C compiler identification is MSVC 19.16.27045.0
-- The CXX compiler identification is MSVC 19.16.27045.0
...
apr/1.7.0 package(): Packaged 40 '.h' files
apr/1.7.0 package(): Packaged 1 '.lib' file: apr-1.lib
apr/1.7.0 package(): Packaged 1 file: LICENSE
apr/1.7.0: Package 'e0817053b6cce1afc89704f330a2df1cc1023e30' created
apr/1.7.0: Created package revision e6b6fe0a3162dd35c6c038695b5fb752
apr/1.7.0: Settings APR_ROOT environment var: /mnt/d/ConanData/apr/1.7.0/_/_/package/e0817053b6cce1afc89704f330a2df1cc1023e30

```