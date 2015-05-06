//============================================================================
//                                  I B E X
// File        : pyIbex_Bsc.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#include <ibex_Bsc.h>
#include <ibex_LargestFirst.h>

#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>

#include <boost/python/stl_iterator.hpp>
#include "pyIbex_to_python_converter.h"

using namespace boost;
using namespace boost::python;
using namespace ibex;
namespace py = boost::python;


struct BscWrap : Bsc, wrapper<Bsc> {
	std::pair<IntervalVector,IntervalVector> bisect(const IntervalVector& box){
		return this->get_override("bisect")(box);
	}
	std::pair<IntervalVector,IntervalVector> bisect(Cell& cell){
		if (override f = this->get_override("bisect"))
            return f(cell); // *note*
        return Bsc::bisect(cell);
	}

	void add_backtrackable(Cell& root){
		if (override f = this->get_override("add_backtrackable"))
            f(root); // *note*
        Bsc::add_backtrackable(root);	
	}
    std::pair<IntervalVector,IntervalVector> default_bisect(Cell& cell) { return this->Bsc::bisect(cell); }
    void default_add_backtrackable(Cell& root) { return Bsc::add_backtrackable(root); }
};


boost::shared_ptr<LargestFirst> largestFirstFromList(const py::list& lst){
    std::vector<double> prec_v = to_std_vector<double>(lst);
    return boost::shared_ptr<LargestFirst>(new LargestFirst(Vector(prec_v.size(), &prec_v[0]), Bsc::default_ratio()));
}

void export_Bsc(){
	// Overloading bisection Function
	typedef std::pair<IntervalVector,IntervalVector> (Bsc::*bisect_1) (const IntervalVector&);
	typedef std::pair<IntervalVector,IntervalVector> (Bsc::*bisect_2) (Cell&);

	// Bsc Class binding
	class_<BscWrap, boost::noncopyable>("Bsc", no_init)
		.def("bisect", pure_virtual(bisect_1(&Bsc::bisect) ))
    	.def("bisect", bisect_2(&Bsc::bisect),  &BscWrap::default_bisect)
    	.def("add_backtrackable", &Bsc::add_backtrackable, &BscWrap::default_add_backtrackable)
		.def( "default_ratio", &Bsc::default_ratio )
		.def( "uniform_prec", &Bsc::uniform_prec )
		.def( "prec", &Bsc::prec )
		.def( "too_small", &Bsc::too_small )
    ;

    // LargestFirst Bisector binding
    class_<LargestFirst, bases<Bsc> >("LargestFirst", init<optional<double, double> >())
    	.def(init<Vector&, optional< double> >())
    	.def("__init__", make_constructor(largestFirstFromList))
    	.def("bisect", &LargestFirst::bisect)
    	;
}