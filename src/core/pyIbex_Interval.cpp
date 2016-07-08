//============================================================================
//                                P Y I B E X
// File        : pyIbex_Interval.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================


#include "ibex_Interval.h"
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "ibex_IntervalVector.h"
#include "pyIbex_doc_Interval.h"
namespace py = pybind11;
using py::self;
using namespace pybind11::literals;
using namespace ibex;

#include <sstream>

double getitem(Interval& X, int i){
    if(i == 0)
        return X.lb();
    else if (i == 1)
        return X.ub();
    else
        return IBEX_NAN;
}

Interval my_copy(Interval& itv){
    return Interval(itv);
}

void assignItv(Interval& self, const Interval& other){
    self = other;
}

std::string to_string(const Interval& a){
  std::stringstream ss;
  ss << a;
  return ss.str();
  // return "[" + std::to_string(a.lb()) + "," + std::to_string(a.ub()) + "]" ;
}


void export_Interval(py::module& m){
    py::class_<Interval>(m, "Interval", DOCS_INTERVAL_TYPE)
    .def(py::init<>())
    .def(py::init<double, double>(), "\tbuild Interval [lb, ub]", "lb"_a, "ub"_a)
    .def(py::init<double>(), "\tbuild singleton [val,val]", "val"_a)
    .def(py::init<Interval>(), "\tbuild a  copy of interval itv", "itv"_a)
    .def("assign", &assignItv, "\tassign the value of itv to this", "itv"_a)
    .def( self == self )
    .def( self + self )
    .def( self - self )
    .def( self * self )
    .def( self / self )
    .def( self & self )
    .def( self | self )

    // .def( self += self)
    .def("__iadd__", [](Interval& self,Interval& o ) {return self+=o;})
    .def("__isub__", [](Interval& self,Interval& o ) {return self-=o;})
    .def("__imul__", [](Interval& self,Interval& o ) {return self*=o;})
    .def("__ifloordiv__", [](Interval& self,Interval& o ) {return self/=o;})

    // .def( self *= self)
    // .def( self -= self)
    // .def( self /= self)
    .def( self &= self)
    .def( self |= self)

    .def( self + double())
    .def( self += double())
    .def( double() + self)
    .def( self - double())
    .def( double() - self)
    .def( self * double())
    .def( double() * self)
    .def( self / double())
    .def( double() / self)

    .def( "__mul__", [](Interval& x, IntervalVector& a){return x*a;})

    .def( -self )

    .def("__repr__", &to_string)

    .def("__abs__", [](const Interval& a){return ibex::abs(a);})
    .def("__pow__", [](const Interval& x, int n) { return ibex::pow(x, n);})
    .def("__pow__", [](const Interval& x, double d) { return ibex::pow(x, d);})
    .def("__pow__", [](const Interval &x, const Interval &y) { return ibex::pow(x, y);})

    .def( "lb",     &Interval::lb, "return the upper bound")
    .def( "ub",     &Interval::ub, "return the lower bound" )
    .def( "inflate",&Interval::inflate, DOCS_INTERVAL_INFLATE, py::return_value_policy::copy )
    .def( "set_empty", &Interval::set_empty, "set self to empty set" )
    .def( "mid",    &Interval::mid, DOCS_INTERVAL_MID)
    .def( "rad",    &Interval::rad, DOCS_INTERVAL_RAD )
    .def( "diam",   &Interval::diam, DOCS_INTERVAL_DIAM )
    .def( "mig",    &Interval::mig, DOCS_INTERVAL_MIG )
    .def( "mag",    &Interval::mag, DOCS_INTERVAL_MAG )
    .def( "is_subset",  &Interval::is_subset, DOCS_INTERVAL_IS_SUBSET)
    .def( "is_strict_subset",   &Interval::is_strict_subset, DOCS_INTERVAL_IS_STRICT_SUBSET)
    .def( "is_interior_subset", &Interval::is_interior_subset, DOCS_INTERVAL_IS_INTERIOR_SUBSET )
    .def( "is_strict_interior_subset",  &Interval::is_strict_interior_subset,DOCS_INTERVAL_IS_STRICT_INTERIOR_SUBSET )
    .def( "is_superset",    &Interval::is_superset, DOCS_INTERVAL_IS_SUPERSET)
    .def( "is_strict_superset", &Interval::is_strict_superset, DOCS_INTERVAL_IS_STRICT_SUPERSET)
    .def( "contains",   &Interval::contains, DOCS_INTERVAL_CONTAINS )
    .def( "interior_contains",  &Interval::interior_contains, DOCS_INTERVAL_INTERIOR_CONTAINS)
    .def( "intersects", &Interval::intersects, DOCS_INTERVAL_INTERSECTS )
    .def( "overlaps",   &Interval::overlaps, DOCS_INTERVAL_OVERLAPS )
    .def( "is_disjoint",    &Interval::is_disjoint, DOCS_INTERVAL_IS_DISJOINT )
    .def( "is_empty",   &Interval::is_empty, DOCS_INTERVAL_IS_EMPTY )
    .def( "is_degenerated",     &Interval::is_degenerated, DOCS_INTERVAL_IS_DEGENERATED )
    .def( "is_unbounded",   &Interval::is_unbounded, DOCS_INTERVAL_IS_UNBOUNDED )
    .def( "is_bisectable",  &Interval::is_bisectable,DOCS_INTERVAL_IS_BISECTABLE )
    .def( "rel_distance",   &Interval::rel_distance, DOCS_INTERVAL_REL_DISTANCE )
    .def( "complementary",  &Interval::complementary, DOCS_INTERVAL_COMPLEMENTARY )
    .def( "diff",   &Interval::diff, DOCS_INTERVAL_DIFF )

    .def( "bisect", &Interval::bisect, DOCS_INTERVAL_BISECT, py::arg("ratio")=0.5)
    .def("__getitem__", getitem, "self[0] returns the lb and self[1] return ub")
    .def( "copy", &my_copy, "return a new objec which is the copy of self")
    // .def( "__pow__", pow__)
    .def_readonly_static("PI", &Interval::PI )
    .def_readonly_static("TWO_PI", &Interval::TWO_PI )
    .def_readonly_static("HALF_PI", &Interval::HALF_PI )
    .def_readonly_static("EMPTY_SET", &Interval::EMPTY_SET )
    .def_readonly_static("ALL_REALS", &Interval::ALL_REALS )
    .def_readonly_static("ZERO", &Interval::ZERO )
    .def_readonly_static("ONE", &Interval::ONE )
    .def_readonly_static("POS_REALS", &Interval::POS_REALS )
    .def_readonly_static("NEG_REALS", &Interval::NEG_REALS )
    ;

    // External functions
    m.def( "sqr" , &ibex::sqr  );
    m.def( "sqrt", &ibex::sqrt );
    m.def( "root", &ibex::root );
    m.def( "exp" , &ibex::exp  );
    m.def( "log" , &ibex::log  );
    m.def( "cos" , &ibex::cos  );
    m.def( "sin" , &ibex::sin  );
    m.def( "tan" , &ibex::tan  );
    m.def( "acos", &ibex::acos );
    m.def( "asin", &ibex::asin );
    m.def( "atan",    &ibex::atan );
    m.def( "atan2",   &ibex::atan2 );
    m.def( "cosh",    &ibex::cosh );
    m.def( "sinh",    &ibex::sinh );
    m.def( "tanh",    &ibex::tanh );
    m.def( "acosh",   &ibex::acosh );
    m.def( "asinh",   &ibex::asinh );
    m.def( "atanh",   &ibex::atanh );
    m.def( "abs",    ( Interval (*) (const Interval&) ) &ibex::abs  );
    m.def( "max",     &ibex::max  );
    m.def( "min",     &ibex::min  );
    m.def( "sign",    &ibex::sign );
    m.def( "chi",     &ibex::chi  );
    m.def( "integer", &ibex::integer  );

    // Attention en python l'argument est passé en double par defaut et pas en int.
    // Bug possible dans pow_2
    // Interval (*pow_1)(const Interval& x, int n) = &ibex::pow;
    // Interval (*pow_2)(const Interval& x, double d)  = &ibex::pow;
    // Interval (*pow_3)(const Interval &x, const Interval &y) = &ibex::pow;
    // def( "ipow", pow_1);
    // def( "ipow", pow_2);
    // def( "ipow", pow_3);


    // pwd projection
    m.def( "bwd_add",     (bool (*)(const Interval&, Interval&, Interval&)) &ibex::bwd_add );
    m.def( "bwd_sub",     (bool (*)(const Interval&, Interval&, Interval&)) &ibex::bwd_sub );
    m.def( "bwd_mul",     (bool (*)(const Interval&, Interval&, Interval&)) &ibex::bwd_mul );
    m.def( "bwd_div",     &ibex::bwd_div );
    m.def( "bwd_sqr",     &ibex::bwd_sqr );
    m.def( "bwd_sqrt",    &ibex::bwd_sqrt );

    // sbool (*bwd_pow_1)(const Interval&, Interval& , Interval&) = &ibex::bwd_pow;
    // sbool (*bwd_pow_2)(const Interval&, int , Interval&) = &ibex::bwd_pow;
    m.def( "bwd_pow", (bool (*)(const Interval&, Interval& , Interval&))  &ibex::bwd_pow);
    m.def( "bwd_pow", (bool (*)(const Interval&, int , Interval&))        &ibex::bwd_pow);

    m.def( "bwd_root",    &ibex::bwd_root );
    m.def( "bwd_exp",     &ibex::bwd_exp );
    m.def( "bwd_log",     &ibex::bwd_log );
    m.def( "bwd_cos",     &ibex::bwd_cos );
    m.def( "bwd_sin",     &ibex::bwd_sin );
    m.def( "bwd_tan",     &ibex::bwd_tan );
    m.def( "bwd_acos",    &ibex::bwd_acos );
    m.def( "bwd_asin",    &ibex::bwd_asin );
    m.def( "bwd_atan",    &ibex::bwd_atan );
    m.def( "bwd_atan2",   &ibex::bwd_atan2 );
    m.def( "bwd_cosh",    &ibex::bwd_cosh );
    m.def( "bwd_sinh",    &ibex::bwd_sinh );
    m.def( "bwd_tanh",    &ibex::bwd_tanh );
    m.def( "bwd_acosh",   &ibex::bwd_acosh );
    m.def( "bwd_asinh",   &ibex::bwd_asinh );
    m.def( "bwd_atanh",   &ibex::bwd_atanh );
    m.def( "bwd_abs",     &ibex::bwd_abs );
    m.def( "bwd_max",     &ibex::bwd_max );
    m.def( "bwd_min",     &ibex::bwd_min );
    m.def( "bwd_sign",    &ibex::bwd_sign );
    m.def( "bwd_chi",     &ibex::bwd_chi );
    m.def( "bwd_integer", &ibex::bwd_integer );
    m.def( "bwd_imod",    &ibex::bwd_imod );

};
