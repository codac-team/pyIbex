//============================================================================
//                               P Y I B E X
// File        : pyIbex_Function.cpp
// Author      : Benoit Desrochers
// Copyright   : ENSTA Bretagne
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include "ibex_Function.h"
#include "ibex_SyntaxError.h"

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::python;
using namespace ibex;

typedef const char* cc_ptr;
    void export_Function(){
        class_<Function, boost::noncopyable, boost::shared_ptr<ibex::Function> >("Function")
        .def(init<cc_ptr, optional< cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr> >())
        .def(repr(self))
        .def( "eval" , &Function::eval) 
        .def( "eval_vector" , &Function::eval_vector) 
        .def( "eval_matrix" , &Function::eval_matrix) 
        // .def( "gradient" , &Function::gradient) 
        // .def( "jacobian" , &Function::jacobian) 
        ;
}
