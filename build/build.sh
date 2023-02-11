#!/bin/bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DBUILD_DEBUG=FALSE ..
if [ $? -ne 0 ]; then
 return
fi

make -j8

if [ $? -ne 0 ]; then
 return
fi
cp ../bin/* ../../bin/
