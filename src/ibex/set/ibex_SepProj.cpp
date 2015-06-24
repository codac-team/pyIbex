//============================================================================
//                                  I B E X                                   
// File        : ibex_sepProj.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================
#include "ibex_SepProj.h"

#include "vibes.h"
#include "ibex_NoBisectableVariableException.h"
using namespace std;

namespace ibex {

SepProj::SepProj(Sep& sep, const IntervalVector& y_init, double prec, int nb_cut) : sep(sep),
    y_init(y_init), prec(prec), nb_cut(nb_cut)
{
    //    clearFlags();
    bsc = new LargestFirst(prec);
}

SepProj::~SepProj() {}


/**
 * @brief SepProj::process Separate cartesian product [x_in].[y] and [x_out].[y]
 *              if an inner (or outer) contraction happends, the flags impact_cin is set to true
 *              and the removed part of the box is stored in first_cin_boxes.
 *
 * @param x_in : projected inner box
 * @param x_out : projected outer box
 * @param y :  parameter box
 * @return true if x_in or x_out is empty.
 */
bool SepProj::process(IntervalVector& x_in, IntervalVector& x_out, IntervalVector &y, ImpactStatus& impact){
    IntervalVector x = (x_in & x_out);
    if (x.is_empty()) return true;


    // Separate the product [x].[y]
    IntervalVector XinFull = cart_prod(x, y);
    IntervalVector XoutFull = cart_prod(x, y);
    sep.separate(XinFull, XoutFull);



    if (XinFull.is_empty() && XoutFull.is_empty()){
        cout << "Erreur !!!!" << __LINE__  << "\n ";
        exit(-1);
    }
    if ( XinFull.is_empty() || !(y == XinFull.subvector(x_in.size(), XinFull.size() - 1 )))
//         ( !y.is_flat() &&  XinFull.subvector(x_in.size(), XinFull.size() - 1 ).is_strict_subset(y) ) )
    {
        x_in.set_empty();
        impact.setCinFlags(x_in, x);
        return true;
    } else {
        x_in = XinFull.subvector(0, x_in.size()-1);
        impact.setCinFlags(x_in, x);
    }

    if( XoutFull.is_empty()){
        x_out.set_empty();
        if( !y.is_flat())
            impact.setCoutFlags(x_out, x);
        y.set_empty();
        return true;
    } else {
        x_out = XoutFull.subvector(0, x_out.size()-1);
        y = XoutFull.subvector(x_out.size(), XoutFull.size()-1);
        if( !y.is_flat())
            impact.setCoutFlags(x_out, x);
    }
    return false;

}

bool SepProj::fixpoint2(IntervalVector& x_in, IntervalVector& x_out, IntervalVector& x_out_mid, IntervalVector& y, ImpactStatus& impact){
    double ratio = 0.001;
    IntervalVector x = x_in & x_out;
    IntervalVector x_old(x); // tmporary box use during the fix point.
    bool stop;
    do {
        x_old = x;
        stop = process(x_in, x_out, y, impact);
        if (!stop){
            IntervalVector y_mid(y.mid());
            x_out_mid = x_out;
            stop = process(x_in, x_out_mid, y_mid, impact);
        }
        x = x_in & x_out;
        if ( x.is_empty() /*|| x.max_diam() < prec*/)
            return true;
        // The condifion || is more efficient
    } while (( x_old.rel_distance(x_in)>ratio || x_old.rel_distance(x_out)>ratio ));
    vibes::drawBox(x_out, "gray");
    if(impact.impact_cin == false && impact.impact_cout == false){
        assert(x_in == x && x_out == x);
    }
    return false;
}


bool SepProj::fixpoint(IntervalVector& x_in, IntervalVector& x_out, IntervalVector& y, ImpactStatus& impact){
    double ratio = 0.001;
    IntervalVector x = x_in & x_out;
    IntervalVector x_old(x); // tmporary box use during the fix point.
    bool stop;
    do {
        x_old = x;
        stop = process(x_in, x_out, y, impact);
        if (!stop){
            IntervalVector y_mid(y.mid());
            IntervalVector x_out_mid(x_out);
            stop = process(x_in, x_out_mid, y_mid, impact);
//            if (!stop){
//                y_mid = IntervalVector(y.lb());
//                x_out_mid = x_out;
//                stop = process(x_in, x_out_mid, y_mid, impact);
//                if (!stop){
//                    y_mid = IntervalVector(y.ub());
//                    x_out_mid = x_out;
//                    stop = process(x_in, x_out_mid, y_mid, impact);
//                }
//            }
        }
        x = x_in & x_out;
        if ( x.is_empty() /*|| x.max_diam() < prec*/)
            return true;
        // The condifion || is more efficient
    } while (( x_old.rel_distance(x_in)>ratio || x_old.rel_distance(x_out)>ratio ));
    return false;
}

bool SepProj::separate_fixPoint(IntervalVector& x_in, IntervalVector& x_out, IntervalVector& y){

    assert(x_in == x_out);
    IntervalVector x_old0(x_in & x_out); // Initial box
    //    clearFlags();
    ImpactStatus impact;
    fixpoint(x_in, x_out, y, impact);
    impact.reconstrut(x_in, x_out, x_old0);
    if( (x_in & x_out).is_empty() == true)
        return true;
    return false;
    //    reconstrut(x_in, x_out, x_old0);

}


//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================


IntervalVector SepProj::separate_rec2(IntervalVector&x_in, IntervalVector& x_out,  IntervalVector& y, ImpactStatus& impact){
    assert( x_in == x_out);
    IntervalVector x(x_in & x_out);
    ImpactStatus impact_tmp;
    // Separate x_in and x_out
    fixpoint(x_in, x_out, y, impact_tmp);
//    impact.reconstrut(x_in, x_out, x);
//    assert( (x_in | x_out) == x);

    // Si la contraction extérieur n'as eu aucun effet
//    if( impact.impact_cout == false){
//        x_out_res |= x_out;
//    }
    x = x_in & x_out;
    // si les criteres d'arret de la recherche sont satisfait
    if(x.is_empty() || x.max_diam() < prec || y.is_empty() || y.max_diam() < x.max_diam()){
        return x;
    } else {
        try{
            // Si x_out n'est pas inclus x_out_res
            std::pair<IntervalVector, IntervalVector> cut = bsc->bisect(y);
            IntervalVector xin1(x), xin2(x),xout1(x), xout2(x);


            x |= separate_rec2(xin1, xout1, cut.first, impact);
            xin2 &= (xin1 & xout1);
            xout2 &= xin2;
            x |= separate_rec2(xin2, xout2, cut.second, impact);

            // sinon on bisect y et on reconmmence
        } catch (NoBisectableVariableException& e){

        }

    }
    return x;

}


IntervalVector SepProj::separate_rec(IntervalVector &x_in, IntervalVector &x_out, IntervalVector& y, ImpactStatus& impact){
    assert( x_in == x_out);

    IntervalVector x(x_in & x_out);
    fixpoint(x_in, x_out, y, impact);
    impact.reconstrut(x_in, x_out, x);

    assert( (x_in | x_out) == x);
    x = x_in & x_out;

    if(x.is_empty() || x.max_diam() < prec || y.is_empty() || y.max_diam() < x.max_diam()){
        return x;
    } else {
        try{
            std::pair<IntervalVector, IntervalVector> cut = bsc->bisect(y);
            ImpactStatus impact1;
            ImpactStatus impact2;
            IntervalVector xin1(x), xin2(x),xout1(x), xout2(x);
            separate_rec(xin1, xout1, cut.first, impact1);
            separate_rec(xin2, xout2, cut.second, impact2);


            if (impact1.impact_cin == true || impact2.impact_cin == true){
                if(impact.impact_cin == false){
                    impact.impact_cin = true;
                    xin1 &= xin2;
                    impact.n_in = x.diff(xin1, impact.first_cin_boxes);
                }
            }
            if (impact1.impact_cout == true && impact2.impact_cout == true){
                if( impact.impact_cout == true){
                    impact.impact_cout = false;
                    delete[] impact.first_cout_boxes;
                    impact.first_cout_boxes = NULL;
                }
                if( impact.impact_cout == false){
                    impact.impact_cout = true;
                    xout1 = x & (xout1 | xout2);
                    impact.n_out = x.diff(xout1, impact.first_cout_boxes);
                }
            }
        } catch( ibex::NoBisectableVariableException& e){
            return x;
        }
    }
    return x;

}

// COnditions d'arreet:
//  - pas de contraction sep est inactif.
//  - la boite x_out est inclue dans x_res
bool SepProj::proceed2(IntervalVector& x, IntervalVector& x_res, IntervalVector& y, ImpactStatus& impact, std::stack<TwoItv>& l ){

    // x_in : inner box to be contracted
    // x_out_res : when the outer contractor is ineficient, the resulting boc

    IntervalVector x_in(x), x_out_mid(x);
    IntervalVector x_out(x);
    vibes::drawBox(x, "y");
    ImpactStatus impact_tmp;
    this->fixpoint2(x_in, x_out, x_out_mid, y, impact_tmp);
    x = x_in & x_out;
    if(x.is_empty()) return false;
    vibes::drawBox(x, "b");
    if(impact_tmp.impact_cin == false && impact_tmp.impact_cout == false){
        x_res |= x;
        return false;
    }

    if (!x.is_subset(x_res)) {
        if (y.max_diam()<=prec) {
            x_res |= x;
//            if (x_res==x_init) return true;
        }
        else {

            l.push(pair<IntervalVector,IntervalVector>(x,y));
            x_res |= (x_out_mid & x);


        }
    }
    return false;
}

bool SepProj::proceed3(IntervalVector& x_in, IntervalVector& x_out, IntervalVector& x0, IntervalVector& x_res, IntervalVector& y, std::stack<IntervalVector> &l){
//    if(x.is_empty()) return true;
    x_out = x0;
    IntervalVector x(x_in & x_out), x_out_mid(x_out);
    IntervalVector x_out1(x_out), x_in1(x_in);
//    x_in = x0;
    ImpactStatus impact, impact0;
    impact0.setCinFlags(x_in, x0);
    impact0.setCoutFlags(x_out, x0);

    vibes::drawBox(x_in, "g");
    vibes::drawBox(x_out, "k");
    fixpoint2(x_in, x_out, x_out_mid, y, impact);

    if(impact.impact_cin == true || impact.impact_cout == true)
        impact.reconstrut(x_in, x_out, x);
    if(impact0.impact_cin == true || impact0.impact_cout == true)
        impact0.reconstrut(x_in, x_out, x0);
    vibes::drawBox(x_in, "y");
    vibes::drawBox(x_out, "r");
    cerr << (x_in) << " " << (x_out) << " " << x0 << (x_in1 == x_in) << " " << (x_out1 == x_out) << "\n";

    assert( (x_in | x_out) == x0);

    if( impact.impact_cout == false && impact.impact_cin == false){
        x_res |= x_out;
        return false;
    }

    if(!x_out.is_subset(x_res)){
        if (y.max_diam()<=prec) {
            x_res |= x_out;
            if (x_res==x0) return true;
        } else {
            l.push(y);
            IntervalVector x_in2(x_in);
            impact.reconstrut(x_in2, x_out_mid, x0);
            x_res |= x_out_mid;
        }
    }
    return false;
}

void SepProj::proceed4(IntervalVector &x_in, IntervalVector &x_save, IntervalVector &x_out_res, IntervalVector &y, std::queue<TwoItv> &l){

    bool debug = false;
    bool diff = false;
    IntervalVector x_out(x_save);
    if ((x_in & x_out).is_empty()) return ;
//    cerr <<x_in << " " << x_out << "\n";
//    if ( (x_in & x_out).is_empty() ) return;
    IntervalVector x0(x_in | x_out);
    if (debug) vibes::clearFigure();
    if (debug) vibes::drawBox(x_in, "r");
    if (debug) vibes::drawBox(x_out, "b");
    if (debug) vibes::drawBox(x0, "k");


    ImpactStatus impact0;
    if( !( x_in == x_out) ){
        impact0.setCinFlags(x_in, x0);
        impact0.setCoutFlags(x_out, x0);
        diff = true;
    }
    x_in &= x_out;
    x_out &= x_in;
    fixpoint(x_in, x_out, y, impact0);
//    separate_fixPoint(x_in, x_out, y);
    if (debug) vibes::drawBox(x_in, "r");
    if (debug) vibes::drawBox(x_out, "b");
    if (debug) vibes::drawBox(x0, "k");
    impact0.reconstrut_v2(x_in, x_out, x0);

//    if (x_in.is_empty() ){
//        for(int i = 0; i < impact0.n_in; i++){
//            x_out |= impact0.first_cin_boxes[i];
//        }
//        return;
//    }
//    if(x_out.is_empty() ){
//        for(int i = 0; i < impact0.n_out; i++){
//            x_in |= impact0.first_cout_boxes[i];
//        }
//        return;
//    }
//    if( diff == true ){
////        impact0.reconstrut(x_in, x_out, x0);
//        for(int i = 0; i < impact0.n_in; i++){
//            x_out |= impact0.first_cin_boxes[i];
//        }
//        for(int i = 0; i < impact0.n_out; i++){
//            x_in |= impact0.first_cout_boxes[i];
//        }
//    }
    if (debug) vibes::drawBox(x_in, "r");
    if (debug) vibes::drawBox(x_out, "b");
    if (debug) vibes::drawBox(x0, "k");


    IntervalVector x = x_in & x_out;
    if (x.is_empty() || x.max_diam() < prec || y.is_empty() || y.max_diam() < x.max_diam()){
        x_out_res |= x_out;
    } else {
        l.push(TwoItv(x_out, y));
    }
}

void SepProj::separate(IntervalVector &x_in, IntervalVector &x_out){


    //    clearFlags();
//#define SIMPLE_ALG

#ifdef SIMPLE_ALG
#define SIMPLE2
#ifdef SIMPLE2
//    IntervalVector y(y_init);
//    separate_fixPoint(x_in, x_out, y);
    IntervalVector x_old0(x_in & x_out);
    IntervalVector y(y_init);
    ImpactStatus impact;
    IntervalVector x_res = separate_rec2(x_in, x_out,y, impact);
    x_in = x_res;
    x_out = x_res;
//    cout << x_in << " "<< x_old0 << "\n" << std::flush;
//    impact.reconstrut(x_in, x_out, x_old0);

#else
    IntervalVector xold(x_in & x_out);

    IntervalVector x_out_res(x_in.size(), Interval::EMPTY_SET);
    queue<IntervalVector> l, ll;
    l.push(y_init);
    while (!l.empty()){
        if(l.front().max_diam() > xold.max_diam()){
            try{
                TwoItv cut = bsc->bisect(l.front());
                l.push(cut.first);
                l.push(cut.second);
                l.pop();

            } catch(ibex::NoBisectableVariableException&e){
                ll.push(l.front());
                l.pop();
            }
        } else {
            ll.push(l.front());
            l.pop();
        }
    }

//    for( int i = 0; i < ll.size(); i++){
    while(!ll.empty()){
        IntervalVector x_out_tmp(x_out);
        separate_fixPoint(x_in, x_out_tmp, ll.front());
        ll.pop();
        x_out_res |= x_out_tmp;
    }
    x_out = x_out_res;

    assert( (x_in | x_out) == xold);



#endif


#else
    assert(x_in == x_out);
    IntervalVector x_old0(x_in & x_out); // Initial box
    IntervalVector y(y_init);
    IntervalVector x_res  = IntervalVector::empty(x_in.size());
    std::queue<TwoItv> l;
    IntervalVector x(x_in & x_out);

//    vibes::drawBox(x_old0, "y");
    l.push(TwoItv(x_out, y_init));
    static int k = 0;
    while(!l.empty()){
//        std::cerr << "stack size: " << l.size() << " " << x_in << " " << x_out << "\n";
        try{

            IntervalVector x_save(l.front().first);
            TwoItv cut = bsc->bisect(l.front().second);

            l.pop();
            proceed4(x_in, x_save, x_res, cut.second, l);
//            cerr << x_old0 << " | " << x_in << " " << x_res << " " << x_save << "\n";
            if(x_in.is_empty() ) break;
            proceed4(x_in, x_save, x_res, cut.first, l);
//            cerr << x_old0 << " | " << x_in << " " << x_res << " " << x_save << "\n";
            k++;
        } catch (ibex::NoBisectableVariableException& e){
            l.pop();
        }
    }
//    vibes::drawBox(x_in, "r");
//    vibes::drawBox(x_res, "b");
    x_out = x_res;
    x_in = x_in;
#endif
}


} // namespace ibex

