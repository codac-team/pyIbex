//============================================================================
//                               P Y I B E X
// File        : pyIbex_Function.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
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
typedef void (Function::*backward_1) (const IntervalVector&, IntervalVector&) const;

void export_Function(){
    class_<Function, boost::noncopyable, boost::shared_ptr<ibex::Function> >("Function")
    .def(init<cc_ptr, optional< cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr> >())
    .def(repr(self))
    .def( "eval" , &Function::eval) 
    .def( "eval_vector" , &Function::eval_vector) 
    .def( "eval_matrix" , &Function::eval_matrix)

    .def( "backward", backward_1(&Function::backward))
    .def( "nb_arg", &Function::nb_arg)
    // .def( "gradient" , &Function::gradient) 
    // .def( "jacobian" , &Function::jacobian) 
    ;
}
