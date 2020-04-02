#!/bin/bash
set -e -x
mkdir -p /io/cache
cd /io/cache
if [ ! -d "/io/cache/cmake-3.13.4" ]; then
  curl -L https://github.com/Kitware/CMake/releases/download/v3.13.4/cmake-3.13.4.tar.gz --output cmake-3.13.4.tar.gz
  ls .
  tar -zxf cmake-3.13.4.tar.gz
  ls .
  pwd
  cd cmake-3.13.4
  ./bootstrap && make && make install
else
  cd /io/cache/cmake-3.13.4
  make install
fi

if [ ! -d "/io/cache/ibex-lib" ]; then
  cd /io/cache
  git clone -b with_cmake https://github.com/benEnsta/ibex-lib.git
  cd ibex-lib
  mkdir build && cd build
  cmake -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${HOME}/ibex ../
  make -j2
  make install
else
  cd /io/cache/ibex-lib/build
  make install
fi

cd /io
for PYBIN in /opt/python/*/bin; do
     mkdir build_dir && cd build_dir
     cmake -DIBEX_ROOT=${HOME}/ibex -DPYTHON_EXECUTABLE=${PYBIN}/python ..
     make -j2 && make test && make pip_package
     echo "copy wheel and clean build_dir"
     for whl in *.whl; do
         auditwheel repair "$whl" -w /io/wheelhouse/
     done
     cd ..
     rm -fr build_dir
done
#

#
# # Install packages and test
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/python" -m pip   install pyibex --no-deps --no-index -f /io/wheelhouse
    (cd "$HOME"; "${PYBIN}/python" -m unittest discover pyibex.tests)
done
