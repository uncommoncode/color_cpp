# A C+11 toolchain for Linux
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER gcc-4.8)
set(CMAKE_CXX_COMPILER g++-4.8)

set(CMAKE_CXX_FLAGS "-std=c++11" CACHE STRING "C++ compiler settings.")
