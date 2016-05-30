# A C+11 toolchain for Mac OS X
set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11" CACHE STRING "C++ compiler settings.")
