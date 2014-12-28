#include <string>
#include <sstream>

#include "ibex_Function.h"
#include "ibex_SyntaxError.h"

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
using namespace boost::python;
using namespace ibex;

boost::shared_ptr<ibex::Function>  Function_1(const char* x1, const char* y){
	return boost::shared_ptr<ibex::Function>(new Function(x1, y));
}
boost::shared_ptr<ibex::Function>  Function_2(const char* x1, const char* x2, const char* y){

    try{
        boost::shared_ptr<ibex::Function> f(new Function( x1, x2, y));
	    return f;
    } catch (ibex::SyntaxError& e){
        throw e;
        return boost::shared_ptr<ibex::Function>();
    }
}
boost::shared_ptr<ibex::Function>  Function_3(const char* x1, const char* x2, const char* x3, const char* y){
	return boost::shared_ptr<ibex::Function>(new Function( x1, x2, x3, y));
}
boost::shared_ptr<ibex::Function>  Function_4(const char* x1, const char* x2, const char* x3, const char* x4, const char* y){
	return boost::shared_ptr<ibex::Function>(new Function( x1, x2, x3, x4, y));
}
boost::shared_ptr<ibex::Function>  Function_5(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* y){
	return boost::shared_ptr<ibex::Function>(new Function( x1, x2, x3, x4, x5, y));
}
boost::shared_ptr<ibex::Function>  Function_6(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* y){
	return boost::shared_ptr<ibex::Function>(new Function( x1, x2, x3, x4, x5, x6, y));
}
boost::shared_ptr<ibex::Function>  Function_7(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* y){
	return boost::shared_ptr<ibex::Function>(new Function( x1, x2, x3, x4, x5, x6, x7, y));
}
boost::shared_ptr<ibex::Function>  Function_8(const char* x1, const char* x2, const char* x3, const char* x4, const char* x5, const char* x6, const char* x7, const char* x8, const char* y){
	return boost::shared_ptr<ibex::Function>(new Function( x1, x2, x3, x4, x5, x6, x7, x8, y));
}
boost::shared_ptr<ibex::Function>  Function_9(const char* filename){
	return boost::shared_ptr<ibex::Function>(new Function( filename));
}

	
    void export_Function(){
        class_<Function, boost::shared_ptr<ibex::Function> >("Function")
        .def("__init__", make_constructor(&Function_1))
        .def("__init__", make_constructor(&Function_2))
        .def("__init__", make_constructor(&Function_3))
        .def("__init__", make_constructor(&Function_4))
        .def("__init__", make_constructor(&Function_5))
        .def("__init__", make_constructor(&Function_6))
        .def("__init__", make_constructor(&Function_7))
        .def("__init__", make_constructor(&Function_8))
        .def("__init__", make_constructor(&Function_9))
        .def(repr(self))
        .def( "eval" , &Function::eval) 
        .def( "eval_vector" , &Function::eval_vector) 
        .def( "eval_matrix" , &Function::eval_matrix) 
        // .def( "gradient" , &Function::gradient) 
        // .def( "jacobian" , &Function::jacobian) 
        ;
}
