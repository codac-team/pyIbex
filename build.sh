#!/bin/bash

set -x

cd ${0%%$(basename $0)}
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=${HOME} ../ && make && make install && make test
