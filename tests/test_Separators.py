#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_Separators.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochersss
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================


import unittest
import pyIbex
from pyIbex import *
import sys
import math
class TestSeparator(unittest.TestCase):

	def test_SepCtcPair(self):
		f = Function("x", "y", "x^2 + y^2 - 25")
		ctcIn = CtcFwdBwd(f, CmpOp.GEQ)
		ctcOut = CtcFwdBwd(f, CmpOp.LEQ)
		sep = SepCtcPair(ctcIn, ctcOut)
		b0 = IntervalVector([[-10, -6],  [-10, 10]])
		b1 = IntervalVector([[-2, 2], [-2, 2]])
		
		xin = IntervalVector(b0)
		xout = IntervalVector(b0)
		sep.separate(xin, xout)
		self.assertFalse(xin.is_empty())
		self.assertTrue(xout.is_empty())


		xin = IntervalVector(b1)
		xout = IntervalVector(b1)
		sep.separate(xin, xout)
		self.assertFalse(xout.is_empty())
		self.assertTrue(xin.is_empty())
	
	def test_SepFwdBwd_CmdOp(self):
		f = Function("x", "y", "x^2 + y^2 - 25")
		sep = SepFwdBwd(f, CmpOp.LEQ)
		b0 = IntervalVector([[-10, -6],  [-10, 10]])
		b1 = IntervalVector([[-2, 2], [-2, 2]])
		
		xin = IntervalVector(b0)
		xout = IntervalVector(b0)
		sep.separate(xin, xout)
		self.assertFalse(xin.is_empty())
		self.assertTrue(xout.is_empty())


		xin = IntervalVector(b1)
		xout = IntervalVector(b1)
		sep.separate(xin, xout)
		self.assertFalse(xout.is_empty())
		self.assertTrue(xin.is_empty())
		

	def test_SepFwdBwd_Interval(self):
		f = Function("x", "y", "x^2 + y^2")
		sep = SepFwdBwd(f, Interval(0, 16))
		b0 = IntervalVector([[-10, -6],  [-10, 10]])
		b1 = IntervalVector([[1, 2], [1, 2]])
		
		xin = IntervalVector(b0)
		xout = IntervalVector(b0)
		sep.separate(xin, xout)
		self.assertFalse(xin.is_empty())
		self.assertTrue(xout.is_empty())


		xin = IntervalVector(b1)
		xout = IntervalVector(b1)
		sep.separate(xin, xout)
		self.assertFalse(xout.is_empty())
		# print(xin)
		self.assertTrue(xin.is_empty())
		
	def test_SepFwdBwd_IntervalVector(self):
		f = Function("x", "y", "(x^2 + y^2, x^2 + y^2)")
		sep = SepFwdBwd(f, IntervalVector(2, Interval(0, 16)))
		b0 = IntervalVector([[-10, -6],  [-10, 10]])
		b1 = IntervalVector([[1, 2], [1, 2]])
		
		xin = IntervalVector(b0)
		xout = IntervalVector(b0)
		sep.separate(xin, xout)
		self.assertFalse(xin.is_empty())
		self.assertTrue(xout.is_empty())


		xin = IntervalVector(b1)
		xout = IntervalVector(b1)
		sep.separate(xin, xout)
		self.assertFalse(xout.is_empty())
		# print(xin)
		self.assertTrue(xin.is_empty())


	def test_SepNot(self):

		f = Function("x", "y", "x^2 + y^2 - 25")
		sep = SepFwdBwd(f, CmpOp.LEQ)
		sepN = SepNot(sep)
		del sep # [optionnal] test for memory manadgment
		b0 = IntervalVector([[-10, -6],  [-10, 10]])
		b1 = IntervalVector([[-2, 2], [-2, 2]])
		
		xin = IntervalVector(b0)
		xout = IntervalVector(b0)
		sepN.separate(xin, xout)
		self.assertTrue(xin.is_empty())
		self.assertFalse(xout.is_empty())


		xin = IntervalVector(b1)
		xout = IntervalVector(b1)
		sepN.separate(xin, xout)
		self.assertTrue(xout.is_empty())
		self.assertFalse(xin.is_empty())

		pass

	def test_SepUnionInter(self):
		f1 = Function("x", "y", "x^2 + y^2 - 25")
		f2 = Function("x", "y", "(x-2)^2 + (y+2)^2 - 25")
		f3 = Function("x", "y", "(x+2)^2 + (y-2)^2 - 25")

		sep1 = SepFwdBwd(f1, CmpOp.LEQ)
		sep2 = SepFwdBwd(f2, Interval(4, 16))
		sep3 = SepFwdBwd(f3, CmpOp.LEQ)

		sepU = SepUnion([sep1, sep2, sep3])
		sepI = SepInter([sep1, sep2, sep3])

		# [optionnal] test for memory manadgment
		del f1, f2, f3, sep1, sep2, sep3 

		b00 = IntervalVector([[-10, -6],  [-10, 10]])
		b01 = IntervalVector(b00)

		b10 = IntervalVector([[-10, -6],  [-10, 10]])
		b11 = IntervalVector(b10)

		sepU.separate(b00, b01)
		sepI.separate(b10, b11)


	# def test_SepProj(self):
	# 	f = Function('x', 'y', 'x^2 + y^2 - 4')
	# 	sep = SepFwdBwd(f,CmpOp.LEQ)
	# 	y_init = IntervalVector(1, Interval(1/math.sqrt(2), 0.9))
	# 	sep2 = SepProj(sep, y_init, 0.1, 10)
	# 	# [optionnal] test for memory manadgment
	# 	del sep, f
	# 	Xin = IntervalVector([[-10,0]])
	# 	Xout = IntervalVector([[-10,0]])
	# 	sep2.separate(Xin, Xout)
	# 	print(Xin, Xout)
		

	def test_UnionInter_with_array(self):
		cx = [3, 7,-3]
		cy = [4, 3, 7]
		d  = IntervalVector([3, 6, 6]).inflate(0.5)
		seps = []
		for i,(x,y) in enumerate(zip(cx,cy)):
			f = Function('x', 'y', '(x - %f)^2 + (y - %f)^2'%(x,y))
			seps.append(SepFwdBwd(f, sqr(d[i])))

		sepUnion = SepUnion(seps)
		sepInter = SepInter(seps)
		del seps

		
		Xin = IntervalVector(2, Interval(-10,10))
		Xout = IntervalVector(2, Interval(-10,10))
		sepUnion.separate(Xin, Xout)
		# print(Xin, Xout)

		Xin = IntervalVector(2, Interval(-10,10))
		Xout = IntervalVector(2, Interval(-10,10))
		sepInter.separate(Xin, Xout)

		# print(Xin, Xout)




	# def test_Function_vector(self):
	# 	f = Function("x[2]", "y[3]", "(x[1]^2 - y[1]*y[0], y[0])")
	# 	del f

	# def test_function_matrix(self):
	# 	f = Function("x[2]", "y[3]", "(( x[1]^2 - y[1]*y[0], y[0]);(x[0] + y[2], x[0]))")
 	
 # 	def test_CtcFwdBwd_default_arg(self):
 # 		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 # 		ctc1 = CtcFwdBwd(f)
 # 		ctc1 = CtcFwdBwd(f, CmpOp.LEQ)
 		

 # 	def test_CtcUnion_2_arguments(self):
 # 		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 # 		ctc1 = CtcFwdBwd(f, CmpOp.EQ, FwdMode.AFFINE2_MODE)
	# 	f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
 # 		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
 # 		ctc = CtcUnion([ctc1, ctc2])
 # 		a = IntervalVector(2)
 # 		ctc.contract(a)
 # 		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))

 # 	def test_CtcUnion_Array(self):
 # 		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 # 		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
	# 	f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
 # 		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
 # 		ctc = ctc1 | ctc2
 # 		a = IntervalVector(2)
 # 		ctc.contract(a)
 # 		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))

 # 	def test_CtcCompo(self):
	# 	f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 # 		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
	# 	f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
 # 		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
 # 		ctc = ctc1 & ctc2
 # 		a = IntervalVector(2)
 # 		ctc.contract(a)
 # 		self.assertEqual(a, IntervalVector(2, Interval(-1.1000000000000005, 2.1000000000000005)))

 # 	def test_CtcArray(self):
 # 		f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
 # 		ctc1 = CtcFwdBwd(f, CmpOp.EQ)
	# 	f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
 # 		ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
 # 		ctc = CtcUnion( [ctc1, ctc2, ctc2, ctc1,ctc1, ctc2, ctc2, ctc1 ])
 # 		a = IntervalVector(2)
 # 		ctc.contract(a)
 # 		self.assertEqual(a, IntervalVector(2, Interval(-2.1000000000000005, 3.1000000000000005)))


if __name__ == '__main__':
	
	unittest.main()

