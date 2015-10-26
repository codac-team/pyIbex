//============================================================================
//                               P Y I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Mar 02, 2015
//============================================================================

#include "pyIbex_to_python_converter.h"

#include "ibex_IntervalVector.h"
#include "ibex_Sep.h"
#include "ibex_SepUnion.h"
#include "ibex_SepInter.h"
#include "ibex_SepNot.h"
#include "ibex_SepCtcPair.h"
#include "ibex_SepFwdBwd.h"
#include "ibex_SepBoundaryCtc.h"
#include "ibex_SepPolygon.h"

#include "ibex_SepInverse.h"
#include "ibex_SepTransform.h"
#include "ibex_SepQInter.h"
#include "ibex_SepCtcPairProj.h"

#include "ibex_Set.h"
#include "ibex_SetInterval.h"


#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python;


typedef boost::shared_ptr<ibex::SepUnion> SepUnionPtr;
typedef boost::shared_ptr<ibex::SepInter> SepInterPtr;


struct SepWrap : Sep, wrapper<Sep> {
    SepWrap(int nb_var) : Sep(nb_var) {}
    void separate(IntervalVector& xin, IntervalVector& xout){
        this->get_override("separate")(boost::ref(xin), boost::ref(xout));
    }
};
SepUnion* __or(Sep& s1, Sep& s2) { return (new SepUnion(s1, s2)); }
SepInter* __and(Sep& s1, Sep& s2){ return (new SepInter(s1, s2)); }


boost::shared_ptr<SepPolygon> initFromList(const py::list& lst){
    // check if the list contains two sub-list [[ x1, x2, ..., xn], [ y1, y2, ..., yn]]

    boost::python::ssize_t n = boost::python::len(lst);
    if(n != 2){
        std::cout << "Invalide input argmment: expected [[ x1, x2, ..., xn], [ y1, y2, ..., yn]]\n";
        throw;
    }
    std::vector<double> lx = to_std_vector<double>(lst[0]);
    std::vector<double> ly = to_std_vector<double>(lst[1]);
    n = lx.size();
    std::vector<double> ax(n), ay(n),bx(n),by(n);
    for(int i = 0; i < n; i++){
        ax[i] = lx[i];
        ay[i] = ly[i];
        bx[i] = lx[ (i+1)%n ];
        by[i] = ly[ (i+1)%n ];
    }
    return boost::shared_ptr<SepPolygon>(new SepPolygon(ax, ay, bx, by));
}


void export_Separators(){

    typedef void (Sep::*separate) (IntervalVector&, IntervalVector&);
    typedef void (Sep::*contract_1) (Set& , double);
    typedef void (Sep::*contract_2) (SetInterval& , double);
    class_<SepWrap, boost::noncopyable>("Sep", init<int >())
            .def("separate", pure_virtual( separate(&Sep::separate)))
            .def("contract", contract_1(&Sep::contract))
            .def("contract", contract_2(&Sep::contract))
            .def("__or__", &__or, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0,1, with_custodian_and_ward_postcall<0,2 > > >())
            .def("__and__", &__and, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0,1, with_custodian_and_ward_postcall<0,2 > > >())
            .def_readonly("nb_var", &Sep::nb_var);
            ;

    class_<SepUnion, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepUnion> >("SepUnion", no_init)
            //    	.def("__init__", make_constructor(ctcFromList<SepUnion>), "SepUnion from list of separators\n Usage : SepUnion([s1, s2, ...])")
            .def(init<Array<Sep> >()[with_custodian_and_ward<1,2>()])
            .def("separate", &SepUnion::separate)
            ;

    class_<SepInter, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepInter> >
            ("SepInter", no_init)
            .def(init<Array<Sep> >()[with_custodian_and_ward<1,2>()])
            //        .def("__init__", make_constructor(ctcFromList<SepInter>), "SepInter from list of separators\n Usage : SepInter([s1, s2, ...])")
            .def("separate", &SepInter::separate)
            ;


    class_<SepCtcPair, bases<Sep>,  boost::noncopyable, boost::shared_ptr<ibex::SepCtcPair> >
            ("SepCtcPair", init< Ctc&, Ctc&>()[with_custodian_and_ward<1,2, with_custodian_and_ward<1, 3> >()])
            .def("separate", &SepCtcPair::separate)
            ;


    class_<SepFwdBwd, bases<Sep>,  boost::noncopyable, boost::shared_ptr<ibex::SepFwdBwd> >("SepFwdBwd", no_init)
            .def(init< Function&, CmpOp >()[with_custodian_and_ward<1,2>()])
            .def(init<Function&, Interval& >()[with_custodian_and_ward<1,2>()])
            .def(init<Function&, IntervalVector& >()[with_custodian_and_ward<1,2>()])
            //            .def(init<Function&, IntervalMatrix& > ())
            .def("separate", &SepFwdBwd::separate)
            ;

    class_<SepNot, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepNot> >("SepNot", no_init)
            .def(init<Sep&>()[with_custodian_and_ward<1,2>()])
            .def("separate", &SepNot::separate);

    class_<SepPolygon, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepPolygon> >("SepPolygon", no_init)
            .def("__init__", make_constructor(initFromList))
            .def("separate", &SepPolygon::separate);


    class_<SepQInterProjF, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepQInterProjF> >("SepQInterProjF", no_init)
            .def(init<Array<Sep> >()[with_custodian_and_ward<1,2>()])
            //          .def("__init__", make_constructor(ctcFromList<SepQInterProjF>), "SepQInterProjF from list of separators\n Usage : SepQInterProjF([s1, s2, ...])")
            .def("separate", &SepQInterProjF::separate)
            .add_property("q", &SepQInterProjF::getq, &SepQInterProjF::setq)
            ;

    class_<SepInverse, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepInverse> >("SepInverse", no_init)
            .def(init<Sep&, Function& >()[with_custodian_and_ward<1,2, with_custodian_and_ward<1,3> >()])
            .def("separate", &SepInverse::separate);


    class_<SepTransform, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepTransform> >("SepTransform", no_init)
            .def(init<Sep&, Function&, Function&>()[with_custodian_and_ward<1,2, with_custodian_and_ward<1,3, with_custodian_and_ward<1,4> > >()])
            .def("separate", &SepTransform::separate);

    class_<SepCtcPairProj, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepCtcPairProj> >("SepCtcPairProj", no_init)
            .def(init<Ctc&, Ctc&, const IntervalVector&, double>()[with_custodian_and_ward<1,2, with_custodian_and_ward<1,3> >()])
            .def("separate", &SepCtcPairProj::separate);
}
