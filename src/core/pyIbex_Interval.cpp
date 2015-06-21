//============================================================================
//                                P Y I B E X
// File        : pyIbex_Interval.cpp
// Author      : Benoit Desrochers
// Copyright   : ENSTA Bretagne
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================


#include "ibex_Interval.h"
#include <boost/python.hpp>
using namespace boost::python;
using namespace ibex;

double getitem(Interval& X, int i){
    if(i == 0)
        return X.lb();
    else if (i == 1)
        return X.ub();
    else 
        return IBEX_NAN;
}

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(bisect_overloads, bisect, 0,1)

    void export_Interval(){
        class_<Interval, boost::shared_ptr<ibex::Interval> >("Interval")
        .def(init<double, double>())
        .def(init<double>())
        .def(init<Interval>())
        .def( self == self )
        .def( self + self )
        .def( self - self ) 
        .def( self * self )
        .def( self / self )
        .def( self & self )
        .def( self | self )

        .def( self += self)
        .def( self -= self)
        .def( self *= self)
        .def( self /= self)
        .def( self &= self)
        .def( self |= self)

        .def( self + double())
        .def( other<double>() + self)
        .def( self - double())
        .def( double() - self)
        .def( self * other<double>())
        .def( other<double>() * self)
        .def( self / double())
        .def( double() / self)

        .def( -self )

        .def(repr(self))
        .def(abs(self))
        // .def(pow(self,self))
        // .def(pow(self,int()))
        // .def(pow(self,float()))

        .def( "lb",     &Interval::lb, "return the upper bound")
        .def( "ub",     &Interval::ub, "return the lower bound" )
        .def( "inflate",&Interval::inflate, return_internal_reference<>() )
        .def( "set_empty", &Interval::set_empty )
        .def( "mid",    &Interval::mid )
        .def( "rad",    &Interval::rad )
        .def( "diam",   &Interval::diam )
        .def( "mig",    &Interval::mig )
        .def( "mag",    &Interval::mag )
        .def( "is_subset",  &Interval::is_subset )
        .def( "is_strict_subset",   &Interval::is_strict_subset )
        .def( "is_interior_subset", &Interval::is_interior_subset )
        .def( "is_strict_interior_subset",  &Interval::is_strict_interior_subset )
        .def( "is_superset",    &Interval::is_superset )
        .def( "is_strict_superset", &Interval::is_strict_superset )
        .def( "contains",   &Interval::contains )
        .def( "interior_contains",  &Interval::interior_contains )
        .def( "intersects", &Interval::intersects )
        .def( "overlaps",   &Interval::overlaps )
        .def( "is_disjoint",    &Interval::is_disjoint )
        .def( "is_empty",   &Interval::is_empty )
        .def( "is_degenerated",     &Interval::is_degenerated )
        .def( "is_unbounded",   &Interval::is_unbounded )
        .def( "is_bisectable",  &Interval::is_bisectable )
        .def( "rel_distance",   &Interval::rel_distance )
        .def( "complementary",  &Interval::complementary )
        .def( "diff",   &Interval::diff )
        .def_readonly("PI", &Interval::PI)
        .def_readonly("TWO_PI", &Interval::TWO_PI)
        .def_readonly("HALF_PI", &Interval::HALF_PI)
        .def_readonly("EMPTY_SET", &Interval::EMPTY_SET)
        .def_readonly("ALL_REALS", &Interval::ALL_REALS)
        .def_readonly("ZERO", &Interval::ZERO)
        .def_readonly("ONE", &Interval::ONE)
        .def_readonly("POS_REALS", &Interval::POS_REALS)
        .def_readonly("NEG_REALS", &Interval::NEG_REALS)    
        .def( "bisect", &Interval::bisect, bisect_overloads())
        .def<double(Interval&, int)> ("__getitem__", getitem)
            
        ;

        // External functions
        def( "sqr" , &ibex::sqr  );
        def( "sqrt", &ibex::sqrt );
        def( "root", &ibex::root );
        def( "exp" , &ibex::exp  );
        def( "log" , &ibex::log  );
        def( "cos" , &ibex::cos  );
        def( "sin" , &ibex::sin  );
        def( "tan" , &ibex::tan  );
        def( "acos", &ibex::acos );
        def( "asin", &ibex::asin );
        def( "atan",    &ibex::atan );  
        def( "atan2",   &ibex::atan2    );  
        def( "cosh",    &ibex::cosh );  
        def( "sinh",    &ibex::sinh );  
        def( "tanh",    &ibex::tanh );  
        def( "acosh",   &ibex::acosh);  
        def( "asinh",   &ibex::asinh);  
        def( "atanh",   &ibex::atanh);  
        def<Interval(const Interval&)>( "abs",     &ibex::abs  );  
        def( "max",     &ibex::max  );  
        def( "min",     &ibex::min  );  
        def( "sign",    &ibex::sign );   
        def( "chi",     &ibex::chi  );  
        def( "integer", &ibex::integer  );

        // Attention en python l'argument est passé en double par defaut et pas en int.
        // Bug possible dans pow_2
        Interval (*pow_1)(const Interval& x, int n) = &ibex::pow;
        // Interval (*pow_2)(const Interval& x, double d)  = &ibex::pow;
        // Interval (*pow_3)(const Interval &x, const Interval &y) = &ibex::pow;
        def( "ipow", pow_1);
        // def( "ipow", pow_2);
        // def( "ipow", pow_3);


        // pwd projection
        def( "bwd_add",     &ibex::bwd_add );
        def( "bwd_sub",     &ibex::bwd_sub );
        def( "bwd_mul",     &ibex::bwd_mul );
        def( "bwd_div",     &ibex::bwd_div );
        def( "bwd_sqr",     &ibex::bwd_sqr );
        def( "bwd_sqrt",    &ibex::bwd_sqrt );
        
        bool (*bwd_pow_1)(const Interval&, Interval& , Interval&) = &ibex::bwd_pow;
        bool (*bwd_pow_2)(const Interval&, int , Interval&) = &ibex::bwd_pow;
        def( "bwd_pow",     bwd_pow_1 );
        def( "bwd_pow",     bwd_pow_2 );
        
        def( "bwd_root",    &ibex::bwd_root );
        def( "bwd_exp",     &ibex::bwd_exp );
        def( "bwd_log",     &ibex::bwd_log );
        def( "bwd_cos",     &ibex::bwd_cos );
        def( "bwd_sin",     &ibex::bwd_sin );
        def( "bwd_tan",     &ibex::bwd_tan );
        def( "bwd_acos",    &ibex::bwd_acos );
        def( "bwd_asin",    &ibex::bwd_asin );
        def( "bwd_atan",    &ibex::bwd_atan );
        def( "bwd_atan2",   &ibex::bwd_atan2 );
        def( "bwd_cosh",    &ibex::bwd_cosh );
        def( "bwd_sinh",    &ibex::bwd_sinh );
        def( "bwd_tanh",    &ibex::bwd_tanh );
        def( "bwd_acosh",   &ibex::bwd_acosh );
        def( "bwd_asinh",   &ibex::bwd_asinh );
        def( "bwd_atanh",   &ibex::bwd_atanh );
        def( "bwd_abs",     &ibex::bwd_abs );
        def( "bwd_max",     &ibex::bwd_max );
        def( "bwd_min",     &ibex::bwd_min );
        def( "bwd_sign",    &ibex::bwd_sign );
        def( "bwd_chi",     &ibex::bwd_chi );
        def( "bwd_integer", &ibex::bwd_integer );
        def( "bwd_imod",    &ibex::bwd_imod );
        
    };


