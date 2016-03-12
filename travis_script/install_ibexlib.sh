# install_ibexlib.sh
set -x

installDir=""

if [ -n "$1" ]; then
  installDir="-DCMAKE_INSTALL_PREFIX=$1"
fi

echo "Ibex will be installed in ${installDir}"
git clone https://github.com/benEnsta/ibex-lib.git
cd ibex-lib
git checkout pyIbex_version_3
mkdir build
cd build
cmake -DBUILD_TESTS=OFF ${installDir} ../
make -j2
make install