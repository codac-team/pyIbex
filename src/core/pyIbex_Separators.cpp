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
using namespace pybind11::literals;


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

#include "pyibex_QInterProjF.h"
#include "pyibex_SepTransform.h"
#include "pyibex_SepFixPoint.h"
#include "pyibex_SepProj.h"
#include "pyibex_SepCtcPairProj.h"


#include "pyIbex_doc_Separators.h"

// #include "ibex_SepPolygon.h"
// #include "ibex_SepTransform.h"
// #include "ibex_SepCtcPairProj.h"

// #include "ibex_Set.h"
// #include "ibex_SetInterval.h"
using namespace ibex;
using pyibex::SepTransform;
using pyibex::SepFixPoint;
using pyibex::SepProj;
using pyibex::SepCtcPairProj;
using pyibex::SepQInterProjF;







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


  class_<SepCtcPair> sepCtcPair(m, "SepCtcPair", sep, DOCS_SEP_SEPCTCPAIR);
    sepCtcPair.def(init<Ctc&, Ctc&>(), keep_alive<1,2>(), keep_alive<1,3>(), py::arg("ctc_in"), py::arg("ctc_out"))
    .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &SepCtcPair::separate)
    .def("ctc_in", [](const SepCtcPair* o) -> const Ctc& {return o->ctc_in;}, py::return_value_policy::reference_internal)
    .def("ctc_out", [](const SepCtcPair* o) -> const Ctc& {return o->ctc_out;}, py::return_value_policy::reference_internal)
  ;


  class_<SepFwdBwd>(m, "SepFwdBwd", sepCtcPair, DOCS_SEP_SEPFWDBWD)
    .def(init< Function&, CmpOp >(), keep_alive<1,2>(), py::arg("f"), py::arg("op"))
    .def(init<Function&, Interval& >(), keep_alive<1,2>(), py::arg("f"), py::arg("itv_y"))
    .def(init<Function&, IntervalVector& >(), keep_alive<1,2>(), py::arg("f"), py::arg("box_y"))
    .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &SepFwdBwd::separate)
    // .def("ctc_in", [](const SepFwdBwd* o) -> const Ctc& {return o->ctc_in;}, py::return_value_policy::reference)
    // .def("ctc_out", [](const SepFwdBwd* o) -> const Ctc& {return o->ctc_out;}, py::return_value_policy::reference)
  ;

  class_<SepNot>(m, "SepNot", sep, DOCS_SEP_SEPNOT)
    .def(init<Sep&>(), keep_alive<1,2>(), py::arg("sep"))
    .def("separate", &SepNot::separate)
  ;


  class_<SepQInter>(m, "SepQInter", sep, DOCS_SEP_SEPQINTER)
    .def(init<Array<Sep> >(), keep_alive<1,2>(), py::arg("list"))
    .def("separate", &SepQInter::separate)
    .def_property("q", py::cpp_function(&SepQInter::get_q), py::cpp_function(&SepQInter::set_q))
  ;

  class_<SepQInterProjF>(m, "SepQInterProjF", sep, DOCS_SEP_SEPQINTER)
    .def(init<Array<Sep> >(), keep_alive<1,2>(), py::arg("list"))
    .def("separate", &SepQInterProjF::separate)
    .def_property("q", py::cpp_function(&SepQInterProjF::get_q), py::cpp_function(&SepQInterProjF::set_q))
  ;



  class_<SepInverse>(m, "SepInverse", sep, DOCS_SEP_SEPINVERSE)
    .def(init<Sep&, Function& >(), keep_alive<1,2>(), keep_alive<1,3>(), py::arg("sep"), py::arg("f"))
    .def("separate", &SepInverse::separate)
  ;

  // Export SepProj
  py::class_<pyibex::SepProj>(m, "SepProj", sep, DOCS_SEP_SEPPROJ)
    .def(py::init<ibex::Sep&,const IntervalVector&, double>(), py::keep_alive<1,2>(),
          py::arg("sep"), py::arg("y_init"), py::arg("prec")=1e-3 )
    .def(py::init<ibex::Sep&,const Interval&, double>(), py::keep_alive<1,2>(),
          py::arg("sep"), py::arg("y_init"), py::arg("prec")=1e-3 )
    .def("separate", &pyibex::SepProj::separate)
  ;

  // Export SepCtcPairProj
  py::class_<pyibex::SepCtcPairProj>(m, "SepCtcPairProj", sep, DOCS_SEP_SEPCTCPAIRPROJ)
    .def(py::init<ibex::Ctc&, ibex::Ctc&,const IntervalVector&, double>(), py::keep_alive<1,2>(), py::keep_alive<1,3>(),
          py::arg("ctc_in"), py::arg("ctc_out"), py::arg("y_init"), py::arg("prec"))
    .def(py::init<ibex::Sep&,const IntervalVector&, double>(), py::keep_alive<1,2>(),
          py::arg("sep"), py::arg("y_init"), py::arg("prec") )
    .def("separate", &pyibex::SepCtcPairProj::separate)
  ;

  // Export SepTransform
  py::class_<pyibex::SepTransform>(m, "SepTransform", sep)
    .def(py::init<ibex::Sep&, ibex::Function&, ibex::Function& >(), py::keep_alive<1,2>(), py::keep_alive<1,3>(), py::keep_alive<1,4>())
    .def("separate", &pyibex::SepTransform::separate)
  ;

  // Export SepFixPoint
  py::class_<pyibex::SepFixPoint>(m, "SepFixPoint", sep)
    .def(py::init<ibex::Sep&, double >(), py::keep_alive<1,2>(), "sep"_a, "ratio"_a=0.01)
    .def("separate", &pyibex::SepFixPoint::separate)
  ;

}
