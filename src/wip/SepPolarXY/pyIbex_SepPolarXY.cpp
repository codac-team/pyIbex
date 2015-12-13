//============================================================================
//                               P Y I B E X
// File        : pyIbex_SepPolarXY.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================

#include "ibex_Interval.h"
#include "ibex_Sep.h"
#include "ibex_SepPolarXY.h"
#include <boost/python.hpp>


using namespace boost;
using namespace boost::python;
using namespace ibex;



void export_SepPolarXY(){

    class_<SepPolarXY, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepPolarXY> >("SepPolarXY", no_init)
        .def(init<Interval, Interval>())
        .def("separate", &SepPolarXY::separate);
}
