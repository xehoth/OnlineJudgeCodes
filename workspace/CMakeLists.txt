cmake_minimum_required(VERSION 3.0)
project(oi)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wall -Wextra -Winline -g -DDBG -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

set(SOURCE_FILES src/main.cpp)
add_executable(main ${SOURCE_FILES})
