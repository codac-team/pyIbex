//============================================================================
//                                P Y I B E X
// File        : pyIbex_Ctc.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>
#include <pyIbex_type_caster.h>
namespace py = pybind11;
using py::self;
using namespace pybind11::literals;
#include <ibex_Ctc.h>
#include <ibex_CtcFwdBwd.h>
#include <ibex_CtcUnion.h>
#include <ibex_CtcCompo.h>
#include <ibex_LargestFirst.h>
#include <ibex_CtcNotIn.h>
#include <ibex_Function.h>
#include <ibex_CtcInverse.h>
#include <ibex_CtcFixPoint.h>
#include <ibex_CtcQInter.h>

#include "pyibex_CtcHull.h"
#include <pyibex_QInterProjF.h>
#include "pyIbex_doc_Ctc.h"

// #include <ibex_CtcSegment.h>
// #include <ibex_CtcPixelMap.h>
using ibex::Ctc;
using ibex::CtcUnion;
using ibex::CmpOp;
using ibex::CtcUnion;
using ibex::CtcCompo;
using ibex::CtcFwdBwd;
using ibex::CtcInverse;
using ibex::CtcNotIn;
// using ibex::CtcSegment;
using ibex::CtcFixPoint;
using ibex::CtcQInter;
using ibex::IntervalVector;
using ibex::Function;
using ibex::Array;
using ibex::Interval;

using pyibex::CtcQInterProjF;


class pyCtc : public Ctc {
public:
  /* Inherit the constructors */
  using Ctc::Ctc;

  /* Trampoline (need one for each virtual function) */
  void contract(IntervalVector& box) override {
    PYBIND11_OVERLOAD_PURE(
      void,       /* return type */
      Ctc,        /* Parent class */
      contract,   /* Name of function */
      box         /* Argument(s) */
    );
  }
};


CtcUnion* __or(Ctc& c1, Ctc& c2){ return (new CtcUnion(c1, c2)); }
CtcCompo* __and(Ctc& c1, Ctc& c2){ return (new CtcCompo(c1, c2)); }

void export_Ctc(py::module& m){

  py::class_<Ctc, std::unique_ptr<Ctc>, pyCtc > ctc(m, "Ctc", DOCS_CTC_TYPE);
  ctc.def(py::init<int>())
    // .def("contract",(void (ibex::Ctc::*)(IntervalVector&)) &Ctc::contract, DOC_CTC_CONTRACT)
    .def_readonly("nb_var", &Ctc::nb_var, DOC_CTC_NB_VAR)
    .def("__or__", &__or, DOC_CTC_OR, py::return_value_policy::take_ownership, py::keep_alive<0,1>(),py::keep_alive<0,2>()  )
    .def("__and__", &__and, DOC_CTC_AND, py::return_value_policy::take_ownership, py::keep_alive<0,1>(),py::keep_alive<0,2>())
  ;

  // Export comparaison constant
  py::enum_<CmpOp>(m, "CmpOp", DOC_CMPOP_TYPE)
    .value( "LT", 	ibex::LT)
    .value( "LEQ", 	ibex::LEQ)
    .value( "EQ", 	ibex::EQ)
    .value( "GEQ", 	ibex::GEQ)
    .value( "GT", 	ibex::GT)
    .export_values()
  ;

  // Export CtcUnion
  py::class_<CtcUnion>(m, "CtcUnion", ctc, DOC_CTCUNION_TYPE)
    .def(py::init<ibex::Array<Ctc> >(), py::keep_alive<1,2>(), "list"_a)
    .def("contract", &CtcUnion::contract)
    ;

  // Export CtcCompo
  py::class_<CtcCompo>(m, "CtcCompo", ctc, DOC_CTCCOMPO_TYPE)
    .def(py::init<ibex::Array<Ctc> >(), py::keep_alive<1,2>(), "list"_a)
    .def("contract", &CtcCompo::contract)
    ;

  // Export CtcFwdBwd
  py::class_<CtcFwdBwd>(m, "CtcFwdBwd", ctc, DOC_CTCFWDBWD_TYPE)
    .def(py::init<Function&, CmpOp>(), py::keep_alive<1,2>(), "f"_a, "op"_a=ibex::EQ)
    .def(py::init<Function&,Interval&>(), py::keep_alive<1,2>(), "f"_a, "itv_y"_a)
    .def(py::init<Function&,IntervalVector&>(), py::keep_alive<1,2>(), "f"_a, "box_y"_a)
    .def("contract", &CtcFwdBwd::contract)
    ;

  // Export CtcInverse
  py::class_<CtcInverse>(m, "CtcInverse", ctc, DOC_CTCINVERSE_TYPE)
    .def(py::init<Ctc&, Function&>(),py::keep_alive<1,2>(), py::keep_alive<1,3>(), "ctc"_a, "f"_a)
    .def("contract", &CtcInverse::contract, py::arg("box").noconvert())
    ;

  // Export CtcNotIn
  py::class_<CtcNotIn>(m, "CtcNotIn", ctc, DOC_CTCNOTIN_TYPE)
    .def(py::init<Function&, Interval&>(), py::keep_alive<1,2>())
    .def(py::init<Function&, IntervalVector&>(), py::keep_alive<1,2>())
    .def("contract", &CtcNotIn::contract)
    ;

  // Export CtcNotIn
  py::class_<CtcFixPoint>(m, "CtcFixPoint", ctc, DOC_CTCFIXPOINT_TYPE)
    .def(py::init<Ctc&, double>(), py::keep_alive<1,2>(), "ctc"_a, "ratio"_a=1e-3)
    .def("contract", &CtcFixPoint::contract)
    ;


  // Export CtcQInterProjF
  py::class_<CtcQInter>(m, "CtcQInter", ctc, DOC_CTCQINTER_TYPE)
    .def(py::init<Array<Ctc>, int>(), py::keep_alive<1,2>())
    .def("contract", &CtcQInter::contract, py::arg("box").noconvert())
    ;

  // Export CtcHull
  py::class_<pyibex::CtcHull>(m, "CtcHull", ctc)
    .def(py::init<ibex::Sep&, double, ibex::Bsc&>(),
            py::keep_alive<1,2>(), py::keep_alive<1,4>(),
            "sep"_a, "epsilon"_a, "bsc"_a = ibex::LargestFirst(1e-10)
          )
    .def("contract", &pyibex::CtcHull::contract, py::arg("box").noconvert())
    ;

}
