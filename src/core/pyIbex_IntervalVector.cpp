//============================================================================
//                                P Y I B E X
// File        : pyIbex_IntervalVector.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================


#include "ibex_IntervalVector.h"
#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include "pyIbex_to_python_converter.h"

using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python;



boost::shared_ptr<ibex::IntervalVector> CreateWithList(const py::list & lst)
{
  // construct with a list here
  extract<double> get_double(lst[0]);
  if (get_double.check()){
      std::vector<double> v = to_std_vector<double>(lst);
      return shared_ptr<ibex::IntervalVector>(new ibex::IntervalVector( Vector(v.size(), &v[0])));
  } else {
        double (*tmp)[2] = new double[len(lst)][2];
        for(int i = 0; i < len(lst); i++){
            extract<list> get_list(lst[i]);
            if (get_list.check()){
                assert(len(lst[i]) == 2);
                for(int j = 0; j < 2;j++){
                    tmp[i][j] = extract<double>(lst[i][j]);
                }
            }
        }
        boost::shared_ptr<ibex::IntervalVector> ptr =  shared_ptr<ibex::IntervalVector>(new ibex::IntervalVector( len(lst), tmp));
        delete[] tmp;
        return ptr;
    }
}
boost::shared_ptr<ibex::IntervalVector> CreateWithIntAndList(int ndim, const py::list & lst){
    // construct with a list here
    extract<double> get_double(lst[0]);
    if (get_double.check()){
        std::vector<double> v = to_std_vector<double>(lst);
        return shared_ptr<ibex::IntervalVector>(new ibex::IntervalVector(ndim, Interval(v[0], v[1])));
    }
}


boost::shared_ptr<ibex::IntervalVector> CreateWithTuple(const py::tuple & tup)
{
    extract<double> get_double(tup[0]);
    if(get_double.check()){
        // construct with a list here
        std::vector<double> v = to_std_vector<double>(tup);
        return shared_ptr<ibex::IntervalVector>(new ibex::IntervalVector( Vector(v.size(), &v[0])));  

    } else {
        extract<Interval> get_interval(tup[0]);
        if(get_interval.check()){
        // construct with a list here
            std::vector<Interval> v = to_std_vector<Interval>(tup);
            shared_ptr<ibex::IntervalVector> itv(new ibex::IntervalVector( v.size()));
            for(int i = 0 ; i < v.size(); i++){
                (*itv)[i] = v[i];
            }    
            return itv;
        } else {

        }
    }
}


Interval& getitem(IntervalVector& X, int i){
      return X[i];
}

void setitem(IntervalVector& X, int i, Interval& itv){
      X[i] = itv;
}

py::list complementary_wrapper(IntervalVector& X){
      IntervalVector* result;
      int n = X.complementary(result);
      py::list l;
      for(int i = 0 ; i < n ; i++){
            l.append(result[i]);
      }
      delete[] result;
    return l;
}

py::list diff_wrapper(IntervalVector& X, IntervalVector& Y){
      IntervalVector* result;
      int n = X.diff(Y, result);
      py::list l;
      for(int i = 0 ; i < n ; i++){
                l.append(result[i]);
      }
      delete[] result;
    return l;
}


void assignItv(IntervalVector& self, const IntervalVector& other){
    self = other;
}

