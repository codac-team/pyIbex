from pyIbex import Interval, IntervalVector, Function, Paving, pySIVIA
from pyIbex import SetVisitor, PdcFwdBwd, YES, NO, MAYBE, EMPTY
from pyIbex import BoolInterval, CmpOp, PdcAnd, PdcOr, PdcNot, op_binaire, op_Restrict
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
    A = Paving(P,BoolInterval(YES))
    f = Function("x", "y", "x^2 + y^2")
    pdcA = pyPdcIn(f, IntervalVector(1, [0, 3]), IntervalVector(1, [4, 1000]))
    A.Sivia(pdcA,ibex.op_And,0.03);
    A.visit(ToVibes(10, "Aset"))
    
    B = Paving(P,BoolInterval(YES));
    f = Function("x[2]", 'x[0]^2 + (1 + x[1])^2 + (x[0] + x[1])^2')
    pdcB = pyPdcIn(f, IntervalVector(1, [0, 3]), IntervalVector(1, [6, 10000]))
    B.Sivia(pdcB,ibex.op_And,0.03);
    B.visit(ToVibes(10, "Bset"))
    
    AmoinsB = Paving(P, MAYBE);
    op_binaire(AmoinsB,A,B,ibex.op_Restrict);
    AmoinsB.visit(ToVibes(10, "AmoinsB"))
    
    BmoinsA = Paving(2);
    op_binaire(BmoinsA,B,A,ibex.op_Restrict);
    BmoinsA.visit(ToVibes(10, "BmoinsA"))
    
    D = Paving(2)
    op_binaire(D, BmoinsA,AmoinsB,ibex.op_Or);
    D.visit(ToVibes(10, "BmoinsA cup BmoinsA"))
    
    D2 = Paving(2)
    op_binaire(D2, A, B, ibex.op_Xor);
    D2.visit(ToVibes(10, "AxorB"))


    AunionB = Paving(2)
    op_binaire(AunionB, A, B, ibex.op_Or);
    AunionB.visit(ToVibes(10, "AunionB"))

    AinterB = Paving(2)
    op_binaire(AinterB, A, B, ibex.op_And);
    AinterB.visit(ToVibes(10, "AinterB"))