#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_SepPolygon.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================


import unittest
import pyIbex
from pyIbex import *
import sys

class TestSepPolygon(unittest.TestCase):
  def test_SIVIA(self):
    l = [[-1, 1, 0], [-1, -2, 1]]
    
    sep = SepPolygon(l)
    f = Function('x', 'y', '(2*x-y, 2*y - x)')
    finv = Function('x', 'y', '(1/3.0*(2*x+y),1/3.0*(2*y+x))')
    # sepInv = SepInverse(sep, f)
    # sepTrans = SepTransform(sep, f, finv)
    
    Xin = IntervalVector(2, Interval(-10, 10))
    Xout = IntervalVector(2, Interval(-10, 10))
    
    eps = 0.05
    # sep.separate(Xin, Xout)
    pySIVIA(Xin, sep, eps, figureName='Sep', draw_boxes=False, )
    # SIVIA(Xin, sepInv, figureName='SepInv',  eps=eps)
    # SIVIA(Xin, sepTrans, eps=eps, figureName='sepTransfrom')

if __name__ == '__main__':
  unittest.main()

