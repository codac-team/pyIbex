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
#include <ibex_CtcInverse.h>
#include <ibex_CtcPolar.h>
#include <ibex_CtcSegment.h>
#include <ibex_CtcQInter.h>


#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <boost/python.hpp>

using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python;

namespace  ibex {


struct CtcWrap : Ctc, wrapper<Ctc> {
    void contract(IntervalVector& box){
        this->get_override("contract")(box);
    }
};

}
typedef boost::shared_ptr<Ctc> CtcPtr;

// Contructor CtcFwdBwd
typedef boost::shared_ptr<ibex::CtcFwdBwd> CtcFwdBwdPtr;

// Constructor CtcUnion
typedef boost::shared_ptr<ibex::CtcUnion> CtcUnionPtr;

// Constructor CtcCompo
typedef boost::shared_ptr<ibex::CtcCompo> CtcCompoPtr;


CtcUnion* __or(Ctc& c1, Ctc& c2){ return (new CtcUnion(c1, c2)); }
CtcCompo* __and(Ctc& c1, Ctc& c2){ return (new CtcCompo(c1, c2)); }


boost::shared_ptr<CtcQInterProjF> CtcQInterProjFFromList(py::list & lst, int q)
{
    // construct with a list here
//    incref(lst);
    ibex::Array<Ctc> list(len(lst));
    for(uint i = 0; i < len(lst); i++){
        extract<Ctc> get_Ctc(lst[i]);
        if (get_Ctc.check()){
            Ctc* C = extract<Ctc*>(lst[i]);

            list.set_ref(i, *C);
        } else {
            std::cout << "Extraction Error \n";
            return boost::shared_ptr<CtcQInterProjF>();
        }
    }
    return boost::shared_ptr<CtcQInterProjF>(new CtcQInterProjF(list, q));
}


template<typename CtcType>
boost::shared_ptr<CtcType> ctcFromList(PyObject* obj_ptr)
{

    boost::python::list lst(boost::python::handle<>(boost::python::borrowed(obj_ptr)));
    Py_IncRef(obj_ptr);

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
    class_<CtcWrap, boost::noncopyable, boost::shared_ptr<CtcWrap> >("Ctc", no_init)
            .def("contract", pure_virtual( contract_1(&Ctc::contract)))
            .def_readonly("nb_var", &Ctc::nb_var)
            .def("__or__", &__or, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0,1, with_custodian_and_ward_postcall<0,2 > > >())
            .def("__and__", &__and, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0,1, with_custodian_and_ward_postcall<0,2 > > >());
            ;

    // Export comparaison constant
    enum_<CmpOp>("CmpOp")
            .value( "LT", 	ibex::LT)
            .value( "LEQ", 	ibex::LEQ)
            .value( "EQ", 	ibex::EQ)
            .value( "GEQ", 	ibex::GEQ)
            .value( "GT", 	ibex::GT)
            ;

    // Export computation mode
    enum_<FwdMode>("FwdMode")
            .value( "INTERVAL_MODE", ibex::INTERVAL_MODE)
            .value( "AFFINE2_MODE",  ibex::AFFINE2_MODE)
            .value( "AFFINE_MODE",   ibex::AFFINE_MODE)
            ;

    // Export CtcUnion
    class_<CtcUnion, bases<Ctc>, boost::noncopyable, boost::shared_ptr<ibex::CtcUnion> >("CtcUnion", no_init)
            .def(init<ibex::Array<Ctc> >()[with_custodian_and_ward<1, 2>()])
//            .def("__init__", make_constructor(ctcFromList<CtcUnion>), "CtcUnion from list of contractor\n Usage : CtcUnion([c1, c2, ...])")
            .def("contract", &CtcUnion::contract)
            ;

    // Export CtcCompo
    class_<CtcCompo, bases<Ctc>, boost::noncopyable, boost::shared_ptr<ibex::CtcCompo> >("CtcCompo", no_init)
            .def(init<ibex::Array<Ctc> >()[with_custodian_and_ward<1, 2>()])
//            .def("__init__", make_constructor(ctcFromList<CtcCompo>), "CtcCompo from list of contractor\n Usage : CtcCompo([c1, c2, ...])")
            .def("contract", &CtcCompo::contract)
            ;

    // Export CtcFwdBwd
    class_<CtcFwdBwd, bases<Ctc> , boost::noncopyable, boost::shared_ptr<ibex::CtcFwdBwd> >("CtcFwdBwd", no_init)
            .def(init<Function&,optional<CmpOp, FwdMode> > ()[with_custodian_and_ward<1, 2>()])
            .def(init<Function&,Interval&>()[with_custodian_and_ward<1, 2>()])
            .def(init<Function&,IntervalVector&>()[with_custodian_and_ward<1, 2>()])
            .def("contract", &CtcFwdBwd::contract)
            ;

    // Export CtcInverse
    class_<CtcInverse, bases<Ctc> , boost::noncopyable, boost::shared_ptr<ibex::CtcInverse> >("CtcInverse", no_init)
            .def(init<Ctc&, Function&>()[with_custodian_and_ward<1, 2, with_custodian_and_ward<1, 3> >()])
            .def("contract", &CtcInverse::contract);

    // Export CtcNotIn
    class_<CtcNotIn, bases<Ctc>, boost::noncopyable, boost::shared_ptr<ibex::CtcNotIn> >("CtcNotIn", no_init)
            .def(init<Function&, Interval&>()[with_custodian_and_ward<1, 2 >()])
            .def(init<Function&, IntervalVector&>()[with_custodian_and_ward<1, 2 >()])
            .def("contract", &CtcNotIn::contract);

    // Export CtcPolar
    void (CtcPolar::*contract_ctcPolar_1) (IntervalVector&) = &CtcPolar::contract;
    void (CtcPolar::*contract_ctcPolar_2) (Interval&, Interval&, Interval&, Interval&)  = &CtcPolar::contract;
    class_<CtcPolar, bases<Ctc>, boost::noncopyable, boost::shared_ptr<ibex::CtcPolar> >("CtcPolar")
            .def("contract", contract_ctcPolar_1)
            .def("contract", contract_ctcPolar_2);

    // Export CtcSegment
    class_<CtcSegment, bases<Ctc> , boost::noncopyable, boost::shared_ptr<ibex::CtcSegment> >("CtcSegment", no_init)
            .def(init<double, double,double,double>())
            .def("contract", &CtcSegment::contract);

    // Export CtcQInterProjF
    class_<CtcQInterProjF, bases<Ctc> , boost::noncopyable, boost::shared_ptr<ibex::CtcQInterProjF> >("CtcQInterProjF", no_init)
//            .def("__init__", make_constructor(CtcQInterProjFFromList))
            .def(init<Array<Ctc>, int>()[with_custodian_and_ward<1,2>()])
            .def("contract", &CtcQInterProjF::contract);
}
