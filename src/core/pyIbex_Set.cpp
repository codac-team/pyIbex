//============================================================================
//                               P Y I B E X
// File        : pyIbex_Set.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Sep 19, 2015
//============================================================================

#include "pyIbex_to_python_converter.h"

#include "ibex_IntervalVector.h"


#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

#include "ibex_Set.h"
#include "ibex_SetVisitor.h"
#include "ibex_SetInterval.h"


using namespace boost;
using namespace ibex;
using namespace boost::python;
namespace py = boost::python;


namespace  ibex {


struct SetVisitorWrap : SetVisitor, wrapper<SetVisitor> {
    SetVisitorWrap() : SetVisitor() {};
    void visit_leaf(const IntervalVector& box, BoolInterval status){
        this->get_override("visit_leaf")(box, status);
    }
};

}


void export_Set(){
    class_<Set, boost::noncopyable, boost::shared_ptr<ibex::Set>  >("Set", no_init)
        .def(init<int, optional<BoolInterval> >())
        .def(init<IntervalVector&, optional<BoolInterval> >())
        .def(init<const char*>())
        .def(init<Function&, CmpOp, double>())
        .def(self &= self)
        .def(self |= self)
        .def( "is_empty", &Set::is_empty)
        .def( "save",     &Set::save)
        .def( "visit",    &Set::visit)
        .def( "dist",     &Set::dist)
        .def( "is_superset", &Set::is_superset)
        ;
    //std::vector<std::vector<SetLeaf*> > connected_components();

    class_<SetVisitorWrap, boost::noncopyable, boost::shared_ptr<SetVisitorWrap> >("SetVisitor", no_init)
        .def(init<>())
        .def("visit_leaf", pure_virtual( &SetVisitor::visit_leaf))
        .def("visit_node", &SetVisitor::visit_node)
        ;

    class_<SetInterval, boost::noncopyable, boost::shared_ptr<ibex::SetInterval> > ("SetInterval", no_init)
        .def(init<const IntervalVector&, BoolInterval>())
        .def(init<const char*>())
        .def("is_empty",    &SetInterval::is_empty)
        .def("save",        &SetInterval::save)
        .def("visit",       &SetInterval::visit)
        ;
            
}
