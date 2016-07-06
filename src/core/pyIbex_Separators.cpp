//============================================================================
//                               P Y I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================


#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pyIbex_type_caster.h>
#include <pybind11/stl.h>
namespace py = pybind11;
using py::self;
using py::class_;
using py::init;
using py::keep_alive;


#include "ibex_IntervalVector.h"
#include "ibex_Sep.h"
#include "ibex_SepUnion.h"
#include "ibex_SepInter.h"
#include "ibex_SepNot.h"
#include "ibex_SepCtcPair.h"
#include "ibex_SepFwdBwd.h"
#include "ibex_SepBoundaryCtc.h"
#include "ibex_SepInverse.h"
#include "ibex_SepQInter.h"

#include "pyIbex_Separators_doc.h"

// #include "ibex_SepPolygon.h"
// #include "ibex_SepTransform.h"
// #include "ibex_SepCtcPairProj.h"

// #include "ibex_Set.h"
// #include "ibex_SetInterval.h"
using namespace ibex;





class pySep : public Sep {
public:
  /* Inherit the constructors */
  using Sep::Sep;
  /* Trampoline (need one for each virtual function) */
  virtual void separate(IntervalVector& xin, IntervalVector& xout){
    PYBIND11_OVERLOAD_PURE(
      void,       /* return type */
      Sep,        /* Parent class */
      separate,   /* Name of function */
      xin, xout          /* Argument(s) */
    );
  }
};


SepUnion* __or(Sep& s1, Sep& s2) { return (new SepUnion(s1, s2)); }
SepInter* __and(Sep& s1, Sep& s2){ return (new SepInter(s1, s2)); }
SepNot* __not(Sep& s1){ return (new SepNot(s1)); }


void export_Separators(py::module& m){

  // typedef void (Sep::*separate) (IntervalVector&, IntervalVector&);
  // typedef void (Sep::*contract_1) (Set& , double );

  py::class_<Sep, std::unique_ptr<Sep>, pySep> sep(m, "Sep", DOCS_SEP_TYPE);
  sep.def(init<int>())
    .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &Sep::separate, DOCS_SEP_SEPARATE,
                      py::arg("x_in"), py::arg("x_out"))
    .def_readonly("nb_var", &Sep::nb_var, DOCS_SEP_NBVAR)
    .def("__or__", &__or, py::return_value_policy::take_ownership, keep_alive<0,1>(),keep_alive<0,2>(), DOCS_SEP_OR)
    .def("__and__", &__and, py::return_value_policy::take_ownership, keep_alive<0,1>(),keep_alive<0,2>(), DOCS_SEP_AND)
    .def("__invert__", &__not, py::return_value_policy::take_ownership, keep_alive<0,1>(), DOCS_SEP_INVERSE)
  ;


  class_<SepUnion>(m, "SepUnion", sep, DOCS_SEP_SEPUNION)
    .def(init<Array<Sep> >(), keep_alive<1,2>(), py::arg("list"))
    .def("separate", &SepUnion::separate)
  ;

  class_<SepInter>(m, "SepInter", sep, DOCS_SEP_SEPINTER)
    .def(init<Array<Sep> >(), keep_alive<1,2>(), py::arg("list"))
    .def("separate", &SepInter::separate)
  ;


  class_<SepCtcPair>(m, "SepCtcPair", sep, DOCS_SEP_SEPCTCPAIR)
    .def(init<Ctc&, Ctc&>(), keep_alive<1,2>(), keep_alive<1,3>(), py::arg("ctc_in"), py::arg("ctc_out"))
    .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &SepCtcPair::separate)
    .def("ctc_in", [](const SepCtcPair* o) -> const Ctc& {return o->ctc_in;})
    .def("ctc_out", [](const SepCtcPair* o) -> const Ctc& {return o->ctc_out;})
  ;


  class_<SepFwdBwd>(m, "SepFwdBwd", sep, DOCS_SEP_SEPFWDBWD)
    .def(init< Function&, CmpOp >(), keep_alive<1,2>(), py::arg("f"), py::arg("op"))
    .def(init<Function&, Interval& >(), keep_alive<1,2>(), py::arg("f"), py::arg("itv_y"))
    .def(init<Function&, IntervalVector& >(), keep_alive<1,2>(), py::arg("f"), py::arg("box_y"))
    .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &SepFwdBwd::separate)
    .def("ctc_in", [](const SepFwdBwd* o) -> const Ctc& {return o->ctc_in;})
    .def("ctc_out", [](const SepFwdBwd* o) -> const Ctc& {return o->ctc_out;})
  ;

  class_<SepNot>(m, "SepNot", sep, DOCS_SEP_SEPNOT)
    .def(init<Sep&>(), keep_alive<1,2>(), py::arg("sep"))
    .def("separate", &SepNot::separate)
  ;


  class_<SepQInter>(m, "SepQInter", sep, DOCS_SEP_SEPQINTER)
    .def(init<Array<Sep> >(), keep_alive<1,2>(), py::arg("list"))
    .def("separate", &SepQInter::separate)
    .def_property("q", &SepQInter::get_q, &SepQInter::set_q)
  ;

  class_<SepInverse>(m, "SepInverse", sep, DOCS_SEP_SEPINVERSE)
    .def(init<Sep&, Function& >(), keep_alive<1,2>(), keep_alive<1,3>(), py::arg("sep"), py::arg("f"))
    .def("separate", &SepInverse::separate)
  ;

}
