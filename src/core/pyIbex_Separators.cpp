//============================================================================
//                               P Y I B E X
// File        : pyIbex_Separator.cpp
// Author      : Benoit Desrochers
// Copyright   : ENSTA Bretagne
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


#ifdef _PYIBEX_WITH_CUSTOM_
#include "ibex_SepInverse.h"
#include "ibex_SepTransform.h"
#include "ibex_SepQInter.h"
#include "ibex_SepPolarXY.h"
#include "ibex_SepPolarXYT.h"
#include "ibex_SepProj.h"
#include "ibex_SepCtcPairProj.h"
#endif

#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>


#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python;


typedef boost::shared_ptr<ibex::SepUnion> SepUnionPtr;
typedef boost::shared_ptr<ibex::SepInter> SepInterPtr;


struct SepWrap : Sep, wrapper<Sep> {
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


//#define DEF_CONSTRUCTOR_PARAM(r, data, i, elem ) BOOST_PP_COMMA_IF( i ) boost::shared_ptr<elem> BOOST_PP_CAT(ptr_arg,i)
//#define DEF_CONSTRUCTOR_CLASS_PARAM(r, data, i, elem ) BOOST_PP_COMMA_IF( i ) data BOOST_PP_CAT(ptr_arg,i)
//#define DEF_MEMBER_PARAM(r, data, i, elem ) boost::shared_ptr<elem> BOOST_PP_CAT(ptr_arg,i);
//#define DEF_ASSIGN_CLASS( r, data, i, elem ) BOOST_PP_CAT(this->ptr_arg,i) = BOOST_PP_CAT(ptr_arg,i);

//#define GEN_CLASS_WITH_SAVED_ARGUMENTS(class_name, new_class_name, params) \
//    class new_class_name: public class_name {                          \
//    public:                                                                     \
//        new_class_name( \
//        BOOST_PP_SEQ_FOR_EACH_I( DEF_CONSTRUCTOR_PARAM, boost::shared_ptr<elem>, params) \
//        ) : class_name(BOOST_PP_SEQ_FOR_EACH_I( DEF_CONSTRUCTOR_CLASS_PARAM, *, params )) { \
//            BOOST_PP_SEQ_FOR_EACH_I( DEF_ASSIGN_CLASS,, params) \
//        } \
//    protected:\
//        BOOST_PP_SEQ_FOR_EACH_I( DEF_MEMBER_PARAM,boost::shared_ptr<elem>, params)\
//    };

//namespace ibex {

//GEN_CLASS_WITH_SAVED_ARGUMENTS(SepTransform,SepTransforWrapper, (Sep)(Function)(Function));
//GEN_CLASS_WITH_SAVED_ARGUMENTS(SepCtcPair,SepCtcPairWrapper, (Ctc)(Ctc));


//class SepTransforWrapper : public SepTransform {
//public:
//    SepTransforWrapper(SepPtr sep, boost::shared_ptr<Function> f1, boost::shared_ptr<Function> f2):
//        SepTransform(*sep, *f1, *f2),
//        sep(sep), f1(f1), f2(f2){}

//private:
//    SepPtr sep;
//    boost::shared_ptr<Function> f1;
//    boost::shared_ptr<Function> f2;

//};




//}
void export_Separators(){

    typedef void (Sep::*separate) (IntervalVector&, IntervalVector&);
    class_<SepWrap, boost::noncopyable>("Sep", init<>())
            .def("separate", pure_virtual( separate(&Sep::separate)))
            .def("__or__", &__or, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0,1, with_custodian_and_ward_postcall<0,2 > > >())
            .def("__and__", &__and, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0,1, with_custodian_and_ward_postcall<0,2 > > >());
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

#ifdef _PYIBEX_WITH_CUSTOM_
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

    class_<SepPolarXY, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepPolarXY> >("SepPolarXY", no_init)
            .def(init<Interval, Interval>())
            .def("separate", &SepPolarXY::separate);

    class_<SepPolarXYT, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepPolarXYT> >("SepPolarXYT", no_init)
            .def(init<Interval, Interval, double , double>())
            .def("separate", &SepPolarXYT::separate);

    class_<SepProj, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepProj> >("SepProj", no_init)
            .def(init<Sep&, const IntervalVector&, double, int>()[with_custodian_and_ward<1,2>()])
            .def("separate", &SepProj::separate);

    class_<SepCtcPairProj, bases<Sep>, boost::noncopyable, boost::shared_ptr<ibex::SepCtcPairProj> >("SepCtcPairProj", no_init)
            .def(init<Ctc&, Ctc&, const IntervalVector&, double>()[with_custodian_and_ward<1,2, with_custodian_and_ward<1,3> >()])
            .def("separate", &SepCtcPairProj::separate);
#endif
}
