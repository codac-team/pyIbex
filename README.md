==========================
Ibex python binding
==========================

--------------------------
Introduction
--------------------------
This is a python binding, based on boost python library interface for 
ibex library.

Only the Interval and Interval Vector class are supported.

--------------------------
Prerequisites
--------------------------

+ [CMake](http://www.cmake.org "CMake project page") (>= 2.8.3)
+ [Boost](http://www.boost.org/ "Boost project page") (tested with 1.4.2, but should work with >= 1.3.2)
+ [Python](http://www.python.org "Python home page") (tested with 2.7, but should work with >= 2.2)
+ a C++ compiler for your platform, e.g. [GCC](http://gcc.gnu.org "GCC home") or [MinGW](http://www.mingw.org "Minimalist GNU for Windows")

--------------------------
Building
--------------------------
+ Set the `BOOST_ROOT` environment variable if Boost is installed in a non-standard directory
+ create a build directory, e.g. directly in the project directory and cd to it: `mkdir build ; cd build`
+ run `cmake ..` and afterwards `make`, `make install` and `make test`
option : 
	- use -DCMAKE_INSTALL_PREFIX= to change the install destination
	- use -DCMAKE_BUILD_TYPE=DEBUG | RELEASE to change the compilation mode

Alternatively, run the provided `build.sh` script.

--------------------------
TODO
--------------------------
- Add numpy array support for interval Vector class

Function
	- add Matric evaluation support
	- add Jacobian and gradient
Ctc
	