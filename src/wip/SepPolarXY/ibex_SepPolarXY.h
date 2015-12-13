//============================================================================
//                                P Y I B E X
// File        : Separator for the constraint : Point inside a Pie
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
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
