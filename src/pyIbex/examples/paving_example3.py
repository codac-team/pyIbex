from pyIbex import Interval, IntervalVector, Function, Paving, pySIVIA
from pyIbex import SetVisitor, PdcFwdBwd, YES, NO, MAYBE, EMPTY
from pyIbex import BoolInterval, CmpOp, PdcAnd, PdcOr, PdcNot, op_binaire, op_unaire, op_Restrict, op_Not, op_And, Sqcap
from pyIbex import Set_Contractor_Subset, Set_Contractor_Disjoint
from vibes import vibes
import pyIbex as ibex
from pyPredicate import pyPdcIn, pyPdcTransform, pyPdcInSet
from pyVisitors import ToVibes
import numpy as np
import math

if __name__ == '__main__':

    params = {'color_in': '#888888[#444444]', 'color_out':
                  '#888888[#DDDDDD]', 'color_maybe': '#888888[w]'}
    vibes.beginDrawing()
    
    P = IntervalVector(2, [-4, 4])
    A = Paving(P,YES);
    X = Paving(P,MAYBE);

    B_in = Paving(P,YES);
    B_out = Paving(P,YES);
    eps = 0.02

    f = Function("x", "y", "x^2 + y^2")
    ThickDisk = pyPdcIn(f, IntervalVector(1, [0, 3]))
    f = Function("x[2]", '(x[0]-0.5)^2 + x[1]^2')
    Pastille_in = pyPdcIn(f, IntervalVector(1, [0, 0.3]))
    f = Function("x[2]", '(x[0]-1)^2 + (x[1]-1)^2')
    Pastille_out = pyPdcIn(f, IntervalVector(1, [0, 0.15]))

    A.Sivia(ThickDisk,op_And,eps);
    A.visit(ToVibes(10, "A"))
    
    B_in.Sivia(Pastille_in,op_And,eps);
    B_in.visit(ToVibes(10, "B_in"))
    
    B_out.Sivia(Pastille_out,op_And,eps);
    B_out.visit(ToVibes(10, "B_out"))
    
    
    Set_Contractor_Subset(X,A);
    X.visit(ToVibes(10, "Set_Contractor_Subset"))
    
    Set_Contractor_Subset(B_in,X);
    X.visit(ToVibes(10, "Set_Contractor_Subset Bin"))
    
    Set_Contractor_Disjoint(B_out,X);
    X.visit(ToVibes(10, "Set_Contractor_Disjoint"))
    
    theta = math.pi/6.0
    f = Function("x", "y", "(cos(%f)*x - sin(%f)*y, sin(%f)*x + cos(%f)*y)"%(theta,theta, theta, theta ))    
    pdcY = pyPdcTransform(f, pyPdcInSet(X))
    Y = Paving(P,YES);
    Y.Sivia(pdcY,op_And,eps);
    X=Sqcap(Y,X);
    X.visit(ToVibes(10, "Sqcap(Y,X)"))
   
    for k in range(0,10):
        Y = Paving(P,YES);
        pdcY = pyPdcTransform(f, pyPdcInSet(X))
        Y.Sivia(pdcY,op_And,eps);
        X=Sqcap(Y,X);
        X.visit(ToVibes(10, "Sqcap(Y,X)x10"))
