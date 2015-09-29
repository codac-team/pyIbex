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


# example from doc-set.cpp

class ToConsole(SetVisitor):

#The function that will be called automatically on every boxes (leaves) of the set.
	def __init__(self):
		SetVisitor.__init__(self)

	def visit_leaf(self, box, status):

		output =  str(box) + " : "

		if status == YES:
			output += 'in'
		elif (status == NO):
			output += 'out'
		elif status == MAYBE:
			output += '?'
		print(output)      

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
			color="r"
		elif status == NO:
			color="b"
		else :
			color="g"
	  
		# Plot the box with Vibes
		vibes.drawBox(framebox[0].lb(), framebox[0].ub(), framebox[1].lb(), framebox[1].ub(), color);


class TestSet(unittest.TestCase):
	def test_Set1(self):
		# create the two-dimensional set (-oo,+oo)x(-oo,+oo)
		set1 = Set(2);
	
			# create the two-dimensional set [0,1]x[0,1]
		set2 = Set(IntervalVector(2,[0,1]));

	def test_Set2(self):

		vibes.beginDrawing ();
		vibes.newFigure("set-sep");
	
		f = Function("x", "y", "x^2+(y+1)^2 - 4")
		set = Set(f,CmpOp.LEQ, 0.2)
		to_vibes = ToVibes(5)
		set.visit(to_vibes);
	
		vibes.endDrawing();
	
	def test_Set3(self):
		
	
		fx = Function("x","y","(x^2-[64,81],y^2-[64,81])");
		sepx = SepFwdBwd(fx,CmpOp.LEQ);
		set = Set(2);
		sepx.contract(set,8.0);

		vibes.beginDrawing ();
		vibes.newFigure("set-example");
	
		to_vibes = ToVibes(5)
		set.visit(to_vibes);
	
		vibes.endDrawing();
		set.save("set-example");
	
	def test_Set4(self):
		pass
		set = Set("set-example");
		to_console = ToConsole()
		set.visit(to_console);
	 


if __name__ == '__main__':

	unittest.main()

