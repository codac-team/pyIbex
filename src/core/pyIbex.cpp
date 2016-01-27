//============================================================================
//                                  I B E X
// File        : pyIbex.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include <pybind11/pybind11.h>

namespace py = pybind11;

#include <string>
#include <sstream>


void export_Interval(py::module&m);
void export_IntervalVector(py::module& m);
void export_IntervalMatrix(py::module& m);
void export_Function(py::module& m);
void export_Bsc(py::module& m);
void export_Ctc(py::module& m);
void export_Separators(py::module& m);


PYBIND11_PLUGIN(_core)
{
  py::module m("_core", "python binding of ibex core fonctionnality");
  export_Interval(m);
  export_IntervalVector(m);
  export_Function(m);
  export_IntervalMatrix(m);
  export_Bsc(m);
  export_Ctc(m);    
  export_Separators(m);
  return m.ptr();
    
}


