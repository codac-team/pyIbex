[![Build Status](https://travis-ci.org/benEnsta/pyIbex.svg?branch=master)](https://travis-ci.org/benEnsta/pyIbex)
[![Build status](https://ci.appveyor.com/api/projects/status/ughoj13u2rve6mw0?svg=true)](https://ci.appveyor.com/project/benEnsta/pyibex)
[![Conda_Statux](https://anaconda.org/benensta/pyibex/badges/installer/conda.svg)](https://ci.appveyor.com/project/benEnsta/pyibex)
==========================
Ibex python binding
==========================

--------------------------
Introduction
--------------------------
pyIbex is a python binding of Ibex library. It aims at providing a basic interface
of Ibex types (Interval, IntervalVector, Ctc, ...),  and high level functionalities such as
contractors programming.

Since the version 1.5, pyibex module only contains core functionalities of ibex.

The version 1.2 uses now uses pyBind11 instead of Boost-Python to link C++ and python code.
The core functionalities of pyIbex are also reduced and additionnal features are moved to additionnal module.
As a consequence, the following modules are removed from the core distributes :
 + [geometry](https://github.com/benEnsta/ibex-geometry) (CtcAngle, CtcPolar, SepPolarXY, ...)
 + Thickset ( still under huge developpement)
 + [Tubes](https://github.com/SimonRohou/tube-lib)

 See [pyibex website](http://www.ensta-bretagne.fr/desrochers/pyIbex/) for more informations.

# Install

## With Conda
```bash
conda install -c benensta pyibex
```


## Ubuntu 16.04 / 14.04
```bash
sudo add-apt-repository ppa:ben-ensta/pyibex
sudo apt-get update
sudo apt install python3-pyibex
```

## Mac OSX / Windows
```python
>>> python -m pip install pyibex
```

--------------------------
Dependancies
--------------------------

+ [CMake](http://www.cmake.org "CMake project page") (>= 2.8.3)
+ [Python](http://www.python.org "Python home page") (tested with 3.4, but should work with >= 2.7)
+ [pybind11](https://github.com/wjakob/pybind11) required C++11 support
+ [ibex-lib](http://www.ibex-lib.org/) custom version from [github](https://github.com/benEnsta/ibex-lib/tree/pyIbex_version_3)
+ a C++ compiler for your platform , e.g. [GCC](http://gcc.gnu.org "GCC home") (>= 4.8), [MSVS 13](https://www.visualstudio.com "Visual Studio 2014"), [llvm](http://llvm.org "llvm")


-----------------
Build From Source
-----------------

###Building
--------------------------------------

The build process is entirely based on cmake with the following options:

	- use -DCMAKE_INSTALL_PREFIX= to change the install destination
	- use -DCMAKE_BUILD_TYPE=DEBUG | RELEASE to change the compilation mode
	- use -DIBEX_ROOT= if ibex is installed in a non-standard directory
  - use -DPYTHON_VERSION= to set the target python version.

Sometime, you will have to manualy specify python executable, include and libs dir using:

  -using -DPYTHON_EXECUTABLE to specify python executable path.

Alternatively, run the provided `install.sh` script.
remark : If Ibex isn't installed on the current machine, it will be download, build an installed.


### Linux/Mac OS X
--------------------------------------

To install pyIbex run the following commands:
```bash
git clone https://github.com/benEnsta/pyIbex.git
cd pyIbex
git submodule init
mkdir -p build && cd build
make
# if pip is installed run
make install_python
# otherwise
make install
```
If you used a non stantdart install preifx (-DCMAKE_INSTALL_PREFIX),
add `${PREFIX}/lib/python3/dist-packages` to your PYTHONPATH

###For Windows Users (Win64 version) (Not ready Yet)
--------------------------------------
+ Build Ibex
  Open the developer command prompt and compile ibex-lib
```bash
git clone -b with_cmake https://github.com/benEnsta/ibex-lib.git
cd ibex-lib
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF ../
msbuild /P:Configuration=Release /p:RuntimeLibrary=MT_StaticRelease PACKAGE.vcxproj
```

The last command generates a installer for ibex-lib
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

e = IntervalVector( (a, Interval(-1,0), Interval(0)) )

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
- [X] Write the documentation !!!
- [X] Add IntervalMatrix support
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
Changelog
--------------------------
- 19/0702016 (v1.5.0a0)
  - only keep ibex core fonctionnalities
    - Interval,
    - IntervalVector,
    - Interval
    - Ctc,
    - Sep,
- 06/07/2016 : use ibex-lib 3.2.0
               update Docstring
               update pybind11 interface
- 27/01/2016 : Use pybind11 instead of boost python.
               Remove wip directory
- 12/02/2015 : Add SepPolarXY
- 04/10/2015 : Add predicate, Paving
- 20/09/2015 : Add Set interface, add contract method with set for Sep
