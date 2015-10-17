#!/bin/bash

set -x

cd ${0%%$(basename $0)}
git clone https://github.com/benEnsta/ibex-lib.git
cd ibex-lib
git checkout pyIbex_version
mkdir build
cd build
cmake ../
make -j2
make test
sudo make install