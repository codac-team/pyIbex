from pyIbex import Interval, IntervalVector, Function, Paving, pySIVIA
from pyIbex import SetVisitor, PdcFwdBwd, YES, NO, MAYBE, EMPTY
from pyIbex import BoolInterval, CmpOp, PdcAnd, PdcOr, PdcNot, op_binaire, op_unaire, op_Restrict, op_Not
from vibes import vibes
import pyIbex as ibex
from pyPredicate import pyPdcIn
from pyVisitors import ToVibes
import numpy as np

if __name__ == '__main__':

    params = {'color_in': '#888888[#444444]', 'color_out':
                      '#888888[#DDDDDD]', 'color_maybe': '#888888[w]'}
    vibes.beginDrawing()
    
    
    P = IntervalVector(2, [-4, 4])
    X = Paving(P,BoolInterval(YES))
    f = Function("x", "y", "x^2 + y^2")
    pdcX = pyPdcIn(f, IntervalVector(1, [0, 3]), IntervalVector(1, [4, 1000]))
    X.Sivia(pdcX,ibex.op_And,0.03);
    X.visit(ToVibes(10, "X"))
    
    B_in = Paving(P,BoolInterval(YES));
    f = Function("x[2]", '(x[0]-0.5)^2 + x[1]^2')
    pdcB_in = pyPdcIn(f, IntervalVector(1, [0, 0.03]))
    B_in.Sivia(pdcB_in,ibex.op_And,0.03);
    B_in.visit(ToVibes(10, "B_in"))

    B_out = Paving(P,BoolInterval(YES));
    f = Function("x[2]", '(x[0]-1)^2 + (x[1]-1)^2')
    pdcB_out = pyPdcIn(f, IntervalVector(1, [0, 0.15]))
    B_out.Sivia(pdcB_out,ibex.op_And,0.03);
    B_out.visit(ToVibes(10, "B_out"))

    W = Paving(2)
    op_unaire(W,X,op_Not);
    W.visit(ToVibes(10, "W"))    

