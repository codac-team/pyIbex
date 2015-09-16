#example 1, simple sivia

from pyIbex import *
from vibes import *

f = Function('x', 'y', 'x*cos(x-y)+y')

sep = SepFwdBwd(f, CmpOp.LEQ)

box = IntervalVector(2, [-10, 10])

vibes.beginDrawing()

pySIVIA(box, sep, 0.3)

vibes.endDrawing()