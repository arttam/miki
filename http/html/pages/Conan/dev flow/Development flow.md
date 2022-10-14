# Development flow of conan package

[Source](https://docs.conan.io/en/latest/developing_packages/package_dev_flow.html)

## Trial and error in a local sub-directories
All steps done relative to folder where conanfile.py is located

### Step 1 - Source
```bash
conan source . --source-folder=tmp/source
```
Once you’ve got your source method right and it contains the files you expect, you can move on to testing the various attributes and methods related to downloading dependencies.  

### Step 2 - Install
Conan has multiple methods and attributes which relate to dependencies (all the ones with the word “require” in the name). The command conan install activates all them.
```bash
conan install . --install-folder=tmp/build [--profile XXXX]
```
This also generates the conaninfo.txt and conanbuildinfo.xyz files (extensions depends on the generator you’ve used) in the temp folder (install-folder)  
Once you’ve got this command working with no errors, you can move on to testing the build() method.  

### Step 3 - Build
The build method takes a path to a folder that has sources and also to the install folder to get the information of the settings and dependencies. It uses a path to a folder where it will perform the build. In this case, as we are including the conanbuildinfo.cmake file, we will use the folder from the install step.  
```bash
conan build . --source-folder=tmp/source --build-folder=tmp/build
```

### Step 4 - Package
This is pretty straightforward, but it does add a very helpful new shortcut for people who are packaging their own library.   
Now, developers can make changes in their normal source directory and just pass that path as the --source-folder.   
Just as it sounds, this command now simply runs the package() method of a recipe.   
It needs all the information of the other folders in order to collect the needed information for the package: header files from source folder, settings and dependency information from the install folder and built artifacts from the build folder.  

```bash
conan package . --source-folder=tmp/source --build-folder=tmp/build --package-folder=tmp/package
```
When you have checked that the package is done correctly, you can generate the package in the local cache. Note that the package is generated again to make sure this step is always reproducible.

### Step 5 - Export package
Using source-folder and build-folder will use the package() method to extract the artifacts from those folders and create the package, directly in the Conan local cache.   
Strictly speaking, it doesn’t require executing a `conan package` before, as it packages directly from these source and build folders, though `conan package` is still recommended in the dev-flow to debug the package() method.  
```bash
conan export-pkg . user/channel --source-folder=tmp/source --build-folder=tmp/build --profile=myprofile
```
In my case:
```bash
 conan export-pkg . art/test --source-folder=tmp/source --build-folder=tmp/build --profile=../../conan-win/conan.profile
```

### Step 6 - Test package
```bash
conan test test_package hello/1.1@user/channel
```

## Dev steps summary 
```bash
$ git clone https://github.com/conan-io/examples.git
$ cd features/package_development_flow
$ conan source .
$ conan install . -pr=default
$ conan build .
$ conan package .
# So far, this is local. Now put the local binaries in cache
$ conan export-pkg . hello/1.1@user/testing -pr=default
# And test it, to check it is working in the local cache
$ conan test test_package hello/1.1@user/testing
...
hello/1.1@user/testing (test package): Running test()
Hello World Release!
```

### Step 7 - Create package
Now we know we have all the steps of a recipe working.   
Thus, now is an appropriate time to try to run the recipe all the way through, and put it completely in the local cache.

The usual command for this is conan create and it basically performs the previous commands with conan test for the test_package folder:
```bash
$ conan create . user/channel
```

Even with this command, the package creator can iterate over the local cache if something does not work.   
This could be done with `--keep-source` and `--keep-build` flags.  

If you see in the traces that the source() method has been properly executed but the package creation finally failed, you can skip the **source()** method the next time issue conan create using `--keep-source`:
```bash
$ conan create . user/channel --keep-source
```

If you see that the library is also built correctly, you can also skip the build() step with the --keep-build flag:
```bash
$ conan create . user/channel --keep-build
```

## Or condenced version:
```bash
conan source . -sf tmp/src
conan install . -if tmp/build -pr .\conan.profile
conan build . -sf tmp/src -bf tmp/build
conan package . -sf tmp/src -bf tmp/build -pf tmp/package
conan export-pkg . mirriad/test -sf tmp/src -bf tmp/build -pr .\conan.profile
```