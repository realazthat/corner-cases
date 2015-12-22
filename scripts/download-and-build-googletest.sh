#!/bin/bash

#####################################################################
####Run this from the project root directory
####This script assumes that these libraries are already there.
#####################################################################


#exit on failed line
set -exv

PROJECT_PATH=$PWD






#############################################################################
## get/build googletest
#############################################################################

#CMAKE_GENERATOR_FLAG='-GMSYS Makefiles'
#CMAKE_GTEST_FLAG='-Dgtest_disable_pthreads=1'
 

cd $PROJECT_PATH
cd libs

mkdir -p googletest && cd googletest

rm -rf ./googletest/
git clone https://github.com/google/googletest.git
cd googletest
git checkout "ddb8012"
cd googletest

mkdir -p build
cd build
cmake "$CMAKE_GENERATOR_FLAG" .. "$CMAKE_GTEST_FLAG"
cmake --build .

