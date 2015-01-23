//============================================================================
//                                  I B E X
// File        : pyIbex_Ctc.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include <ibex_Ctc.h>
#include <ibex_CtcFwdBwd.h>
#include <ibex_CtcUnion.h>
#include <ibex_CtcCompo.h>
// #include <ibex_CtcIn.h>
#include <ibex_CtcNotIn.h>
#include <ibex_Function.h>

#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <boost/python.hpp>

using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python;

struct CtcWrap : Ctc, wrapper<Ctc> {
	void contract(IntervalVector& box){
		this->get_override("contract")(box);
	}
};
typedef boost::shared_ptr<Ctc> CtcPtr; 

// Contructor CtcFwdBwd
typedef boost::shared_ptr<ibex::CtcFwdBwd> CtcFwdBwdPtr;
CtcFwdBwdPtr  CtcFwdBwd_1(Function &f, CmpOp op=EQ, FwdMode mode=INTERVAL_MODE ){
    return CtcFwdBwdPtr(new CtcFwdBwd(f, op, mode));
}
CtcFwdBwdPtr  CtcFwdBwd_2(Function &f, CmpOp op=EQ ){
	return CtcFwdBwdPtr(new CtcFwdBwd(f, op));
}
CtcFwdBwdPtr  CtcFwdBwd_3(Function &f ){
	return CtcFwdBwdPtr(new CtcFwdBwd(f));
}

// Constructor CtcUnion
typedef boost::shared_ptr<ibex::CtcUnion> CtcUnionPtr;
// CtcUnionPtr  CtcUnion_1(Ctc &c1, Ctc& c2 ){ return CtcUnionPtr(new CtcUnion(c1, c2)); }
// CtcUnionPtr  CtcUnion_list(const Array<Ctc>& list){ return CtcUnionPtr(new CtcUnion(list)); }

// Constructor CtcCompo
typedef boost::shared_ptr<ibex::CtcCompo> CtcCompoPtr;
// CtcCompoPtr  CtcCompo_1(Ctc &c1, Ctc& c2 ){ return CtcCompoPtr(new CtcCompo(c1, c2)); }
// CtcCompoPtr  CtcCompo_list(const Array<Ctc>& list){ return CtcCompoPtr(new CtcCompo(list)); }


// Contructor CtcIn
boost::shared_ptr<ibex::CtcIn>  CtcIn_Interval(Function &f, const Interval& y){
	return boost::shared_ptr<ibex::CtcIn>(new CtcIn(f, y));
}
boost::shared_ptr<ibex::CtcIn>  CtcIn_IntervalVector(Function &f, const IntervalVector& y ){
	return boost::shared_ptr<ibex::CtcIn>(new CtcIn(f, y));
}
boost::shared_ptr<ibex::CtcIn>  CtcIn_IntervalMatrix(Function &f, const IntervalMatrix& y ){
	return boost::shared_ptr<ibex::CtcIn>(new CtcIn(f, y));
}

// Constructor CtcNotIn
boost::shared_ptr<ibex::CtcNotIn>  CtcNotIn_Interval(Function &f, const Interval& y ){
	return boost::shared_ptr<ibex::CtcNotIn>(new CtcNotIn(f, y));
}



CtcUnionPtr __or(Ctc& c1, Ctc& c2){
	Array<Ctc> lst = Array<Ctc>(c1,c2);
	return CtcUnionPtr(new CtcUnion(lst));
}

CtcCompoPtr __and(Ctc& c1, Ctc& c2){
	Array<Ctc> lst = Array<Ctc>(c1,c2);
	return CtcCompoPtr(new CtcCompo(lst));
}

template<typename CtcType>
boost::shared_ptr<CtcType> ctcFromList(const py::list & lst)
{
    // construct with a list here
    ibex::Array<Ctc> list(len(lst));
    for(uint i = 0; i < len(lst); i++){
        extract<Ctc> get_Ctc(lst[i]);
        if (get_Ctc.check()){
            Ctc* C = extract<Ctc*>(lst[i]);
            list.set_ref(i, *C);
        } else {
            std::cout << "Extraction Error \n";
            return boost::shared_ptr<CtcType>();
        }      
    }
    return boost::shared_ptr<CtcType>(new CtcType(list));
}

void export_Ctc(){

	typedef void (Ctc::*contract_1) (IntervalVector&);
	class_<CtcWrap, boost::noncopyable>("Ctc", no_init)
		.def("contract", pure_virtual( contract_1(&Ctc::contract)))
    	.def_readonly("nb_var", &Ctc::nb_var)
    	.def( "__or__", &__or)
    	.def( "__and__", &__and)
    ;

    enum_<CmpOp>("CmpOp")
    .value( "LT", 	ibex::LT) 
    .value( "LEQ", 	ibex::LEQ)
    .value( "EQ", 	ibex::EQ) 
    .value( "GEQ", 	ibex::GEQ)
    .value( "GT", 	ibex::GT)
    ;

    enum_<FwdMode>("FwdMode") 
    .value( "INTERVAL_MODE", ibex::INTERVAL_MODE)
    .value( "AFFINE2_MODE",  ibex::AFFINE2_MODE)
    .value( "AFFINE_MODE",   ibex::AFFINE_MODE)
    ;

    class_<CtcUnion, bases<Ctc>, boost::noncopyable, boost::shared_ptr<ibex::CtcUnion> >("CtcUnion", no_init)
    	.def("__init__", make_constructor(ctcFromList<CtcUnion>), "CtcUnion from list of contractor\n Usage : CtcUnion([c1, c2, ...])")
    	.def("contract", &CtcUnion::contract)
		;
    class_<CtcCompo, bases<Ctc>, boost::noncopyable, boost::shared_ptr<ibex::CtcCompo> >
        ("CtcCompo", no_init)
    	.def("__init__", make_constructor(ctcFromList<CtcCompo>), "CtcCompo from list of contractor\n Usage : CtcCompo([c1, c2, ...])")
    	.def("contract", &CtcCompo::contract)
    	;

    class_<CtcFwdBwd, bases<Ctc> , boost::noncopyable, boost::shared_ptr<ibex::CtcFwdBwd> >("CtcFwdBwd", no_init)
    	.def("__init__", make_constructor(CtcFwdBwd_1))
    	.def("__init__", make_constructor(CtcFwdBwd_2))
    	.def("__init__", make_constructor(CtcFwdBwd_3))
        .def("contract", &CtcFwdBwd::contract)
    	;

    // class_<CtcIn, bases<Ctc>, boost::noncopyable, boost::shared_ptr<ibex::CtcIn> >("CtcIn", no_init)
    // 	.def("__init__", make_constructor(CtcIn_Interval))
    // 	.def("__init__", make_constructor(CtcIn_IntervalVector))
    // 	.def("__init__", make_constructor(CtcIn_IntervalMatrix))
    // 	.def("contract", &CtcIn::contract);

    class_<CtcNotIn, bases<Ctc>, boost::noncopyable, boost::shared_ptr<ibex::CtcNotIn> >("CtcNotIn", no_init)
    	.def("__init__", make_constructor(CtcNotIn_Interval))
    	.def("contract", &CtcNotIn::contract);

}