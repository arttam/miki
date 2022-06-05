## Without extra step - all done by make

### With repository and profile
```bash
# ==== Conan ====
# As we usign Ubuntu 20.04 following variables population might be not necessary
# But for furture versions (operating systems ?) - just a reference
find_program(LSB_RELEASE_EXEC lsb_release)

execute_process(COMMAND ${LSB_RELEASE_EXEC} -is
   OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
   OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND ${LSB_RELEASE_EXEC} -rs
   OUTPUT_VARIABLE LSB_RELEASE_VERSION
   OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "conan install ${CMAKE_SOURCE_DIR}/Conan/conanfile.txt -pr ${CMAKE_SOURCE_DIR}/Conan/${LSB_RELEASE_ID_SHORT}-${LSB_RELEASE_VERSION}_Release.conan.profile -g cmake -r mirriad-artifactory")
execute_process(
   COMMAND conan install ${CMAKE_SOURCE_DIR}/Conan/conanfile.txt -pr ${CMAKE_SOURCE_DIR}/Conan/${LSB_RELEASE_ID_SHORT}-${LSB_RELEASE_VERSION}_Release.conan.profile -g cmake -r mirriad-artifactory
   OUTPUT_FILE conan_install_output.txt
   ERROR_FILE conan_install_error.txt)

# Conan Paths
if(EXISTS "${CMAKE_BINARY_DIR}/conan_paths.cmake")
   include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup(TARGETS)
# ==== Conan ====
```

Followed by explicit linking instructions
```bash
target_link_libraries(${PROJECT_NAME} 
   CONAN_PKG::log4cplus 
   CONAN_PKG::rapidjson
...
```

### Minimal set centered only on working host 
```bash
cmake_minimum_required(VERSION 3.16)
project(try-sqlite)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==== Conan ====
message(STATUS "conan install ${CMAKE_SOURCE_DIR}/conanfile.txt -g cmake ")
execute_process(
   COMMAND conan install ${CMAKE_SOURCE_DIR}/conanfile.txt -g cmake
   OUTPUT_FILE conan_install_output.txt
   ERROR_FILE conan_install_error.txt)

# Conan Paths
if(EXISTS "${CMAKE_BINARY_DIR}/conan_paths.cmake")
   include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup(TARGETS)
# ==== Conan ====

include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable (${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::sqlite3)
```
