//============================================================================
//                                  I B E X
// File        : pyIbex.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include <string>
#include <sstream>
#include <boost/python.hpp>
using namespace boost::python;

#include "pyIbex_to_python_converter.h"
#include "ibex_IntervalVector.h"
#include "ibex_UnknownFileException.h"
#include "ibex_SyntaxError.h"
// #include "ibex_EmptyBoxException.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_DimException.h"
#include "ibex_Exception.h"
#include "ibex_Ctc.h"
#include "ibex_Sep.h"
void export_Interval();
void export_IntervalVector();
void export_Bsc();
void export_Ctc();
void export_Function();
void export_Separators();
void export_Predicate();
void export_Set();
int export_PixelMap();
//void export_algorithms();


using namespace ibex;


// void translateException_IbexException(ibex::Exception const& e){
// 	// Use the Python 'C' API to set up an exception object
//     PyErr_SetString(PyExc_RuntimeError, e.what());
// }

void translateException_SyntaxError( const SyntaxError& x) {
	std::ostringstream ss;
    ss << x;
	PyErr_SetString(PyExc_UserWarning, ss.str().c_str());
};

void translateException_UnknownFileException( const UnknownFileException& x) {
	PyErr_SetString(PyExc_UserWarning, x.filename);
};

void translateException_DimException( const DimException& x) {
	// std::ostringstream ss;
 //    ss << x.message();
	// std::string s = x.message();
	PyErr_SetString(PyExc_UserWarning, "DimException [need to modify DimException.h to get the message] ");
};

void translateException_NoBisectableVariableException( const NoBisectableVariableException& x) {
	PyErr_SetString(PyExc_UserWarning, "NoBisectableVariableException raised");
};

// void translateException_EmptyBoxException( const EmptyBoxException& x) {
// 	PyErr_SetString(PyExc_UserWarning, "EmptyBoxException raised");
// };

void translateException_InvalidIntervalVectorOp( const InvalidIntervalVectorOp& x){
	PyErr_SetString(PyExc_UserWarning, "InvalidIntervalVectorOp [need to modify DimException.h to get the message] ");	
}


BOOST_PYTHON_MODULE(pyIbex)
{

	// register converter
	std_pair_to_python_converter<Interval, Interval>();
	std_pair_to_python_converter<IntervalVector, IntervalVector>();
    boost::python::to_python_converter<ibex::Vector, Vector_to_list>();
    Vector_from_python_list();
    Array_from_python<Ctc>();
    Array_from_python<Sep>();

    
    register_exception_translator<SyntaxError>(translateException_SyntaxError);
	register_exception_translator<UnknownFileException>(translateException_UnknownFileException);
	register_exception_translator<DimException>(translateException_DimException);
	register_exception_translator<NoBisectableVariableException>(translateException_NoBisectableVariableException);
	// register_exception_translator<EmptyBoxException>(translateException_EmptyBoxException);
	register_exception_translator<InvalidIntervalVectorOp>(translateException_InvalidIntervalVectorOp);

    export_Interval();
    export_IntervalVector();    
    export_Bsc();
    export_Ctc();
    export_Function();
    export_Separators();
    export_Predicate();
    export_Set();
    export_PixelMap();
    //export_algorithms();
    
}


