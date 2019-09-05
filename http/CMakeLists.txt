cmake_minimum_required(VERSION 3.15)
project (miki)

set (CMAKE_CXX_COMPILER /Users/arturstamanis/Sources/emsdk/fastcomp/emscripten/em++)
add_definitions(-std=c++17 -O3)

file (GLOB SOURCES src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/html")

#if(JS_ONLY)
#message(STATUS "Setting compilation target to native JavaScript")
#set(CMAKE_EXECUTABLE_SUFFIX ".js")
#set_target_properties(a PROPERTIES LINK_FLAGS "-s EXPORTED_FUNCTIONS='[_main]'")
#else(JS_ONLY)
#message(STATUS "Setting compilation target to WASM")
#set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
#set_target_properties(a PROPERTIES LINK_FLAGS "-s WASM=1 -s BINARYEN_METHOD='native-wasm' -s EXPORTED_FUNCTIONS='[_main]'")
#endif(JS_ONLY)

add_executable(${PROJECT_NAME} ${SOURCES})
set(CMAKE_EXECUTABLE_SUFFIX ".html")

set(MIKI_LINK_FLAGS "-s FETCH=1 -s NO_EXIT_RUNTIME=1  -s \"EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']\"")
set(MIKI_TEMPLATE "--shell-file ${CMAKE_SOURCE_DIR}/templates/miki_main.html")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${MIKI_LINK_FLAGS} ${MIKI_TEMPLATE}")

#em++ -std=c++17 -Wall -O3 -s FETCH=1 -s NO_EXIT_RUNTIME=1  -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']"  --shell-file templates/miki_main.html  -o html/miki.html src/main.cpp
