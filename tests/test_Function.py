#!/usr/bin/env python
import unittest
import pyIbex
from pyIbex import *
import sys
class SimplisticTest(unittest.TestCase):
	def test_Function(self):
		# generate memory leak !!!
		try:
			f = Function("x", "y", "sqr(x) + sqr(y)")
		except:
			print("Unexpected error:", sys.exc_info()[0])
		# # except SyntaxError:
		# 	print("Get syntaxError exeption")
		
	def test_Function_vector(self):
		f = Function("x[2]", "y[3]", "(x[1]^2 - y[1]*y[0], y[0])")
		del f

	def test_function_matrix(self):
		f = Function("x[2]", "y[3]", "(( x[1]^2 - y[1]*y[0], y[0]);(x[0] + y[2], x[0]))")
 	
 	def test_CtcFwdBwd_default_arg(self):
 		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 		ctc1 = CtcFwdBwd(f)
 		ctc1 = CtcFwdBwd(f, CmpOp.LEQ)
 		

 	def test_CtcUnion_2_arguments(self):
 		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 		ctc1 = CtcFwdBwd(f, CmpOp.EQ, FwdMode.AFFINE2_MODE)
		f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
 		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
 		ctc = CtcUnion([ctc1, ctc2])
 		a = IntervalVector(2)
 		ctc.contract(a)
 		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))

 	def test_CtcUnion_Array(self):
 		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
		f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
 		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
 		ctc = ctc1 | ctc2
 		a = IntervalVector(2)
 		ctc.contract(a)
 		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))

 	def test_CtcCompo(self):
		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
		f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
 		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
 		ctc = ctc1 & ctc2
 		a = IntervalVector(2)
 		ctc.contract(a)
 		self.assertEqual(a, IntervalVector(2, Interval(-1.1000000000000005, 2.1000000000000005)))

 	def test_CtcArray(self):
 		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
		f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
 		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
 		ctc = CtcUnion( [ctc1, ctc2, ctc2, ctc1,ctc1, ctc2, ctc2, ctc1 ])
 		a = IntervalVector(2)
 		ctc.contract(a)
 		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))


if __name__ == '__main__':
	
	unittest.main()

