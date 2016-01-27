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

#include <pybind11/pybind11.h>
#include <pybind11/operators.h> 
namespace py = pybind11;

using py::self;
using namespace ibex;

typedef const char* cc_ptr;
typedef void (Function::*backward_1) (const IntervalVector&, IntervalVector&) const;
std::string to_string(const Function& f){
  std::stringstream ss;
  ss << f;
  return ss.str();
}

typedef void (Function::*backward_1) (const IntervalVector&, IntervalVector&) const;

void export_Function(py::module& m){
  
  py::class_<Function>(m, "Function")
    
    .def(py::init<cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def(py::init<cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr, cc_ptr>())
    .def("__repr__", &to_string)
    .def( "eval" , &Function::eval) 
    .def( "eval_vector" , &Function::eval_vector) 
    .def( "eval_matrix" , &Function::eval_matrix)
    .def( "backward", backward_1(&Function::backward))
    // .def( "backward", (*backward_1) &Function::backward)
    .def( "nb_arg", &Function::nb_arg)
    // .def( "gradient" , &Function::gradient) 
    // .def( "jacobian" , &Function::jacobian) 
    ;
}
