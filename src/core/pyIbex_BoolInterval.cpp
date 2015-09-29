//============================================================================
//                               P Y I B E X
// File        : pyIbex_Predicate.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Sep 19, 2015
//============================================================================


#include "ibex_BoolInterval.h"
#include <boost/python.hpp>

using namespace boost;
using namespace boost::python;
using namespace ibex;



void export_BoolInterval(){

    // Export comparaison constant
    enum_<IBOOLEAN>("IBOOLEAN")
        .value( "NO",   ibex::NO)
        .value( "MAYBE",  ibex::MAYBE)
        .value( "YES",   ibex::YES)
        .value( "EMPTY",  ibex::EMPTY)
        .export_values()
        ;
    class_<BoolInterval, boost::noncopyable, boost::shared_ptr<ibex::BoolInterval>  >("BoolInterval", init<>())
            .def(init<bool>())
            .def(init<IBOOLEAN>() )
            .def(init<BoolInterval>())
            .def(init<BoolInterval&>())
            .def(self & self)
            .def(self | self)
            .def(self &= self)
            .def(self |= self)
            .def(self == self)
            .def(self != self)
            ;

    def("Inter",     &ibex::Inter);
    def("Not",   &ibex::Not);
    def("Union",     &ibex::Union);
    def("And",   &ibex::And);
    def("Or",    &ibex::Or);
    def("leq",   &ibex::leq);
    def("geq",   &ibex::geq);
    def("Restrict",  &ibex::Restrict);
    def("Xor",   &ibex::Xor);

}
