#!/bin/bash

set -x

cd ${0%%$(basename $0)}
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=./ -DCMAKE_BUILD_TYPE=DEBUG .. && make && make install

