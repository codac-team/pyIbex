//============================================================================
//                               P Y I B E X
// File        : pyIbex_Predicate.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Sep 19, 2015
//============================================================================


#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
#include "ibex_Pdc.h"
#include "ibex_PdcAnd.h"
#include "ibex_PdcOr.h"
#include "ibex_PdcCleared.h"
#include "ibex_PdcFwdBwd.h"


#include "pyIbex_to_python_converter.h"
#include <boost/python.hpp>
using namespace boost;
using namespace boost::python;
namespace py = boost::python;

using namespace ibex;


struct PdcWrap : Pdc, wrapper<Pdc> {
    PdcWrap(int nb_var) : Pdc(nb_var) {}
    ibex::BoolInterval test(const IntervalVector& box){
        this->get_override("test")( ::ref(box));
    }
};

PdcOr* __or(Pdc& c1, Pdc& c2){ return (new PdcOr(c1, c2)); }
PdcAnd* __and(Pdc& c1, Pdc& c2){ return (new PdcAnd(c1, c2)); }

void export_Predicate(){

    
    class_<PdcWrap, boost::noncopyable, boost::shared_ptr<PdcWrap> >("Pdc", no_init)
            .def(init<int>())
            .def("test", pure_virtual( &Pdc::test))
            .def_readonly("nb_var", &Pdc::nb_var)
            .def("__or__", &__or, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0,1, with_custodian_and_ward_postcall<0,2 > > >())
            .def("__and__", &__and, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0,1, with_custodian_and_ward_postcall<0,2 > > >());
            ;

    // Export CtcUnion
    class_<PdcOr, bases<Pdc>, boost::noncopyable, boost::shared_ptr<ibex::PdcOr> >("PdcOr", no_init)
            .def(init<ibex::Array<Pdc> >()[with_custodian_and_ward<1, 2>()])
            .def("test", &PdcOr::test)
            ;

    // Export PdcCompo
    class_<PdcAnd, bases<Pdc>, boost::noncopyable, boost::shared_ptr<ibex::PdcAnd> >("PdcAnd", no_init)
            .def(init<ibex::Array<Pdc> >()[with_custodian_and_ward<1, 2>()])
            .def("test", &PdcAnd::test)
            ;

    class_<PdcCleared, bases<Pdc>, boost::noncopyable, boost::shared_ptr<ibex::PdcCleared> >("PdcCleared", no_init)
            .def(init<ibex::Ctc& >()[with_custodian_and_ward<1, 2>()])
            .def("test", &PdcCleared::test)
            ;
    
    class_<PdcFwdBwd, bases<Pdc>, boost::noncopyable, boost::shared_ptr<ibex::PdcFwdBwd> >("PdcFwdBwd", no_init)
            .def(init<Function&, CmpOp>()[with_custodian_and_ward<1, 2>()])
            .def("test", &PdcFwdBwd::test)
            ;


}
