#!/bin/sh

cmake .
make

#mv AutoParts bin/

rm Makefile
rm cmake_install.cmake
rm CMakeCache.txt
rm -rf CMakeFiles



