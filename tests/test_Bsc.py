#!/usr/bin/env python
import unittest
import pyIbex
from pyIbex import *
class SimplisticTest(unittest.TestCase):
	def test_LargestFirst(self):

		bsc = LargestFirst(0.1)
		a = IntervalVector([[1,2], [2,6]])
		(c,b) = bsc.bisect(a)
		self.assertEqual(c, IntervalVector([[1,2],[2,3.8]]))
		self.assertEqual(b, IntervalVector([[1,2],[3.8,6]]))
		self.assertTrue(True)

	def test_LargestFirst2(self):
		bsc = LargestFirst([0.1, 1])
		a = IntervalVector([[1,2], [2,6]])
		(c,b) = bsc.bisect(a)
		self.assertEqual(c, IntervalVector([[1,1.45],[2,6]]))
		self.assertEqual(b, IntervalVector([[1.45,2],[2,6]]))
		self.assertTrue(True)


if __name__ == '__main__':
	
	unittest.main()

