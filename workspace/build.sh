#!/bin/bash
# rm "CMakeCache.txt" "cmake_install.cmake" "Makefile"
# rm -r "CMakeFiles"
# cmake . && make
g++-7 -g src/main.cpp -o main -DDBG -D_DEBUG -Wall -Wextra