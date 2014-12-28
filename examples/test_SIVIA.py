#!/usr/bin/env python
# import matplotlib as mpl
# mpl.use('pgf')


from pyIbex import *
# import glob
# from time import sleep

# mpl.rcParams['text.latex.unicode']=True
# mpl.rcParams['text.usetex']=True
# mpl.rcParams['pgf.texsystem'] = 'pdflatex'



def contract(X, ctc, res_list):
	X0 = IntervalVector(X)
	ctc.contract(X)
	if( X != X0):
		diff = X0.diff(X)
		for b in diff:
			res_list.append(b)
		
	



X0 = IntervalVector(2, Interval(-5,5))
stack =  [X0]
res_y = []
res_in = []
res_out = []
f = Function("x", "y", "x^2+y^2")
R2 = Interval(4).inflate(0.1)
CtcIn = CtcIn(f, R2)
CtcNotIn = CtcNotIn(f, R2)
lf = LargestFirst(0.01)
while len(stack) > 0:
	X = stack.pop()
	contract(X, CtcNotIn, res_in)
	contract(X, CtcIn, res_out)
	if( X.max_diam() < 0.01):
		res_y.append(X)
		# drawbox(ax, X, "y")
	elif (X.is_empty() == False):
		(X1, X2) = lf.bisect(X)
		stack.append(X1)
		stack.append(X2)

# print(len(res_y),len(res_in), len(res_out))

# import matplotlib.pyplot as plt
# from matplotlib.patches import Rectangle, Circle, Polygon
# import numpy as np
# from numpy import cos,sin
# def drawbox(ax,data,cin,facecolor='b', fill=False):
# 	x = [data[0].lb(), data[0].ub()]
# 	y = [data[1].lb(), data[1].ub()]
	
# 	wx = (x[1] - x[0])
# 	wy = (y[1] - y[0])
# 	x0 = 0.5*(x[0] + x[1] - wx) 
# 	y0 = 0.5*(y[0] + y[1] - wy)
# 	ax.add_patch(Rectangle( (x0,y0),wx,wy,fill=fill,edgecolor=cin, facecolor=facecolor))


# fig = plt.figure()
# ax = fig.add_subplot(111)
# for b in res_out:
# 	drawbox(ax, b, "b")
# for b in res_in:
# 	drawbox(ax, b, "r")
# for b in res_y:
# 	drawbox(ax, b, "y")

# plt.axis([-3, 3, -3, 3])
# plt.show()