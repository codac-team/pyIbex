//============================================================================
//                               P Y I B E X
// File        : pyIbex_Predicate.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Sep 19, 2015
//============================================================================


#include "ibex_IntervalVector.h"
#include "ibex_BoolInterval.h"
#include "ibex_Pdc.h"
#include "ibex_PdcAnd.h"


#include "pyIbex_to_python_converter.h"
#include <boost/python.hpp>
using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python;



void export_Predicate(){

        // Export comparaison constant
    enum_<BoolInterval>("BoolInterval")
        .value( "NO",   ibex::NO)
        .value( "MAYBE",  ibex::MAYBE)
        .value( "YES",   ibex::YES)
        .export_values()
        ;
    def<BoolInterval( const BoolInterval&, const BoolInterval&)>("IBooleanAnd", operator&);
    def<BoolInterval( const BoolInterval&, const BoolInterval&)>("IBooleanOr", operator|);
    def<BoolInterval( BoolInterval&, const BoolInterval&)>("IBooleaniAnd", operator&=);
    def<BoolInterval( BoolInterval&, const BoolInterval&)>("IBooleaniOr", operator|=);
    def<BoolInterval( const BoolInterval&)>("IBooleanNeg", operator!);

    


}
