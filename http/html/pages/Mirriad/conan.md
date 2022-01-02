## Conan on Mirriad

### On Ubuntu (works)

Installation from remote
```bash
$ conan install -r mirriad-artifactory-test qt/5.12.3@mirriad/stable -pr ~/Projects/ZoneSense/zonesense/Conan/Ubuntu-20.04_Release.conan.profile
```

Creation of downloaded package 
```bash
$ conan create . mirriad/stable-9.3 -pr ../ZSArt/zonesense/Conan/Ubuntu-20.04_Release.conan.profile
```

Uploading to repository
```bash
$ conan upload cpp_redis/0.1@mirriad/stable --all -r mirriad-artifactory
```
Remotes list
```bash
╭─art@pop-os ~/.ssh
╰─➤  conan remote list
conancenter: https://center.conan.io [Verify SSL: True]
mirriad-artifactory: https://mirriad.jfrog.io/mirriad/api/conan/zonesense-dependencies [Verify SSL: True]
```
Remotes list (previous/dev)
```bash
╭─art@ip-172-16-0-45 ~
╰─➤  conan remote list
mirriad-artifactory-test: https://mirriad.jfrog.io/mirriad/api/conan/zonesense-dependencies-test [Verify SSL: True]
conancenter: https://center.conan.io [Verify SSL: True]
conan-center: https://conan.bintray.com [Verify SSL: True]
mirriad-artifactory: https://mirriad.jfrog.io/mirriad/api/conan/zonesense-dependencies [Verify SSL: True]
```

Refs
```bash
╭─art@pop-os ~/.ssh
╰─➤  host gpu002.dev.mirriad.com
gpu002.dev.mirriad.com is an alias for ec2-3-249-71-242.eu-west-1.compute.amazonaws.com.
ec2-3-249-71-242.eu-west-1.compute.amazonaws.com has address 3.249.71.242
```

### Probablies
Another saga
```bash
$ conan search threadpool -r mirriad-artifactory-test
$ conan search threadpool -r conan-center
$ conan install threadpool/20140926@_/_ -pr ../zonesense/Conan/Ubuntu-20.04_Release.conan.profile
$ conan install threadpool/20140926@_/_ -pr ../zonesense/Conan/Pop_Release.conan.profile
$ conan install threadpool/20140926@_/_ -pr ../zonesense/Conan/Pop_Release.conan.profile --build=threadpool

$ conan remote add conancenter https://center.conan.io
$ conan install harfbuzz/2.8.2@_/_ -pr ./Conan/Ubuntu-20.04_Release.conan.profile -g cmake --build=missing

$ conan remote update conancenter https://center.conan.io --insert
$ conan install /home/art/Projects/ZoneSense/zonesense-c9cxx17/conanfile.py -pr /home/art/Projects/ZoneSense/zonesense-c9cxx17/Conan/Pop_Release.conan.profile -g cmake --build=missing
$ conan remote update mirriad-artifactory-test https://mirriad.jfrog.io/artifactory/api/conan/zonesense-dependencies-test --insert
```

## Gotchas
If repository wise recepi does not work:
```bash
conan install C:/ZoneSense/zonesense/conanfile.py -pr C:/ZoneSense/zonesense/Conan/Windows_vs2017_Release.conan.profile -r mirriad-artifactory-test -g cmake 
```
and another recepies are avaialble it is possible to try to install from another ones:
```bash
conan install C:/ZoneSense/zonesense/conanfile.py -pr C:/ZoneSense/zonesense/Conan/Windows_vs2017_Release.conan.profile -g cmake
```
And then upload missing to target repository 

## Broken package
Under <conan_data> find package export folder, for example
```bash
 /d/ConanData/activemq_cpp/3.9.5.3/mirriad/stable/export
```
Edit conanfile.py, and try to build
