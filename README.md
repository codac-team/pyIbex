[![Build Status](https://travis-ci.org/benEnsta/pyIbex.svg?branch=master)](https://travis-ci.org/benEnsta/pyIbex)

==========================
Ibex python binding
==========================

--------------------------
Introduction
--------------------------
pyIbex is a python binding of Ibex library. It aims at providing a basic interface
of Ibex Basic types (Interval, IntervalVector, Ctc, ...), high level functionnalities and 
contractors programming.

--------------------------
Prerequisites
--------------------------

+ [CMake](http://www.cmake.org "CMake project page") (>= 2.8.3)
+ [Boost](http://www.boost.org/ "Boost project page") (tested with 1.58.0)
+ [Python](http://www.python.org "Python home page") (tested with 3.4, but should work with >= 2.7)
+ a C++ compiler for your platform, e.g. [GCC](http://gcc.gnu.org "GCC home"), [MSVS 13](https://www.visualstudio.com "Visual Studio 2013"), [llvm](http://llvm.org "llvm")



--------------------------
Features
--------------------------
Binding for :
- Interval, IntervalVector
- Bsc, LargestFirst
- Function (only text constructors)
- Ctc, CtcFwdBwd, CtcIn, CtcNotIn
- CtcUnion, CtcCompo
- CtcInverse
- CtcPolar
- CtcSegment (need ibex modification !!!)
- CtcQInterProjF
- SepUnion, SepInter
- SepCtcPair , SepFwdBwd, SepNot (negation of a separator)
- SepInverse, SepTransform
- SepPolygon, SepPolarXY, SepPolarXYT
- SepQInterProjF, SepCtcPairProj, SepProj



-----------------
Build From Source
-----------------

###Building
--------------------------------------
+ Set the `BOOST_ROOT` environment variable if Boost is installed in a non-standard directory
+ create a build directory, e.g. directly in the project directory and cd to it: `mkdir build ; cd build`
+ run `cmake ..` and afterwards `make`, `make install` and `make test`
option : 
	- use -DCMAKE_INSTALL_PREFIX= to change the install destination
	- use -DCMAKE_BUILD_TYPE=DEBUG | RELEASE to change the compilation mode
	- use -DCPPTEST_DIR= if libcpptest is installed in a non-standard directory
	- use -DIBEX_ROOT= if ibex is installed in a non-standard directory
Alternatively, run the provided `build.sh` script.

### Linux/Mac OS X
--------------------------------------
We assume that we want to install files in $DEVEL_BASE directory.

+ Firstly python dev packages must be installed.
+ Boost Python needs to be compiled with the correct python version. 
	If it is not the case, download the sources from [Boost] webpage.
+ Build boost from source [boost_1_58_0](http://www.boost.org/users/history/version_1_58_0.html "boost_1_58_0 download page")
```bash
cd boost_1_58_0
sh ./bootstrap.sh --with-python=3.4
./b2 --with-python variant=release link=static
```
+ Build Ibex (Use version from benEnsta repository)
```bash
git clone https://github.com/benEnsta/ibex-lib.git
cd ibex-lib
git checkout pyIbex_version
mkdir build
cd build
cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release ../
make 
make install
```

+ Build pyIbex
```bash
git clone https://github.com/benEnsta/pyIbex.git
cd pyIbex
mkdir build 
cd build
cmake -DBOOST_ROOT=${path_to_boost_1_58_0} ../
make && make install
```
		
###For Windows Users (Win64 version)
--------------------------------------
+ build boost from source
  Create in boost directory a file named "user-config.jam" which looks like:
```text
using python
	: 3.4 												# python version 
	: E:\pyzo2015a\python.exe 		# python executable path
	: E:\pyzo2015a\include				# python include dir
	: E:\pyzo2015a\libs 					# python lib dir
```
++ Compile boost with visual studio with correct options :
```bash
./bjam --with-python variant=release link=static address-model=64 runtime-link=static
```
+ Build Ibex
  Open the developer command prompt and compile ibex-lib
```bash
git clone https://github.com/benEnsta/ibex-lib.git
cd ibex-lib
git checkout pyIbex_version
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF ../
msbuild /P:Configuration=Release /p:RuntimeLibrary=MT_StaticRelease PACKAGE.vcxproj
```
The last command generates a installer for ibex-lib

+ build pyIbex with pyzo2015a 
```bash 
cd pyIbex
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DBOOST_ROOT=$$$$$$$$ -DIBEX_ROOT=$$$$$$$$ -DPYTHON_EXECUTABLE=$$$$$$$$ ../
msbuild /P:Configuration=Release /p:RuntimeLibrary=MT_StaticRelease INSTALL.vcxproj
```




--------------------------
Tutorial
--------------------------
###Basic types manipulation

```python
# Load pyIbex lib
from pyIbex import *

# Create new Intervals
a = Interval.EMPTY_SET
a = Interval.ALL_REALS
a  = Interval(-2, 3)

# Create IntervalVector
b = IntervalVector( 2, a)
c = IntervalVector([1,2,3])
d = IntervalVector([[-1,3], [3,10], [-3, -1]])

# Operations
e = c & d
e = c+d
e = a * c
```

###Functions and contractors manipulation
```python
# Define a Function from an equation
f = Function("x", "y", "x^2 + y^2 - 3")

# FwdBwd Contractor
ctc1 = CtcFwdBwd(f, CmpOp.LEQ, FwdMode.AFFINE_MODE)

# CtcIn/CtcOut contractors :math:`$f \in [-2.5, 3.5]$`
ctcIn = CtcIn(f, Interval(3).inflate(0.5))
ctcOut = CtcNotIn(f, Interval(3).inflate(0.5))

# Operations on Contractors 
# composition of two contractor
ctc = ctcIn & ctcOut 
# union of a python list of contractors
ctc1 = CtcUnion([ctcIn, ctcOut, ctc1]) 

```

--------------------------
TODO
--------------------------
- [ ] Write the documentation !!!
- [ ] Add IntervalMatrix support
	- [ ] Add IntervalMatrix support
	- [ ] Add Jacobian and gradient
- [ ] Add numpy array support for interval Vector class 
- [ ] Add ratio option for LargestFirst(Vector, double )
- [X] Simplify Wrapper interface
- [X] Add n_var in separator
- [ ] fix exceptions
- [X] Improve ItervalVectorinterface
	- [X] Initialisation with X = IntervalVector(2, [2,3]) 
	- [ ] Iterate over dimension of an IntervalVector
- [ ] add Function / Ctc / Sep Properties ( nb_vars, ...)
- [X] inheritance with python objects
	
--------------------------
Change Log
--------------------------
04/10/2015 : Add predicate, Paving
20/09/2015 : Add Set interface, add contract method with set for Sep


	
