//============================================================================
//                               P Y I B E X
// File        : pyIbex_BoolInterval.cpp
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

struct BoolInterval_from_python_IBOOLEAN
{
  BoolInterval_from_python_IBOOLEAN()
  {
    boost::python::converter::registry::push_back(
        &convertible,
        &construct,
        boost::python::type_id<BoolInterval>());
  }

  // Determine if obj_ptr can be converted in a QString
  static void* convertible(PyObject* obj_ptr)
  {
    if (!extract<IBOOLEAN>(obj_ptr).check()) return 0;
    // if (!PyLong_Check(obj_ptr)) return 0;
    return obj_ptr;
  }

  // Convert obj_ptr into a QString
  static void construct(
      PyObject* obj_ptr,
      boost::python::converter::rvalue_from_python_stage1_data* data)
  {
    // Extract the character data from the python string
    const IBOOLEAN value = extract<IBOOLEAN>(obj_ptr);

    // Grab pointer to memory into which to construct the new QString
    void* storage = (
        (boost::python::converter::rvalue_from_python_storage<ibex::BoolInterval>*)
        data)->storage.bytes;

    // in-place construct the new QString using the character data
    // extraced from the python object
    new (storage) BoolInterval(value);

    // Stash the memory chunk pointer for later use by boost.python
    data->convertible = storage;
  }
};



void export_BoolInterval(){

    // Export comparaison constant
    enum_<ibex::IBOOLEAN>("IBOOLEAN")
        .value( "NO",   ibex::NO)
        .value( "MAYBE",  ibex::MAYBE)
        .value( "YES",   ibex::YES)
        .value( "EMPTY",  ibex::EMPTY)
        .export_values()
        ;

    class_<BoolInterval, boost::shared_ptr<ibex::BoolInterval>  >("BoolInterval", init<>())
            .def(init<bool>())
            .def(init<ibex::IBOOLEAN>() )
            .def(init<BoolInterval>())
            .def(init<BoolInterval&>())
            .def(self & self)
            .def(self | self)
            .def(self &= self)
            .def(self |= self)
            .def(self == self)
            .def(self != self)
            .def(repr(self))
            ;

    BoolInterval_from_python_IBOOLEAN();

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