IntervalVector my_copy(IntervalVector& itv){
    return IntervalVector(itv);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(bisect_overloads, bisect, 1,2);

void export_IntervalVector(){

    

    class_<IntervalVector, boost::shared_ptr<ibex::IntervalVector>  >("IntervalVector", init<int>())
            .def(init<int,const Interval>())
            .def(init<const IntervalVector&>() )
            .def("__init__", make_constructor(&CreateWithList))
            .def("__init__", make_constructor(&CreateWithTuple))
            .def("__init__", make_constructor(&CreateWithIntAndList))

            // .def("__init__", make_constructor(&CreateWithPyArrayObject))
            // .def( "__getitem__", &IntervalVector::getitem, return_value_policy<copy_const_reference>()  )
            // .def( "__setitem__", &IntervalVector::setitem)
            .def<Interval&(IntervalVector&, int)> ("__getitem__", getitem, return_internal_reference<>()) //return_value_policy<copy_non_const_reference>())
            .def<void(IntervalVector&, int, Interval&)> ("__setitem__", setitem)

            .def("assign", &assignItv)
            .def( self == self )
            .def( self != self )
            .def( self + self )
            .def( self - self )
            .def( self * self)
            .def( self & self )
            .def( self | self )
            .def( -self )

            .def( self += self)
            .def( self += double())
            .def( self += other<Vector>()) 

            .def( self -= self)
            .def( self -= double())
            .def( self -= other<Vector>()) 

            .def( self *= double())
            .def( self *= other<Interval>()) 
            .def( self * other<Vector>()) 

            .def( self &= self)
            .def( self |= self)



            
            .def (self + other<Vector>())
            .def ( other<Vector>() + self )

            .def (self - other<Vector>())
            .def ( other<Vector>() - self )

            .def (self * other<Vector>())
            .def ( other<Vector>() * self )
            .def (double() * self)
            // .def (other<const Interval&>() * other<Vector>())
            .def (other<Interval>() * self)



            
            // .def( self - double())
            // .def( double() - self)
            // .def( self * other<double>())
            // .def( other<double>() * self)
            // .def( self / double())
            // .def( double() / self)

            // .def( -self )
            // .def( self + double())

            .def(repr(self))
            .def("copy", &my_copy)
            .def("__len__", &IntervalVector::size )
            .def("size", &IntervalVector::size )
            .def( "empty", &IntervalVector::empty ).staticmethod("empty")
            .def( "set_empty", &IntervalVector::set_empty)
            .def( "clear", &IntervalVector::clear)
            .def( "init", &IntervalVector::init)
            .def( "inflate", &IntervalVector::inflate, return_internal_reference<>())
            .def( "resize", &IntervalVector::resize)
            .def( "subvector", &IntervalVector::subvector, return_value_policy<return_by_value>())
            .def( "put", &IntervalVector::put)
            .def( "size" , &IntervalVector::size )
            .def( "lb" , &IntervalVector::lb )
            .def( "ub" , &IntervalVector::ub )
            .def( "mid" , &IntervalVector::mid )
            .def( "mig" , &IntervalVector::mig )
            .def( "mag" , &IntervalVector::mag )
            .def( "is_empty" , &IntervalVector::is_empty )
            .def( "is_flat" , &IntervalVector::is_flat )
            .def( "is_unbounded" , &IntervalVector::is_unbounded )
            .def( "is_subset" , &IntervalVector::is_subset )
            .def( "is_strict_subset" , &IntervalVector::is_strict_subset )
            .def( "is_interior_subset" , &IntervalVector::is_interior_subset )
            .def( "is_strict_interior_subset" , &IntervalVector::is_strict_interior_subset )
            .def( "is_superset" , &IntervalVector::is_superset )
            .def( "is_strict_superset" , &IntervalVector::is_strict_superset )
            .def( "contains" , &IntervalVector::contains )
            .def( "interior_contains" , &IntervalVector::interior_contains )
            .def( "intersects" , &IntervalVector::intersects )
            .def( "overlaps" , &IntervalVector::overlaps )
            .def( "is_disjoint" , &IntervalVector::is_disjoint )
            .def( "is_zero" , &IntervalVector::is_zero )
            .def( "is_bisectable" , &IntervalVector::is_bisectable )
            .def( "rad",    &IntervalVector::rad ) 
            .def( "diam",   &IntervalVector::diam ) 
            .def( "sort_indices",   &IntervalVector::sort_indices )
            .def( "max_diam",   &IntervalVector::max_diam, "return maximal diameter of (*this)" )
            .def( "min_diam",   &IntervalVector::min_diam, "return minimal diameter of (*this)") 
            .def( "volume",     &IntervalVector::volume ) 
            .def( "perimeter",  &IntervalVector::perimeter )
            .def( "rel_distance",   &IntervalVector::rel_distance )
            .def( "diff",   &diff_wrapper )
            .def( "complementary",  &complementary_wrapper )
            .def( "bisect",     &IntervalVector::bisect, bisect_overloads() )
            ;

            def<IntervalVector(const IntervalVector& )>( "abs", &ibex::abs);
            def<bool (const IntervalVector&, IntervalVector&, IntervalVector&)>( "bwd_add", &ibex::bwd_add);
            def<bool (const IntervalVector&, IntervalVector&, IntervalVector&)>( "bwd_sub", &ibex::bwd_sub);
            def( "cart_prod", &ibex::cart_prod);

            // hadamard_product overloading
            // IntervalVector (*hadamard_product_1) (const IntervalVector& , const Vector& ) = &ibex::hadamard_product;
            // IntervalVector (*hadamard_product_2) (const Vector& , const IntervalVector& ) = &ibex::hadamard_product;
            // IntervalVector (*hadamard_product_3) (const IntervalVector& , const IntervalVector& ) = &ibex::hadamard_product;
            // def( "hadamard_product", hadamard_product_1);
            // def( "hadamard_product", hadamard_product_2);
            // def( "hadamard_product", hadamard_product_3);

            // bwd_mul overloading
            bool (*bwd_mul_1) (const IntervalVector&, Interval&, IntervalVector&) = &ibex::bwd_mul;
            bool (*bwd_mul_2) (const Interval&, IntervalVector&, IntervalVector&) = &ibex::bwd_mul;
            def( "bwd_mul", bwd_mul_1);
            def( "bwd_mul", bwd_mul_2);
            
};


