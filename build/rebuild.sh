#!/bin/bash
rm -rf thirdparty
rm -rf module
rm -rf CMakeFiles
rm -rf cmake_install.cmake
rm -rf Makefile
rm -rf CMakeCache.txt
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DBUILD_DEBUG=FALSE ..
if [ $? -ne 0 ]; then
 return
fi

make -j8

if [ $? -ne 0 ]; then
 return
fi
cp ../bin/* ../../bin/
