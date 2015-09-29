//============================================================================
//                               P Y I B E X
// File        : pyIbex_Set.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Sep 19, 2015
//============================================================================

#include "paving.h"
#include <boost/shared_ptr.hpp> 
#include <boost/python.hpp>

using namespace boost;
using namespace boost::python;

namespace ibex {
    typedef enum {op_Inter, op_Not, op_Union, op_And, op_Or, op_leq, op_geq, op_Restrict, op_Xor} BoolOp;
};

using namespace ibex;

ibex::BOOLEAN_OP switch_BoolOpBinaire(BoolOp type){
    switch (type){
        case op_Inter:
            return &ibex::Inter;
        break;

        case op_Union:
            return &ibex::Union;
        break;
        
        case op_And:
            return &ibex::And;
        break;
        
        case op_Or:
            return &ibex::Or;
        break;
        
        case op_leq:
            return &ibex::leq;
        break;
        
        case op_geq:
            return &ibex::geq;
        break;
        
        case op_Restrict:
            return &ibex::Restrict;
        break;
        
        case op_Xor:
            return &ibex::Xor;
        break;
    }
}

void  op_binaire_wrapper(Paving& Z,Paving& X,Paving& Y, BoolOp bool_op){
    BOOLEAN_OP bin_op = switch_BoolOpBinaire(bool_op);
    ibex::op_binaire(Z, X, Y, bin_op);
}
void  op_unaire_wrapper(Paving& Z,Paving& X,BoolOp bool_op){
    if(bool_op == op_Not){
        ibex::op_unaire(Z, X, ibex::Not);
    }
}

Paving& Sivia_wrapper(Paving& X, Pdc& pdc,BoolOp bool_op,double eps){
    BOOLEAN_OP bin_op = switch_BoolOpBinaire(bool_op);
    return X.Sivia(pdc, bin_op, eps);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(visit_overload, visit, 1,2)
void export_Paving(){
    // Export comparaison constant
    enum_<BoolOp>("BoolOp")
    .value("op_Inter", ibex::op_Inter)
    .value("op_Not", ibex::op_Not)
    .value("op_Union", ibex::op_Union)
    .value("op_And", ibex::op_And)
    .value("op_Or", ibex::op_Or)
    .value("op_leq", ibex::op_leq)
    .value("op_geq", ibex::op_geq)
    .value("op_Restrict", ibex::op_Restrict)
    .value("op_Xor", ibex::op_Xor)
    .export_values()
    ;

    class_<Paving, boost::noncopyable, boost::shared_ptr<Paving>  >("Paving", no_init)
        .def(init<int>())
        .def(init<IntervalVector&, BoolInterval> ())
        .def(init<IntervalVector&, Pdc&, double>())
        .def( "Expand", &Paving::Expand, return_value_policy<copy_non_const_reference>())
        .def( "Remove_sons", &Paving::Remove_sons, return_value_policy<copy_non_const_reference>())
        .def( "Clean", &Paving::Clean, return_value_policy<copy_non_const_reference>())
        .def( "Reunite", &Paving::Reunite, return_value_policy<copy_non_const_reference>())
        .def( "contains", &Paving::contains)
        .def( "Sivia", &Sivia_wrapper, return_value_policy<copy_non_const_reference>())
        .def( "Clear", &Paving::Clear, return_value_policy<copy_non_const_reference>())
        .def( "visit", &Paving::visit, visit_overload())
        .def( "change", &Paving::change)
        .def( repr(self))
        ;
    def( "op_binaire", &op_binaire_wrapper);
    def( "op_unaire", &op_unaire_wrapper);
    def( "Set_Contractor_Inter", &ibex::Set_Contractor_Inter);
    def( "Set_Contractor_Subset", &ibex::Set_Contractor_Subset);
    def( "Set_Contractor_Disjoint", &ibex::Set_Contractor_Disjoint);
    def( "Set_Contractor_Overlap", &ibex::Set_Contractor_Overlap);
    def( "Overlap", &ibex::Overlap);
    def( "Sqcap", &ibex::Sqcap);
    // def( "Contract_distance_gt_Paving", &ibex::Contract_distance_gt_Paving);
    // def( "Complement", &ibex::Complement);
}
