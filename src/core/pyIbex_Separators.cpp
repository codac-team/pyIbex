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
#include "ibex_SepPolygon.h"

#include "ibex_SepInverse.h"
#include "ibex_SepTransform.h"
#include "ibex_SepQInter.h"
#include "ibex_SepCtcPairProj.h"

#include "ibex_Set.h"
#include "ibex_SetInterval.h"
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


void SepPolygonFromList(SepPolygon &instance,  std::vector< std::vector<double> >& lst){
  if (lst.size() != 2){
    throw std::invalid_argument("Invalide input argmment: expected [[ x1, x2, ..., xn], [ y1, y2, ..., yn]]");
  }
  std::vector<double> &lx = lst[0];
  std::vector<double> &ly = lst[1];
  int n = lx.size();
  std::vector<double> ax(n), ay(n),bx(n),by(n);
  for(size_t i = 0; i < n; i++){
      ax[i] = lx[i];
      ay[i] = ly[i];
      bx[i] = lx[ (i+1)%n ];
      by[i] = ly[ (i+1)%n ];
  }
  new(&instance) SepPolygon(ax, ay, bx, by);
}


void export_Separators(py::module& m){

    typedef void (Sep::*separate) (IntervalVector&, IntervalVector&);
    typedef void (Sep::*contract_1) (Set& , double );
    // typedef void (Sep::*contract_2) (SetInterval& , double, ibex::BoolInterval, ibex::BoolInterval);
    py::class_<pySep> sep(m, "Sep");
    sep
      .alias<Sep>()
      .def(init<int>())
      .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &Sep::separate)
      .def("contract", contract_1(&Sep::contract))
      .def("contract", (void (Sep::*) (SetInterval& , double, ibex::BoolInterval, ibex::BoolInterval)) &Sep::contract)
      .def("__or__", &__or, py::return_value_policy::take_ownership, keep_alive<0,1>(),keep_alive<0,2>())
      .def("__and__", &__and, py::return_value_policy::take_ownership, keep_alive<0,1>(),keep_alive<0,2>())
      .def("__invert__", &__not, py::return_value_policy::take_ownership, keep_alive<0,1>())
      .def_readonly("nb_var", &Sep::nb_var);
      ;
    

    class_<SepUnion>(m, "SepUnion", sep)
            .def(init<Array<Sep> >(), keep_alive<1,2>())
            .def("separate", &SepUnion::separate)
            ;

    class_<SepInter>(m, "SepInter", sep)
            .def(init<Array<Sep> >(), keep_alive<1,2>())
            .def("separate", &SepInter::separate)
            ;


    class_<SepCtcPair>(m, "SepCtcPair", sep)
     // ctcPair.alias<SepCtcPair>()
            .def(init<Ctc&, Ctc&>(), keep_alive<1,2>(), keep_alive<1,3>())
            .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &SepCtcPair::separate)
            ;


    class_<SepFwdBwd>(m, "SepFwdBwd", sep)
            .def(init< Function&, CmpOp >(), keep_alive<1,2>())
            .def(init<Function&, Interval& >(), keep_alive<1,2>())
            .def(init<Function&, IntervalVector& >(), keep_alive<1,2>())
            .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &SepFwdBwd::separate)
            ;

    class_<SepNot>(m, "SepNot", sep)
            .def(init<Sep&>(), keep_alive<1,2>())
            .def("separate", &SepNot::separate);

    class_<SepPolygon>(m, "SepPolygon", sep)
            // .def(init<std::vector<double>&, std::vector<double>&, std::vector<double>&, std::vector<double>&>())
            .def("__init__", &SepPolygonFromList)
            .def("separate", (void (Sep::*) (IntervalVector&, IntervalVector&)) &SepPolygon::separate);


    class_<SepQInterProjF>(m, "SepQInterProjF", sep)
            .def(init<Array<Sep> >(), keep_alive<1,2>())
            .def("separate", &SepQInterProjF::separate)
            .def_property("q", &SepQInterProjF::getq, &SepQInterProjF::setq)
            ;

    class_<SepInverse>(m, "SepInverse", sep)
            .def(init<Sep&, Function& >(), keep_alive<1,2>(), keep_alive<1,3>())
            .def("separate", &SepInverse::separate);


    class_<SepTransform>(m, "SepTransform", sep)
            .def(init<Sep&, Function&, Function&>(), keep_alive<1,2>(), keep_alive<1,3>(),  keep_alive<1,4>()) 
            .def("separate", &SepTransform::separate);

    // class_<SepCtcPairProj, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepCtcPairProj> >("SepCtcPairProj", no_init)
    //         .def(init<Ctc&, Ctc&, const IntervalVector&, double>()[with_custodian_and_ward<1,2, with_custodian_and_ward<1,3> >()])
    //         .def("separate", &SepCtcPairProj::separate);
}
