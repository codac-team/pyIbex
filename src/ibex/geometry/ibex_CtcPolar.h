//============================================================================
//                                  I B E X
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 24, 2014
// Last Update : Mar 24, 2014
//============================================================================

#ifndef __IBEX_CTC_POLAR_H__
#define __IBEX_CTC_POLAR_H__

#include "ibex_IntervalVector.h"
#include "ibex_Ctc.h"

using namespace std;

namespace ibex {

/**
 * \ingroup geometry
 * Minimal contractor for the polar constraint: 
 *  x = rho*cos(theta)  
 *  y = rho*sin(theta)
 */
class CtcPolar : public Ctc {

public:
	CtcPolar();
	/**
	 * \brief Contract a box.
	 * 	the box = ([x],[y],[rho],[theta])
	 */
	virtual void contract(IntervalVector& box);
	
	void contract(Interval &x, Interval& y, Interval& rho, Interval& theta);

	/**
	 * \brief Delete this.
	 */
	~CtcPolar();

protected:
};

} // end namespace

#endif // __IBEX_CTC_POLAR_H__
