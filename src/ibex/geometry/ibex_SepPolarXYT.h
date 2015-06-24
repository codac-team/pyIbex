//============================================================================
//                                  I B E X
// File        : Separator for the constraint : Point inside a POLARXY
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#ifndef __IBEX_SEP_POLARXYT_H__
#define __IBEX_SEP_POLARXYT_H__
#include "ibex_Sep.h"
#include "ibex_CtcPolar.h"

using namespace std;

namespace ibex {

/**
 * \ingroup iset
 *
 * \brief Separator for point in sector.
 * A sector is defined by its center, a distance and an angle (with uncertainty).
 *
 */
class SepPolarXYT : public Sep {

public:

	SepPolarXYT(Interval rho, Interval theta, double mx, double my);

  virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

protected:
	void contract(Interval &x, Interval &y, Interval &theta, Interval &rho, Interval &phi);
    void contractIn(IntervalVector &x_in);
    void contractOut(IntervalVector &x_out);


   	double mx, my;
    Interval rho, theta;
    Interval rho_m, rho_p;
    Interval theta_m, theta_p;
    Interval cmpl;
};

} // end namespace

#endif // __IBEX_SEP_POLARXY_H__
