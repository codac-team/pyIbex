//============================================================================
//                               P Y I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================

#include "ibex_Interval.h"
#include "ibex_CtcAngle.h"
#include "ibex_CtcPolar.h"
#include "ibex_Sep.h"
#include "ibex_SepPolarXY.h"


#include <pybind11/pybind11.h>

namespace py = pybind11;

using ibex::CtcAngle;
using ibex::CtcPolar;
using ibex::SepPolarXY;
using ibex::Interval;
using ibex::IntervalVector;

PYBIND11_PLUGIN(polar)
{
  py::module m("polar", "python binding of ibex core fonctionnality");

  py::object ctc = (py::object) py::module::import("pyIbex").attr("Ctc");
  py::object sep = (py::object) py::module::import("pyIbex").attr("Sep");

    // Export CtcAngle
    py::class_<CtcAngle>(m, "CtcAngle", ctc)
        .def(py::init<>())
        .def("contract", ( void (CtcAngle::*) (IntervalVector&) ) &CtcAngle::contract)
        .def("contract", ( void (CtcAngle::*) (Interval&, Interval&, Interval&)) &CtcAngle::contract)
    ;

    // Export CtcPolar
    py::class_<CtcPolar>(m, "CtcPolar", ctc)
        .def(py::init<>())
        .def("contract", ( void (CtcPolar::*) (IntervalVector&) ) &CtcPolar::contract)
        .def("contract", ( void (CtcPolar::*) (Interval&, Interval&, Interval&, Interval&) ) &CtcPolar::contract)
    ;

    // Export SepPolarXY
    py::class_<SepPolarXY>(m, "SepPolarXY", sep)
        .def(py::init<Interval, Interval>())
        .def("separate", &SepPolarXY::separate)
    ;

    m.def("bwd_angle", &ibex::bwd_angle);
    return m.ptr();

}
