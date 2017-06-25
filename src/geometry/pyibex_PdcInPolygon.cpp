//============================================================================
//                                P Y I B E X
// File        : ibex_PdcInPolygon.cpp
// Author      : Benoit Desrochers, Gilles Chabert
// Copyright   :  Benoit Desrochers
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#define _USE_MATH_DEFINES
#include <cmath>
#include "pyibex_PdcInPolygon.h"

using namespace std;

namespace pyibex {

PdcInPolygon::PdcInPolygon(vector< vector< vector<double> > > &points) : Pdc(2) {
    ax.resize(points.size()); ay.resize(points.size()); bx.resize(points.size()); by.resize(points.size());
    for(int i=0; i< points.size(); i++)
    {
        ax[i] = points[i][0][0];
        ay[i] = points[i][0][1];
        bx[i] = points[i][1][0];
        by[i] = points[i][1][1];
    }
}

PdcInPolygon::PdcInPolygon(vector<double> &_ax, vector<double> &_ay, vector<double> &_bx, vector<double> &_by) : Pdc(2),
            ax(_ax),
            ay(_ay),
            bx(_bx),
            by(_by) {
}

namespace {

Interval argument(Interval mx, Interval my, double xa, double ya, double xb, double yb) {
    Interval ma_x = Interval(xa) - (mx);
    Interval ma_y = Interval(ya) - (my);
    Interval mb_x = Interval(xb) - (mx);
    Interval mb_y = Interval(yb) - (my);

    Interval norm_ma = sqrt( sqr(ma_x) + sqr(ma_y) );
    Interval norm_mb = sqrt( sqr(mb_x) + sqr(mb_y) );

    Interval cos_theta = (ma_x*mb_x + ma_y*mb_y) / (norm_ma*norm_mb);
    Interval sin_theta = (ma_x*mb_y - ma_y*mb_x) / (norm_ma*norm_mb);

    return atan2(sin_theta,cos_theta);
}

} // end anonymous namespace


BoolInterval PdcInPolygon::test(const IntervalVector& x) {

	Interval mx = Interval(x[0].mid());
	Interval my = Interval(x[1].mid());

    Interval theta = Interval(0);
    for(unsigned int i = 0; i < ax.size(); i++) {
        theta += argument(mx,my,ax[i],ay[i],bx[i],by[i]);
    }

    if(!theta.contains(0)) {
        return ibex::YES;
    } else if (!theta.contains(2*M_PI) && theta.contains(0)) {
        return ibex::NO;
    } else {
        // Undetermined case
        return ibex::MAYBE;
    }
}

} // namespace pyibex
