//============================================================================
//                               P Y I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================

#include "ibex_Sep.h"
#include "ibex_Interval.h"

#include "pyibex_Catan2.h"
#include "pyibex_CtcPolar.h"
#include "pyibex_SepPolarXY.h"

#include "pyibex_SepPolygon.h"
#include "pyibex_CtcSegment.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace pybind11::literals;
namespace py = pybind11;

using ibex::Interval;
using ibex::IntervalVector;
// using namespace pyibex;



void SepPolygonFromList(pyibex::SepPolygon &instance,  std::vector< std::array<double, 2> >& lst){
  // if (lst.size() != 2){
  //   throw std::invalid_argument("Invalide input argmment: expected [[ x1, x2, ..., xn], [ y1, y2, ..., yn]]");
  // }
  // if (lst.size() != 2){
  //   throw std::invalid_argument("Invalide input argmment: expected [[ x1, x2, ..., xn], [ y1, y2, ..., yn]]");
  // }
  // std::vector<double> &lx = lst[0];
  // std::vector<double> &ly = lst[1];
  int n = lst.size();
  std::vector<double> ax(n), ay(n),bx(n),by(n);
  for(size_t i = 0; i < n; i++){
      ax[i] = lst[i][0];
      ay[i] = lst[i][1];
      bx[i] = lst[ (i+1)%n ][0];
      by[i] = lst[ (i+1)%n ][1];
  }
  new(&instance) pyibex::SepPolygon(ax, ay, bx, by);
}

PYBIND11_MODULE(geometry, m)
{
  // py::module m("geometry","Python binding of ibex-geometry functions");

  py::object ctc = (py::object) py::module::import("pyibex").attr("Ctc");
  py::object sep = (py::object) py::module::import("pyibex").attr("Sep");

  // Export CtcAngle
  // py::class_<ibex::CtcAngle>(m, "CtcAngle", ctc)
  //     .def(py::init<>())
  //     .def("contract", ( void (ibex::CtcAngle::*) (IntervalVector&) ) &ibex::CtcAngle::contract)
  //     .def("contract", ( void (ibex::CtcAngle::*) (Interval&, Interval&, Interval&)) &ibex::CtcAngle::contract)
  // ;
  //
  // // Export CtcPolar
  py::class_<pyibex::CtcPolar>(m, "CtcPolar", ctc)
      .def(py::init<>())
      .def("contract", ( void (pyibex::CtcPolar::*) (IntervalVector&) ) &pyibex::CtcPolar::contract)
      .def("contract", ( void (pyibex::CtcPolar::*) (Interval&, Interval&, Interval&, Interval&) ) &pyibex::CtcPolar::contract)
      .def("RTfromXY", &pyibex::CtcPolar::RTfromXY)
  ;
  // Export CtcPolarXY
  py::class_<pyibex::CtcPolarXY>(m, "CtcPolarXY", ctc)
      .def(py::init<const Interval &, const Interval& >())
      .def("contract", ( void (pyibex::CtcPolarXY::*) (IntervalVector&) ) &pyibex::CtcPolarXY::contract)
  ;
  // Export CtcPolarXY_landmark
  py::class_<pyibex::CtcPolarXY_landmark>(m, "CtcPolarXY_landmark", ctc)
      .def(py::init<const Interval&, const Interval&, const Interval&, const Interval& >())
      .def("contract", ( void (pyibex::CtcPolarXY_landmark::*) (IntervalVector&) ) &pyibex::CtcPolarXY_landmark::contract)
  ;

  // Export CtcPolarXY_landmark
  py::class_<pyibex::CtcPolarXYT_landmark>(m, "CtcPolarXYT_landmark", ctc)
      .def(py::init<const Interval&, const Interval&, const Interval&, const Interval& >())
      .def("contract", ( void (pyibex::CtcPolarXYT_landmark::*) (IntervalVector&) ) &pyibex::CtcPolarXYT_landmark::contract)
  ;

  // // Export SepPolarXY
  py::class_<pyibex::SepPolarXY>(m, "SepPolarXY", sep)
      .def(py::init<Interval, Interval>())
      .def("separate", &pyibex::SepPolarXY::separate)
  ;

  // py::class_<pyibex::SepPolarXYT>(m, "SepPolarXYT", sep)
  //   .def(py::init<Interval, Interval , double , double >())
  //   .def("separate", &pyibex::SepPolarXYT::separate)
  // ;
  //
  // // Export CtcSegment
  py::class_<pyibex::CtcSegment>(m, "CtcSegment", ctc)
      .def(py::init<double, double,double,double>())
      .def("contract", &pyibex::CtcSegment::contract)
  ;
  //
  // // Export SepPolygon
  py::class_<pyibex::SepPolygon>(m, "SepPolygon", sep)
    .def("__init__", &SepPolygonFromList)
    .def(py::init<std::vector< std::vector< std::vector<double> > >& >())
    .def(py::init<std::vector<double>&, std::vector<double>&, std::vector<double>&, std::vector<double>&>())
    .def("separate", (void (ibex::Sep::*) (IntervalVector&, IntervalVector&)) &pyibex::SepPolygon::separate)
  ;

  //
  //
  // m.def("bwd_angle", &pyibex::bwd_angle);
  m.def("Catan2", &pyibex::Catan2);
  m.def("Cmod", &pyibex::Cmod);
  m.def("Cmod_bwd", &pyibex::Cmod_bwd);
  // return m.ptr();

}
