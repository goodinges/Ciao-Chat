#!/bin/sh 
echo "preparing makefiles for Cpp build:"
mkdir cmake-build
cd cmake-build
# generating unix makefiles by default
# other generators avilable using the -G option:
# see `cmake --help` for details
cmake ..
echo "building Cpp libraries and client:"
make install
cd ..
echo "Cpp libraries and test client built in the build directory:"
ls -al build