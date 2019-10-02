cmake_minimum_required(VERSION 3.15)
project(miki CXX)

#set (CMAKE_CXX_COMPILER /usr/local/bin/clang++)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Boost
# Boost warning hack (macOS only)
set (Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.71.0 REQUIRED COMPONENTS system thread filesystem)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Cannot find Boost libraries")
endif()

# OpenSSL
set (OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/1.0.2t)
find_package(OpenSSL)
if (NOT OPENSSL_FOUND)
    message(FATAL_ERROR "Cannot find OpenSSL libraries")
endif()

include_directories(SYSTEM /usr/local/Cellar/llvm/8.0.1/include/c++/v1)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${OPENSSL_INCLUDE_DIRS})
include_directories(SYSTEM /usr/local/Cellar/openssl/1.0.2t/include)
include_directories(common)
include_directories(.)

# cmark-gfm (macOS)
include_directories(/Users/arturstamanis/Sources/markdown/cmark-gfm/src)
include_directories(/Users/arturstamanis/Sources/markdown/cmark-gfm/build/src)
link_directories(/Users/arturstamanis/Sources/markdown/cmark-gfm/build/src)

file(GLOB SOURCES 
    common/*.cpp
    src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS "-Xlinker -rpath -Xlinker .")
target_link_libraries(
    ${PROJECT_NAME} 
    pthread 
    ${Boost_LIBRARIES} 
    ${OPENSSL_SSL_LIBRARY} 
    ${OPENSSL_CRYPTO_LIBRARY}
    cmark-gfm)

add_custom_target(run 
    COMMAND ../bin/miki --doc-root ../http/html
    DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/miki
    VERBATIM)
