from .pyibex import *
from .pySIVIA import pySIVIA

pyibex.SepInter.__doc__ = "fksdlkcsdcjkjsdcjdsj"

pyibex.Bsc.__doc__="""Generic bisector
    Base class of all bisectors. A bisector is an operator used by a strategy
    (like the \link ibex::Solver solver \endlink) to make a choice point.
    An implementation of this class must provide a #bisect(const IntervalVector& box) function.
    Bisectors also handle a precision: when the domain of a variable is less than
    its corresponding precision, the bisector skips this variable.
    When all the variables are skipped, the bisector throws a NoBisectableVariableException.

    This precision allows to control the depth of the search (paver/solver/optimizer).
    The boundary of the set to be described by the search is usually associated to the boxes
    that were not bisectable. So, the precision of the boundary is directly related to the
    \link ibex::Bsc::prec precision \endlink parameter of this class.

    The docs string is taken from ibex_Bsc.h source file
    For more information read doc from http://www.ibex-lib.org/

"""

pyibex.Bsc.bisect.__doc__="""Bisect the current box and return the result.
Args:
    x (IntervalVector): the box to bisect (split)
Return:
    list<IntervalVector> 2 sub boxes
"""

# pyibex.Bsc.default_ratio.__doc__="""Default ratio (0.45)
# """

pyibex.Bsc.uniform_prec.__doc__="""Return:
  bool : true if the precision is the same for all variables.
"""

pyibex.Bsc.prec.__doc__ ="""Return:
    float : Precision for the ith variable
Args:
    i (int) : index of the variable
"""

pyibex.Bsc.too_small.__doc__ ="""Return:
    True iff box[i] is greater than the required precision and bisectable.
"""


pyibex.LargestFirst.__doc__="""Create a bisector with largest first heuristic.
  Can be created with uniform or variable precision for variable.

Args:
  prec (float or list<float>) : precision
  ratio (float) : the ratio between the diameters of the left and the right parts of the
                  bisected interval. Default value is 0.45.
Raises:
  The precision vector gives the threshold precision for each variable.
  The bisector throws a NoBisectableVariableException if, for every i,
  the diameter of box[i] is less than prec[i].

Examples:
    # Uniform
    bsc = LargestFirst(0.1)
    # variable
    bsc = LargestFirst([0.1, 1, 3])
"""
