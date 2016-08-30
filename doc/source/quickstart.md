# Quickstart

This sections demonstrates the basic features of pyIbex. Before getting
started, pyIbex and its dependencies must be correctly installed.


PyIbex functionalities and object are under the pyibex namespace.

```python
    from pyibex import Interval
```
## Interval and IntervalVector

The type Interval represents a closed set of \$\mathbb{R}^2\$


```python

    # Load pyibex lib
    from pyibex import Interval

    # Create new Intervals
    a = Interval.EMPTY_SET # create an empty interval
    a = Interval.ALL_REALS # create [-oo, oo]
    a = Interval()         # create [-oo, oo]
    a  = Interval(-2, 3)   # create [-2, 3]
```

An IntervalVector or a Box is a cartesion product of intervals. With pyibex it can be created :
 - with the dimension and an initial interval [x_lb, x_ub].
 - with a point passed as a list [x1, x2, ..., xn] or tuple ( x1,x2,...,xn)
 - with a list of list of bounds [[x1_lb, x1_ub], [x2_lb, x2_ub], ..., [xn_lb, xn_ub]]
 - with a list of interval ([x1], [x2], ..., x[n])

```python
    # Create IntervalVector
    from pyibex import IntervalVector
    # Create the box
    IntervalVector( 2, a) # ==> box [1, 3]x[1,3]
    IntervalVector([1,2,3]) # ==> box [1,1]x[2,2]x[3,3]
    IntervalVector([[-1,3], [3,10], [-3, -1]]) # ==> box  [-1,3]x[3,10]x[-3,-1]
    IntervalVector( [a, Interval(-1,0), Interval(0) ] )
```


## Functions and contractors manipulation

```python
    from pyibex import *
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
