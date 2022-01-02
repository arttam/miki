## Conan create

If your have following error
```bash
D:\Sources\conan-center-index\recipes\bzip2\all>conan create . bzpi2/by-hand
ERROR: conanfile didn't specify version
```

There is reason for it: \
Indeed the version is not listed in the recipe, but why? \
Because the same recipe is re-used for any version, \
so Conan Center Index don't need to replicate the same recipe for each new version.

All versions supported by Boost are listed in conandata.yml, \
which is a file with the download link and the checksum, \
according the version.

Thus, to build the desired version, you have to pass it with the command like. For instance, to build Boost 1.73:

```bash
cd recipes/boost/all
conan create . 1.73.0@
```

Note that I only passed the version, not the namespace (username/channel), \
because it's an official recipe from Conan Center Index, \
any other recipe should contain the namespace to avoid any conflict. \
In this case, you can use your namespace too, if you want:

```bash
cd recipes/boost/all
conan create . 1.73.0@hdnn/stable
```

The version in recipe is not mandatory, even without conandata.yml. \
When any mandatory attribute is missing (name or version), \
you can pass them by command line.


So, in my case:
```bash
D:\Sources\conan-center-index\recipes\bzip2\all>type conandata.yml
sources:
  "1.0.8":
    url: "https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz"
    sha256: "ab5a03176ee106d3f0fa90e381da478ddae405918153cca248e682cd0c4a2269"
  "1.0.6":
    url: "https://sourceware.org/pub/bzip2/bzip2-1.0.6.tar.gz"
    sha256: "a2848f34fcd5d6cf47def00461fcb528a0484d8edef8208d6d2e2909dc61d9cd"
patches:
  "1.0.6":
    - patch_file: "patches/0001-fix-sys-stat-include.patch"
      base_path: "source_subfolder"

D:\Sources\conan-center-index\recipes\bzip2\all>conan create . 1.0.8@
Exporting package recipe
bzip2/1.0.8 exports: File 'conandata.yml' found. Exporting it...
bzip2/1.0.8 exports: Copied 1 '.yml' file: conandata.yml
bzip2/1.0.8 exports_sources: Copied 1 '.txt' file: CMakeLists.txt
bzip2/1.0.8 exports_sources: Copied 1 '.patch' file: 0001-fix-sys-stat-include.patch
bzip2/1.0.8: A new conanfile.py version was exported
bzip2/1.0.8: Folder: D:\ConanData\bzip2\1.0.8\_\_\export
bzip2/1.0.8: Using the exported files summary hash as the recipe revision: 0b6ab9e3385e8fce7a544b3f8c90f4be
bzip2/1.0.8: Exported revision: 0b6ab9e3385e8fce7a544b3f8c90f4be
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

bzip2/1.0.8: Forced build from source
bzip2/1.0.8 (test package): Installing package
Requirements
    bzip2/1.0.8 from local cache - Cache
Packages
    bzip2/1.0.8:5be2b7a2110ec8acdbf9a1cea9de5d60747edb34 - Build

Installing (downloading, building) binaries...
bzip2/1.0.8: Configuring sources in D:\ConanData\bzip2\1.0.8\_\_\source
...
```