cmake_minimum_required(VERSION 3.15)

project(miki CXX)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


# Boost
find_package(Boost 1.70.0 REQUIRED COMPONENTS system thread filesystem)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Cannot find Boost libraries")
endif()

# OpenSSL
set (OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/1.0.2s)
find_package(OpenSSL)
if (NOT OPENSSL_FOUND)
    message(FATAL_ERROR "Cannot find OpenSSL libraries")
endif()

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${OPENSSL_INCLUDE_DIRS})
include_directories(SYSTEM /usr/local/Cellar/openssl/1.0.2s/include)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} pthread ${Boost_LIBRARIES} ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})

