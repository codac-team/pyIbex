#!/usr/bin/env python3
#============================================================================
#                                P Y I B E X
# File        : test_Set.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Sep 20, 2015
#============================================================================

import unittest
import pyIbex
from pyIbex import *
from vibes import vibes


class ToVibes(SetVisitor):

	"""
	Plot a  box within the frame [-max,max]x[-max,max]
	 
	The frame avoids, in particular, to plot unbounded OUT boxes.
	"""
	def __init__(self, max):
		SetVisitor.__init__(self)
		self.frame = IntervalVector(2, max*Interval(-1, 1))
	"""
	Function that will be called automatically on every boxes (leaves) of the set.
	"""
	def visit_leaf(self, box, status):

		framebox= box & self.frame;

		#  Associate a color to the box.
		#  - YES (means "inside") is in green
		#  - NO (means "outside") is in red
		#  - MAYBE (means "boundary") is in blue.
	  
		if status == YES:
			print('YES')
			color="r"
		elif status == NO:
			color="b"
			print('NO')
		else :
			color="g"
			print('MAYBE')
	  
		# Plot the box with Vibes
		vibes.drawBox(framebox[0].lb(), framebox[0].ub(), framebox[1].lb(), framebox[1].ub(), color);

class myPdc(Pdc):
	def __init__(self):
		Pdc.__init__(self, 2)

	def test(self, X):
		a = X[0]**2 + X[1]**2
		R = Interval(0, 4)
		if (a & R).is_empty():
			return (NO)
		elif a.is_subset(R):
			return (YES)
		return MAYBE

class TestSet(unittest.TestCase):
	def test_Paving1(self):
		pass
		# create the two-dimensional set (-oo,+oo)x(-oo,+oo)
		# X0 = IntervalVector(2, [-10, 10])
		# mypdc = myPdc()
		
		# set1 = Paving(X0, BoolInterval(YES));
		# set2 = Paving(X0, mypdc, 0.1)
		# set3 = Paving(X0, BoolInterval(MAYBE)) 

		# op_binaire(set3, set2, set1, op_Xor)
		# vibes.beginDrawing()
		# set3.visit(ToVibes(10))
		# vibes.endDrawing()

	def test_Paving2(self):
		P = IntervalVector(2, [-4, 4])
		A = Paving(P,YES);
		# B = Paving(P,BoolInterval(YES));
		f = Function("x", "y", "x^2 + y^2 - 4")
		# pdc = PdcFwdBwd(f, CmpOp.LEQ)
		pdc = myPdc()

		A.Sivia(pdc,op_And,0.3);
		vibes.beginDrawing()
		vibes.newFigure('Test')
		A.visit(ToVibes(10))
		vibes.endDrawing()
	# def test_Set2(self):

	# 	vibes.beginDrawing ();
	# 	vibes.newFigure("set-sep");
	
	# 	f = Function("x", "y", "x^2+(y+1)^2 - 4")
	# 	set = Set(f,CmpOp.LEQ, 0.2)
	# 	to_vibes = ToVibes(5)
	# 	set.visit(to_vibes);
	
	# 	vibes.endDrawing();
	
	# def test_Set3(self):
		
	
	# 	fx = Function("x","y","(x^2-[64,81],y^2-[64,81])");
	# 	sepx = SepFwdBwd(fx,CmpOp.LEQ);
	# 	set = Set(2);
	# 	sepx.contract(set,8.0);

	# 	vibes.beginDrawing ();
	# 	vibes.newFigure("set-example");
	
	# 	to_vibes = ToVibes(5)
	# 	set.visit(to_vibes);
	
	# 	vibes.endDrawing();
	# 	set.save("set-example");
	
	# def test_Set4(self):
	# 	pass
	# 	set = Set("set-example");
	# 	to_console = ToConsole()
	# 	set.visit(to_console);
	 


if __name__ == '__main__':

	unittest.main()

