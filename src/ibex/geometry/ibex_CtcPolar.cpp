//============================================================================
//                                  I B E X
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#include "ibex_CtcPolar.h"

namespace ibex {

CtcPolar::CtcPolar() : Ctc(4) {}

CtcPolar::~CtcPolar() {}

void CtcPolar::contract(Interval &x, Interval& y, Interval& rho, Interval& theta){
	
	Interval theta_tmp = atan2(y,x);
	bwd_imod(theta_tmp, theta, 2*M_PI);
	bwd_atan2(theta, y, x);


	Interval a1 = sqr(x);
	Interval a2 = sqr(y);
	Interval a3 = a1 + a2;
	Interval a4 = sqr(rho);
	// a4 &= a3;
	a3 &= a4;

	bwd_sqr(a4, rho);
	bwd_add(a3, a2, a1);
	bwd_sqr(a1, x);
	bwd_sqr(a2, y);

	Interval x1 = rho*cos(theta);
	Interval y1 = rho*sin(theta);
	x &= x1;
	y &= y1;
}

void CtcPolar::contract(IntervalVector &box) {

	contract(box[0],box[1],box[2],box[3]);
	contract(box[0],box[1],box[2],box[3]);

}

} // end namespace
