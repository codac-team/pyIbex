#!/bin/bash
version=$(python -V 2>&1 | grep -Po '(?<=Python )(.+)')
echo "find python version ${version}"

if [ ! -e "$HOME/Downloads/boost_1_58_0.tar.gz" ]; then
  echo "Download boost"
  mkdir -p ${HOME}/Downloads
  wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz -O ${HOME}/Downloads/boost_1_58_0.tar.gz
  
else
  echo 'Using cached boost_1_58_0.tar.gz';
fi
tar -xf ${HOME}/Downloads/boost_1_58_0.tar.gz -C ${HOME}

echo "Build Boost"
cd ${HOME}/boost_1_58_0
bash ./bootstrap.sh python=${version:0:3}
if [ $TRAVIS_OS_NAME == linux ]; then
  python_include_dir="/opt/python/${version}/include/python${version:0:3}m"
  echo ${python_include_dir}
  ./bjam --with-python variant=release link=shared include=${python_include_dir}
else
  ./bjam --with-python variant=release link=shared
fi




