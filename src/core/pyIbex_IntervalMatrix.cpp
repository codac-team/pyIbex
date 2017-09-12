//============================================================================
//                                P Y I B E X
// File        : pyIbex_IntervalMatrix.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================


#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pyIbex_type_caster.h>

#include "ibex_IntervalMatrix.h"
#include <stdexcept>
#include <sstream>

namespace py = pybind11;
using py::self;
using py::class_;
using py::init;

using ibex::Interval;
using ibex::IntervalVector;
using ibex::IntervalMatrix;
using ibex::Vector;





void CreateWithList(IntervalMatrix &instance, int nb_rows, int nb_cols,  std::vector< Interval > & lst){

  size_t size = lst.size();
  if (size != nb_cols*nb_rows){
    throw std::invalid_argument("Size of the input list is 0");
  }
  double (*tmp)[2] = new double[size][2];
  size_t index = 0;
  for (auto & v : lst){
    tmp[index][0] = v.lb();
    tmp[index][1] = v.ub();
    index++;
  }
  new(&instance) IntervalMatrix(nb_rows, nb_cols, tmp);
  delete[] tmp;
}




IntervalVector& getitem(IntervalMatrix& X, int i){
      return X[i];
}

void setitem(IntervalMatrix& X, int i, IntervalVector   & itv){
      X[i] = itv;
}

void assignItv(IntervalMatrix& self, const IntervalMatrix& other){
    self = other;
}

IntervalMatrix my_copy(IntervalMatrix& itv){
    return IntervalMatrix(itv);
}

std::string to_string(const IntervalMatrix& m){
  std::stringstream ss;
  ss << m;
  return ss.str();
}

// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(bisect_overloads, bisect, 1,2);

void export_IntervalMatrix(py::module& m){

    class_<IntervalMatrix>(m, "IntervalMatrix")
        .def(init<int,int>())
        .def(init<int,int,const Interval>())
        .def(init<const IntervalMatrix&>() )
        .def("__init__", &CreateWithList)
        .def<IntervalVector&(IntervalMatrix&, int)> ("__getitem__", getitem, py::return_value_policy::reference_internal)
        .def<void(IntervalMatrix&, int, IntervalVector&)> ("__setitem__", setitem)

        .def("assign", &assignItv)
        .def( self == self )
        .def( self != self )
        .def( self + self )
        .def( self - self )
        .def( self * self)
        .def( self &= self )

        .def( -self )

        .def( self += self)
        //TODO .def( self += other<Matrix>())

        .def( self -= self)
        // TODO .def( self -= other<Matrix>())

        .def( self *= self)
        .def( self *= double())
        .def( self *= Interval())
        // TODO .def( self * other<Matrix>())
        // TODO .def( other<Matrix>() * self)
        .def( "__mul__", [](IntervalMatrix& m, const IntervalVector& x) {return m*x;})
        .def( double() * self)
        .def( Interval() * self)

        .def("shape", [] (IntervalMatrix& o) { return std::make_tuple(o.nb_rows(), o.nb_cols());} )
        .def("__repr__", &to_string)
        ;
};
