#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_polar.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================from pyibex import Interval, IntervalVector
from pyibex.geometry import *
from pyibex import Interval
import unittest

class TestPolar(unittest.TestCase):
  def test_Polar01(self):
    self.assertTrue(True)

  def test_sepPolar(self):
    	sep = SepPolarXY(Interval(1,2), Interval(0.1, 2.1))



if __name__ == '__main__':

	unittest.main()
