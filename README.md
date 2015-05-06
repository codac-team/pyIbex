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
+ [Boost](http://www.boost.org/ "Boost project page") (tested with 1.4.2, but should work with >= 1.3.2)
+ [Python](http://www.python.org "Python home page") (tested with 2.7, but should work with >= 2.2)
+ a C++ compiler for your platform, e.g. [GCC](http://gcc.gnu.org "GCC home") or [MinGW](http://www.mingw.org "Minimalist GNU for Windows") (not tested on Windows and Mac OS X)

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
- [ ] Add iSet support
- [ ] Add numpy array support for interval Vector class 
- [ ] Add ratio option for LargestFirst(Vector, double )

	

	