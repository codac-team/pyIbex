//============================================================================
//                                  I B E X                                   
// File        : ibex_sdSepCtcPairProj.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================
#include "ibex_SepCtcPairProj.h"

#include "vibes.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_EmptyBoxException.h"
using namespace std;

namespace ibex {

SepCtcPairProj::SepCtcPairProj(Ctc& ctc_in, Ctc& ctc_out, const IntervalVector& y_init, double prec) :
    SepCtcPair(ctc_in, ctc_out),
    vars(BitSet::empty(ctc_in.nb_var+y_init.size()))
{
    for(int i = 0; i < ctc_in.nb_var; i++){
        vars.add(true);
    }
    for(int i = 0; i < y_init.size(); i++){
        vars.add(false);
    }
    ctcForAll = new CtcForAll(ctc_in, vars, y_init, prec);
    ctcExist = new CtcExist(ctc_out, vars, y_init, prec);

}

SepCtcPairProj::~SepCtcPairProj() {
    delete ctcExist;
    delete ctcForAll;
}

void SepCtcPairProj::separate(IntervalVector &x_in, IntervalVector &x_out)
{

    assert(x_in==x_out);

    try {
        ctcExist->contract(x_out);
    } catch (EmptyBoxException&) {

    }
    try {
        ctcForAll->contract(x_in);
    } catch (EmptyBoxException&) {

    }

}





} // namespace ibex

