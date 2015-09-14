#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_Bsc.cpp
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================
import unittest
import pyIbex
from pyIbex import *
import sys
import math
class TestContractors(unittest.TestCase):
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
		del f, ctc1, f2, ctc2 # delete tmp object
		a = IntervalVector(2)
		ctc.contract(a)
		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))

	def test_CtcUnion_Array(self):
		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
		f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
		ctc = ctc1 | ctc2 | ctc1
		del ctc2, ctc1 , f, f2 # Test if references are kept by python
		a = IntervalVector(2)
		ctc.contract(a)
		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))

	def test_CtcCompo(self):
		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
		f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
		f3 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
		ctc3 = CtcFwdBwd(f3, CmpOp.EQ)
		ctc = ctc1 & ctc2 & ctc3 
		del ctc1, ctc2, ctc3 # optionnal test if references are kept by python
		a = IntervalVector(2)
		ctc.contract(a)
		self.assertEqual(a, IntervalVector(2, Interval(-1.1000000000000005, 2.1000000000000005)))
	
	def test_CtcArray(self):
		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
		f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
		f , f2 = None, None
		lst = [ctc1, ctc2, ctc2, ctc1,ctc1, ctc2, ctc2, ctc1]
		ctc = CtcUnion( lst)
		del lst, ctc1, ctc2
		a = IntervalVector(2)
		ctc.contract(a)
		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))
	
	def test_CtcPolar(self):
		ctc = CtcPolar()
		x, y, rho, theta = Interval(3,4), Interval(3,6), Interval(2,6), Interval(0, math.pi/4.0)
		ctc.contract(x,y,rho, theta)
		# [3, 4], [3, 4], [4.24264, 5.65685], [0.643501, 0.785398]

	def test_CtcQInter_withRefereces(self):
		data = [Interval(3.61, 4.41), Interval(3.61, 4.41)]
		cx = [0, 1]
		cy = [0, 1]
		ctcs = []
		
		for x, y, r  in zip(cx, cy, data):
			f = Function('x', 'y', '(x-%f)^2 + (y-%f)^2 - %s'%(x,y,r))
			ctcs.append(CtcFwdBwd(f, CmpOp.LEQ))
		
		ctc = CtcQInterProjF(ctcs, len(ctcs)-1) # Union
		ctcs = None
		a = IntervalVector(2, Interval(-20,20))
		ctc.contract(a)
		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))
		
		


if __name__ == '__main__':
	
	unittest.main()

