//============================================================================
//                                  I B E X
// File        : Separator for the constraint : Point inside a POLARXY
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#ifndef __IBEX_SEP_POLARXY_H__
#define __IBEX_SEP_POLARXY_H__
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
class SepPolarXY : public Sep {

public:

	SepPolarXY(Interval rho, Interval theta);

  virtual void separate(IntervalVector& x_in, IntervalVector& x_out);

protected:

    void contractIn(IntervalVector &x_in);
    void contractOut(IntervalVector &x_out);


    Interval rho, theta;
    Interval rho_m, rho_p;
    Interval theta_m, theta_p;
    Interval cmpl;
    CtcPolar ctc;
};

} // end namespace

#endif // __IBEX_SEP_POLARXY_H__
