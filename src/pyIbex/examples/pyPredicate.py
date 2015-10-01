from pyIbex import Interval, Pdc, YES, NO, MAYBE

class pyPdcIn(Pdc):
	"""
	pyPdcIn :
		Predicate which check f(X) In itv1
		if itv2 is provided, it can be used to build uncertain set :
			return YES if f(X) In itv1
			return NO if f(X) In itv2
			else return MAYBE
	"""
	def __init__(self, f, itv1, itv2=None):
		Pdc.__init__(self, f.nb_arg())
		self.itv = itv1
		self.itv2 = itv2
		self.f = f

	def In(self, X, itv, itv2):
		if X.is_subset(itv):
			return YES
		elif itv2 != None and X.is_subset(itv2) :
			return NO
		elif itv2 == None and ( X & itv ).is_empty() :
			return NO
		else :
			return MAYBE

	def test(self, X):
		Xf = self.f.eval_vector(X)
		return self.In(Xf, self.itv, self.itv2)

class pyPdcTransform(Pdc):
	"""
	pyPdcTransform : 
		return pdc.test(f(X))
	"""
	def __init__(self, f, pdc):
		Pdc.__init__(self, f.nb_arg())
		self.f = f
		self.pdc = pdc

	def test(self, X):
		Xf = self.f.eval_vector(X)
		return self.pdc.test(Xf)

class pyPdcInSet(Pdc):
	"""
	pyPdcInSet : 
		Check if X In Set
	"""
	def __init__(self, A):
		Pdc.__init__(self, 2)
		self.A = A

	def test(self, X):
		return self.A.contains(X)

