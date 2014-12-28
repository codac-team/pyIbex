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

using namespace boost;
using namespace boost::python;
using namespace ibex;



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
    	.def("bisect", &LargestFirst::bisect)
    	;
}