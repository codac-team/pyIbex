#/bin/sh

py_version=python_version=$(python -c "import sys; print(sys.version[:3])") # default python version
prefix=""  # Default install prefix

# extract options and their arguments into variables.
for var in "$@"; do
    case "$var" in
        -h)
            echo "Usage:"
            echo "    $0 [options]"
            echo "Options: "
            echo "    -h                           Display this help message."
            echo "    -prefix=<install_prefix>     Install prefix for libs"
            echo "    -python=<python_version>     Python version to use"
            exit 0;;
        --prefix=*)
            prefix="-DCMAKE_INSTALL_PREFIX=${var##*=}" ;;# Remove 'install' from the argument list
        --python=*)
            py_version=${var##*=}; ;;# Remove 'install' from the argument list
        *) echo "Internal error!" ; exit 1 ;;
    esac
done


# set -x
echo "Installation of pyIbex and its dependancies"
echo "Configuration :"

if [ -n "$prefix" ]; then
  echo "    - custom install prefix = ${prefix##*=}"
fi
echo "    - python version = $py_version"


# save current path
curr_path=$(pwd)
# echo " pybind11 installation"
# git clone https://github.com/wjakob/pybind11.git pybind11
# cd pybind11
# mkdir -p build && cd build
# cmake ${prefix} -DPYBIND11_PYTHON_VERSION=${py_version} ../
# make install

cd $curr_path
echo " ibex_lib installation"
git clone https://github.com/benEnsta/ibex-lib.git
cd ibex-lib
git checkout pyIbex_version_3
mkdir -p build
cd build
cmake ${prefix} -DBUILD_TESTS=OFF  ../
make -j2
make install

cd $curr_path
echo " build pyIbex"
mkdir -p build && cd build
cmake $prefix -DPYTHON_VERSION=${py_version} ../
make && make install && make test




  