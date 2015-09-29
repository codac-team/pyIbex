#!/usr/bin/env python3
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
import numpy as np
class TestPixelMap(unittest.TestCase):

	def test_1(self):
		a = np.ones((3, 4), np.uint64)
		a = np.flipud(a)
		p = PixelMap2D(a, 0,0, 1, 1)
		p.compute_integral_image()
		p.print()
		ctc = CtcPixelMap(p)
		X0 = IntervalVector(2, [1, 2])
		X1 = X0.copy()
		ctc.contract(X0)
		self.assertEqual(X0, X1)

	def test_2(self):
		a = np.zeros((3, 4), np.uint64)
		a[1, 2] = 1
		a = a.cumsum(axis=0).cumsum(axis=1)
		p = PixelMap2D(a, 0,0, 1, 1)		
		ctc = CtcPixelMap(p)
		X0 = IntervalVector(2, [0, 3])
		X1 = X0.copy()
		ctc.contract(X0)
		self.assertEqual(X0, IntervalVector([[1, 2], [2, 3]]))


if __name__ == '__main__':
	
	unittest.main()

