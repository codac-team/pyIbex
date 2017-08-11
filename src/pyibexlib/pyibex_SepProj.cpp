//============================================================================
//                                  I B E X
// File        : ibex_sepProj.cpp
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================
#include "pyibex_SepProj.h"

// #include "vibes.h"
#include "ibex_NoBisectableVariableException.h"
using namespace std;

namespace pyibex {



SepProj::SepProj(Sep& sep, const IntervalVector& y_init, double prec) : Sep(sep.nb_var), sep(sep),
    y_init(y_init), prec(prec), nbx(0)
{
    // The LargestFirst minimal size is set to a very small number to avoid
    //  NoBisectableVariableException to be raised
    bsc = new LargestFirst(1e-10*prec);
}

SepProj::SepProj(Sep& sep, const Interval& y_init, double prec) : Sep(sep.nb_var), sep(sep),
    y_init(1, y_init), prec(prec), nbx(0)
{
    // The LargestFirst minimal size is set to a very small number to avoid
    //  NoBisectableVariableException to be raised
    bsc = new LargestFirst(1e-10*prec);
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
bool SepProj::process(IntervalVector& x_in, IntervalVector& x_out, IntervalVector &y, ImpactStatus& impact, bool use_point){
    IntervalVector x = (x_in & x_out);
    if (x.is_empty()) return true;


    // Separate the product [x].[y]
    IntervalVector XinFull = cart_prod(x, y);
    IntervalVector XoutFull = cart_prod(x, y);
    sep.separate(XinFull, XoutFull);
    nbx++;

    if (!((XinFull | XoutFull)  == cart_prod(x, y))){
      std::cerr << "##########################################################\n";
      std::cerr << XinFull << " " << XoutFull << "\n";
      std::cerr << cart_prod(x, y) << " "  << (XinFull | XoutFull)  << "\n";
      std::cerr << "##########################################################\n";

      assert((XinFull | XoutFull)  == cart_prod(x, y));


    }

    // Handle error case
    if (XinFull.is_empty() && XoutFull.is_empty()){
        cout << "Erreur !!!! line" << __LINE__  << "\n ";
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
        if( use_point == false)
            impact.setCoutFlags(x_out, x);
        y.set_empty();
        return true;
    } else {
        x_out = XoutFull.subvector(0, x_out.size()-1);
        y = XoutFull.subvector(x_out.size(), XoutFull.size()-1);
        if( use_point == false)
            impact.setCoutFlags(x_out, x);
    }
    return false;
}


void complementaryUnion(IntervalVector& x, const IntervalVector& y, const IntervalVector& x0){
  IntervalVector *res;
  if ((x == y) || (x == x0)) return;
  int n = x0.diff(y, res);
  for(int i = 0; i < n; i++){
    x |= res[i];
  }
  delete[] res;
}

bool SepProj::fixpoint(IntervalVector& x_in, IntervalVector& x_out, IntervalVector& y){
    double ratio = 0.001;
    IntervalVector x0(x_in | x_out);

    ImpactStatus impact(x_in, x_out);
    x_in &= x_out;   x_out &= x_in;

    IntervalVector x = x_in & x_out;
    IntervalVector x_old(x); // tmporary box use during the fix point.
    bool stop;

    do {
        x_old = x;
        IntervalVector x_out0(x_out);
        // std::cerr << ">>>> "<< x0 << " " << x_in << " " << x_out << "\n";

        stop = process(x_in, x_out, y, impact, false);
        if (!stop){
            IntervalVector y_mid(y.mid());
            IntervalVector x_out_mid(x_out0);
            stop = process(x_in, x_out_mid, y_mid, impact, true);
        }
        x = x_in & x_out;
        break;

      // The condifion || is more efficient
    } while (!x.is_empty() && ( x_old.rel_distance(x_in)>ratio || x_old.rel_distance(x_out)>ratio ));

    // std::cout << x_in << " "  << x_out << " \n";
    impact.reconstrut_v2(x_in, x_out, x0);
    // if (! ((x_in | x_out) == x0)){
    //   std::cerr << x_in << " " << x_out << " " << x0 << "\n";
    //   exit(-1);
    // }
    return !x.is_empty() ? true : false;
}



//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================





void SepProj::separate(IntervalVector &x_in, IntervalVector &x_out){

    //    clearFlags();
//#define SIMPLE_ALG

    assert(x_in == x_out);
    IntervalVector x_old0(x_in & x_out); // Initial box
    IntervalVector y(y_init);
    IntervalVector x_res  = IntervalVector::empty(x_in.size());
    std::queue<TwoItv> l;
    IntervalVector x(x_in & x_out);

    // vibes::drawBox(x_old0, "y");
    l.push(TwoItv(x_out, y_init));
    static int k = 0;
    // std::cerr << "###########################################################\n";
    while(!l.empty()){
        IntervalVector x_out_save(l.front().first);
        IntervalVector y = l.front().second;
        IntervalVector x_out0(x_out);

        l.pop();
        if (x_out_save.is_subset(x_res))
          continue;
        complementaryUnion(x_out_save, x_in, x_old0);
        // if ((x_in & x_out_save).is_empty()){
        //   // std::cout << x_in << " " << x_out_save << "\n";
        //   x_res |= x_out_save;
        //   continue;
        // };
        IntervalVector y0(y);
        // std::cerr << "##########################################################################\n";

        fixpoint(x_in, x_out_save, y);
        IntervalVector x = x_in & x_out_save;
        if (x_out_save.is_empty()) continue;
        // std::cerr << x_in << " " << x_out_save << "\n";
        // std::cerr << "x0: " << x_out0 << "\n";
        // std::cerr << "x: " << x << " x_in: " << x_in << " x_out: " << x_out_save << "\n"
                  // << "x_res " << x_res  <<  " y: "  << y << "\n";
        if ( ! (( x_in | x_out_save ) == x_old0)){
          std::cerr << x_in << " " << x_out_save << "  " << x_old0 << "\n";
          assert( ( x_in | x_out_save ) == x_old0);
        }
        // std::cerr << x.is_empty() << " " << (x.max_diam()) << " " << prec << " " << y.is_empty() << " " << y.max_diam() << " " << x.max_diam() << " " << l.size()<< "\n";
        if (x.is_empty() || x.is_flat() || x.max_diam() < prec || y0.is_empty() || y.max_diam() < 0.1*x.max_diam()){
            x_res |= x_out_save;
            // std::cerr << "------> end y besection\n";
        } else {
          if (!y.is_empty() && !x_out_save.is_subset(x_res) ){
            try{
              TwoItv cut = bsc->bisect(y);
              l.push(TwoItv(x_out_save, cut.first));
              l.push(TwoItv(x_out_save, cut.second));
              // std::cout << "la \n";
            } catch (ibex::NoBisectableVariableException& e){
                std::cout << "Error while trying to bisect y\n";
                assert(false);

            }
          }
      }
    }
    //
    //     if ( !proceed4(x_in, x_save, x_res, cut.second, l))
    //
    //     try{
    //
    //         IntervalVector x_save(l.front().first);
    //         TwoItv cut = bsc->bisect(l.front().second);
    //
    //         l.pop();
    //         proceed4(x_in, x_save, x_res, cut.second, l);
    //
    //         if(x_in.is_empty() ) break;
    //         proceed4(x_in, x_save, x_res, cut.first, l);
    //
    //         k++;
    //     } catch (ibex::NoBisectableVariableException& e){
    //         l.pop();
    //     }
    // std::cerr << "###########################################################\n";
    x_out = x_res;

    x_in = x_in;

}


} // namespace pyibex
