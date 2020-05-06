### Rename multiple files using find

Renaming **CMakeLists.txt.docker** to **CMakeLists.txt**
```bash
find . -type f -name 'CMakeLists.txt.docker' -exec bash -c 'mv $0 ${0/.docker/}' {} \;
```