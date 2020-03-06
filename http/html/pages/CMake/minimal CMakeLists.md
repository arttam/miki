### Minimal CMakeLists.txt

```
cmake_minimum_required(VERSION 3.16)
project(task CXX)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

file(GLOB SOURCES src/task1.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

add_custom_target(run
    COMMAND ../bin/${PROJECT_NAME}
    DEPENDS ../bin/${PROJECT_NAME})
```