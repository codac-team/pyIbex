#example 1, simple sivia

from pyIbex import *
from vibes import *

class myCtc(Ctc):
	def __init__(self, x0, y0, R):
		Ctc.__init__(self, 2)
		self.x0 = Interval(x0)
		self.y0 = Interval(y0)
		self.R = R

	def contract(self, X):
		a1 = X[0] - Interval(self.x0)
		a2 = X[1] - Interval(self.y0)
		
		a3 = sqr(a1)
		a4 = sqr(a2)
		
		a5 = a3 + a4
		a5 &= sqr(self.R)

		bwd_add(a5, a3, a4)
		bwd_sqr(a4, a2)
		bwd_sqr(a3, a1)

		bwd_sub(a1, X[0], (self.x0))
		bwd_sub(a2, X[1], (self.y0))


ctc = myCtc(1, 2, Interval(1, 2))
box = IntervalVector(2, [-10, 10])

vibes.beginDrawing()

pySIVIA(box, ctc, 0.3)

vibes.endDrawing()