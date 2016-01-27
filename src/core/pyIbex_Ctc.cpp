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

#include <ibex_Ctc.h>
#include <ibex_CtcFwdBwd.h>
#include <ibex_CtcUnion.h>
#include <ibex_CtcCompo.h>

#include <ibex_CtcNotIn.h>
#include <ibex_Function.h>
#include <ibex_CtcInverse.h>
#include <ibex_CtcSegment.h>
#include <ibex_CtcQInter.h>
#include <ibex_CtcPixelMap.h>

using ibex::Ctc;
using ibex::CtcUnion;
using ibex::CmpOp;
using ibex::FwdMode;
using ibex::CtcUnion;
using ibex::CtcCompo;
using ibex::CtcFwdBwd;
using ibex::CtcInverse;
using ibex::CtcNotIn;
using ibex::CtcSegment;
using ibex::CtcQInterProjF;
using ibex::IntervalVector;
using ibex::Function;
using ibex::Array;
using ibex::Interval;

class pyCtc : public Ctc {
public:
  /* Inherit the constructors */
  using Ctc::Ctc;
  /* Trampoline (need one for each virtual function) */
  virtual void contract(IntervalVector& box){
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

  py::class_<pyCtc> ctc(m, "Ctc");
   ctc.alias<Ctc>()
      .def(py::init<int>())
      .def("contract",(void (ibex::Ctc::*)(IntervalVector&)) &Ctc::contract)
      .def_readonly("nb_var", &Ctc::nb_var)
      .def("__or__", &__or, py::return_value_policy::take_ownership, py::keep_alive<0,1>(),py::keep_alive<0,2>()  )
      .def("__and__", &__and, py::return_value_policy::take_ownership, py::keep_alive<0,1>(),py::keep_alive<0,2>())
    ;

    // Export comparaison constant
    py::enum_<CmpOp>(m, "CmpOp")
            .value( "LT", 	ibex::LT)
            .value( "LEQ", 	ibex::LEQ)
            .value( "EQ", 	ibex::EQ)
            .value( "GEQ", 	ibex::GEQ)
            .value( "GT", 	ibex::GT)
            ;

    // Export computation mode
    py::enum_<FwdMode>(m, "FwdMode")
            .value( "INTERVAL_MODE", ibex::INTERVAL_MODE)
            .value( "AFFINE2_MODE",  ibex::AFFINE2_MODE)
            .value( "AFFINE_MODE",   ibex::AFFINE_MODE)
            ;

//     // Export CtcUnion
    py::class_<CtcUnion>(m, "CtcUnion", ctc)
            .def(py::init<ibex::Array<Ctc> >(), py::keep_alive<1,2>())//[with_custodian_and_ward<1, 2>()])
//            .def("__init__", make_constructor(ctcFromList<CtcUnion>), "CtcUnion from list of contractor\n Usage : CtcUnion([c1, c2, ...])")
            .def("contract", &CtcUnion::contract)
            ;

//     // Export CtcCompo
    py::class_<CtcCompo>(m, "CtcCompo", ctc)
            .def(py::init<ibex::Array<Ctc> >(), py::keep_alive<1,2>()) //[with_custodian_and_ward<1, 2>()])
//            .def("__init__", make_constructor(ctcFromList<CtcCompo>), "CtcCompo from list of contractor\n Usage : CtcCompo([c1, c2, ...])")
            .def("contract", &CtcCompo::contract)
            ;

//     // Export CtcFwdBwd
    py::class_<CtcFwdBwd>(m, "CtcFwdBwd", ctc)
            .def(py::init<Function&> (), py::keep_alive<1,2>()) //[with_custodian_and_ward<1, 2>()])
            .def(py::init<Function&, CmpOp>(), py::keep_alive<1,2>()) //[with_custodian_and_ward<1, 2>()])
            .def(py::init<Function&, CmpOp, FwdMode> (), py::keep_alive<1,2>()) //[with_custodian_and_ward<1, 2>()])
            .def(py::init<Function&,Interval&>(), py::keep_alive<1,2>()) //[with_custodian_and_ward<1, 2>()])
            .def(py::init<Function&,IntervalVector&>(), py::keep_alive<1,2>()) //[with_custodian_and_ward<1, 2>()])
            .def("contract", &CtcFwdBwd::contract)
            ;

//     // Export CtcInverse
    py::class_<CtcInverse>(m, "CtcInverse", ctc)
            .def(py::init<Ctc&, Function&>(),py::keep_alive<1,2>(), py::keep_alive<1,3>())
            .def("contract", &CtcInverse::contract);

//     // Export CtcNotIn
    py::class_<CtcNotIn>(m, "CtcNotIn", ctc)
            .def(py::init<Function&, Interval&>(), py::keep_alive<1,2>())
            .def(py::init<Function&, IntervalVector&>(), py::keep_alive<1,2>())
            .def("contract", &CtcNotIn::contract);

//     // Export CtcSegment
    py::class_<CtcSegment>(m, "CtcSegment", ctc)
            .def(py::init<double, double,double,double>())
            .def("contract", &CtcSegment::contract);

//     // Export CtcQInterProjF
    py::class_<CtcQInterProjF>(m, "CtcQInterProjF", ctc)
            .def(py::init<Array<Ctc>, int>(), py::keep_alive<1,2>())
            .def("contract", &CtcQInterProjF::contract);
}
