//============================================================================
//                                  I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================


#include "ibex_IntervalVector.h"
#include "ibex_Sep.h"
#include "ibex_SepUnion.h"
#include "ibex_SepInter.h"
#include "ibex_SepNot.h"
#include "ibex_SepCtcPair.h"
#include "ibex_SepFwdBwd.h"
#include "ibex_SepBoundaryCtc.h"

#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include "pyIbex_to_python_converter.h"

using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python;


typedef boost::shared_ptr<ibex::SepUnion> SepUnionPtr;
typedef boost::shared_ptr<ibex::SepInter> SepInterPtr;


struct SepWrap : Sep, wrapper<Sep> {
    void separate(IntervalVector& xin, IntervalVector& xout){
        this->get_override("separate")(xin, xout);
    }
};
typedef boost::shared_ptr<Sep> SepPtr;

SepUnionPtr __or(Sep& c1, Sep& c2){
    Array<Sep> lst = Array<Sep>(c1,c2);
    return SepUnionPtr(new SepUnion(lst));
}

SepInterPtr __and(Sep& c1, Sep& c2){
    Array<Sep> lst = Array<Sep>(c1,c2);
    return SepInterPtr(new SepInter(lst));
}

template<typename SepType>
boost::shared_ptr<SepType> ctcFromList(const py::list & lst)
{
    // construct with a list here
    ibex::Array<Sep> list(len(lst));
    for(uint i = 0; i < len(lst); i++){
        extract<Sep> get_Sep(lst[i]);
        if (get_Sep.check()){
            Sep* C = extract<Sep*>(lst[i]);
            list.set_ref(i, *C);
        } else {
            std::cout << "Extraction Error \n";
            return boost::shared_ptr<SepType>();
        }
    }
    return boost::shared_ptr<SepType>(new SepType(list));
}


void export_Separators(){

    typedef void (Sep::*separate) (IntervalVector&, IntervalVector&);
    class_<SepWrap, boost::noncopyable>("Sep", no_init)
        .def("separate", pure_virtual( separate(&Sep::separate)))
        .def( "__or__", &__or)
    	.def( "__and__", &__and)
    ;

    class_<SepUnion, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepUnion> >("SepUnion", no_init)
    	.def("__init__", make_constructor(ctcFromList<SepUnion>), "SepUnion from list of separators\n Usage : SepUnion([s1, s2, ...])")
    	.def("separate", &SepUnion::separate)
		;

    class_<SepInter, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepInter> >
        ("SepInter", no_init)
    	.def("__init__", make_constructor(ctcFromList<SepInter>), "SepInter from list of separators\n Usage : SepInter([s1, s2, ...])")
    	.def("separate", &SepInter::separate)
    	;

    class_<SepCtcPair, bases<Sep>,  boost::noncopyable, boost::shared_ptr<ibex::SepCtcPair> >
            ("SepCtcPair", init< Ctc&, Ctc& >())
            .def("separate", &SepCtcPair::separate)
            ;


    class_<SepFwdBwd, bases<Sep>,  boost::noncopyable, boost::shared_ptr<ibex::SepFwdBwd> >
            ("SepFwdBwd", init< Function&, CmpOp >())
            .def(init<Function&, Interval& >())
            .def(init<Function&, IntervalVector& > ())
//            .def(init<Function&, IntervalMatrix& > ())
            .def("separate", &SepFwdBwd::separate)
            ;

    class_<SepNot, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepNot> >("SepNot", no_init)
        .def(init<Sep&>())
    	.def("separate", &SepNot::separate);

}
