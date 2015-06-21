//============================================================================
//                                P Y I B E X
// File        : pyIbex_SIVIA.cpp
// Author      : Benoit Desrochers
// Copyright   : ENSTA Bretagne
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================


// C++ SIVIA Implementation

#include "SIVIA.h"
#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace ibex;

namespace python = boost::python;
void export_algorithms(){
    
    def("SIVIA", &ibex::SiviaC, (python::arg("X0"), python::arg("ctc"), python::arg("eps")=0.1,
                                python::arg("figureName")=string("default"),
                                python::arg("fillOut")=string("[b]"),
                                python::arg("fillBorder")=string("[y]"))
        );
    def("SIVIA", &ibex::SiviaS, (python::arg("X0"), python::arg("Sep"), python::arg("eps")=0.1,
                                    python::arg("figureName")=string("default"),
                                    python::arg("fillOut")=string("[b]"),
                                    python::arg("fillIn")=string("[r]"),
                                    python::arg("fillBorder")=string("[y]"))
        );
}

