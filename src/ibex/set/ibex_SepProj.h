//============================================================================
//                                  I B E X                                   
// File        : ibex_sepProj.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 04, 2015
//============================================================================

#ifndef __IBEX_SEP_PROJ_H__
#define __IBEX_SEP_PROJ_H__


#include <ibex_IntervalVector.h>
#include <ibex_Sep.h>
#include "ibex_SepFixPoint.h"
#include <ibex_LargestFirst.h>
#include <vector>
#include <queue>
#include <stack>
namespace ibex {

typedef std::pair<IntervalVector, IntervalVector> TwoItv;

typedef struct ImpactStatus_{
    bool impact_cin;
    bool impact_cout;
    IntervalVector *first_cin_boxes, *first_cout_boxes;
    int n_in, n_out;

    ImpactStatus_ () : impact_cin(false), impact_cout(false),
        first_cin_boxes(NULL), first_cout_boxes(NULL){
        n_in = 0; n_out = 0;
        clearFlags();
    }

//    ImpactStatus_(const ImpactStatus_& other):
//        impact_cin(other.impact_cin),
//        impact_cout(other.impact_cout),
//        n_in(other.n_in),
//        n_out(other.n_out) {

//        if (n_in > 0){
//            first_cin_boxes = new IntervalVector[n_in];
//            for (uint i = 0; i < n_in; i++){
//                first_cin_boxes[i] = new IntervalVector(other.first_cin_boxes[i]);
//            }
//        }
//        if (n_out > 0){
//            first_cout_boxes = new IntervalVector[n_out];
//            for (uint i = 0; i < n_out; i++){
//                first_cout_boxes[i] = new IntervalVector(other.first_cout_boxes[i]);
//            }
//        }
//    }



    ~ImpactStatus_(){
        clearFlags();
    }

    void clearFlags(){
        n_in = 0;
        n_out = 0;
        if (first_cin_boxes != NULL) delete[] first_cin_boxes;
        if (first_cout_boxes != NULL) delete[] first_cout_boxes;
        first_cin_boxes = NULL;
        first_cout_boxes = NULL;
        impact_cin = false;
        impact_cout = false;
    }


    bool setCinFlags(IntervalVector& x_in, IntervalVector& x0){
        if( !impact_cin ){
            if ( ! (x_in == x0)){
                impact_cin = true;
                n_in = x0.diff(x_in,first_cin_boxes); // calculate the set difference
            }
        }
    }

    bool setCoutFlags(IntervalVector& x_out, IntervalVector& x0){
        if(!impact_cout){
            if ( !(x_out == x0)){
                impact_cout = true;
                n_out = x0.diff(x_out,first_cout_boxes); // calculate the set difference
            }
        }
    }

    void reset_and_setCoutFlags(IntervalVector& x_out, IntervalVector& x0){
        impact_cout = false;
        if (first_cout_boxes != NULL){
            delete[] first_cout_boxes;
            first_cout_boxes = NULL;
        }
        setCoutFlags(x_out, x0);
    }

    void reconstrut_v2(IntervalVector &x_in, IntervalVector& x_out, IntervalVector& x_old){
        if (!x_in.is_empty() && !x_out.is_empty()){
            if (impact_cin == true){
                if (impact_cout == false){
                    x_out = x_old;
                } else {
                    for(int i = 0; i < n_in; i++){
                        x_out |= first_cin_boxes[i];
                    }
                }
            }
            if(impact_cout == true){
                if(impact_cin == false){
                    x_in  = x_old;
                } else {
                    for(int i = 0; i < n_out; i++){
                        x_in |= first_cout_boxes[i];
                    }
                }
            }
        } else if (x_in.is_empty() && impact_cin == true){
            if(impact_cout == false){
                x_out  = x_old;
            } else {
                for(int i = 0; i < n_in; i++){
                    x_out |= first_cin_boxes[i];
                }
            }
        } else if (x_out.is_empty() && impact_cout == true){
            if(impact_cin == false){
                x_in  = x_old;
            } else {
                for(int i = 0; i < n_out; i++){
                    x_in |= first_cout_boxes[i];
                }
            }
        } else {
            assert(false);
        }
    }


    void reconstrut(IntervalVector &x_in, IntervalVector& x_out, IntervalVector& x_old){
        IntervalVector x = x_in & x_out;
        IntervalVector *rest;

        if (x == x_old) return;
        if (impact_cin == true && impact_cout == false){
            x_out = x_old;
            x_in = x;
        } else if (impact_cin == false && impact_cout == true){
            x_in = x_old;
            x_out = x;
        } else if (impact_cin == true && impact_cout == true){
            int n = x_old.diff(x, rest);
//            for(uint i = 0; i < n; i++){
//                std::cout << "rest " << i << ": " << rest[i] << "\n" << std::flush;
//            }
//            for(uint i = 0; i < n_in; i++){
//                std::cout << "in " << i << ": " << first_cin_boxes[i] << "\n" << std::flush;
//            }
//            for(uint i = 0; i < n_out; i++){
//                std::cout << "out " << i << ": " << first_cout_boxes[i] << "\n" << std::flush;
//            }
    //        std::cout << rest[0] << " " << rest_in[0] << " " << rest_out[0] << std::endl;
            assert(n_in  == 1);
            assert(n_out == 1);
            assert(n     == 2);
            for (int i = 0; i < n; i++){
                for(int j = 0; j < n_in; j++){
                    if(first_cin_boxes[j].intersects(rest[i])){
                        x_out |= rest[i];
                        break;
                    }
                }
                for(int j = 0; j < n_out; j++){
                    if(first_cout_boxes[j].intersects(rest[i])){
                        x_in |= rest[i];
                        break;
                    }
                }
            }
            delete[] rest;
        } else {
            assert(false);
        }
    }

} ImpactStatus;

class SepProj : public Sep {

public:
    SepProj(Sep& sep, const IntervalVector& y_init, double prec, int nb_cut);
    ~SepProj();

    void separate(IntervalVector &x_in, IntervalVector &x_out);
    bool separate_fixPoint(IntervalVector& x_in, IntervalVector& x_out, IntervalVector &y);
    bool process(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &y, ImpactStatus &impact);
//    bool reconstrut(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &x_old);
//    void clearFlags();
//    bool setCoutFlags(IntervalVector &x_out, IntervalVector &x0);
//    bool setCinFlags(IntervalVector &x_in, IntervalVector &x0);
//    void reconstruct_with_sep(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &x_old, IntervalVector &y);
    IntervalVector separate_rec(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &y, ImpactStatus &impact);
protected:

    /**
     * \brief The Contractor.
     */
    Sep& sep;

    /**
     * vars[i]=true <=> the ith component is a variable ("x_k")
     * Otherwise, the ith component is a parameter ("y_k")
     */
//    std::vector<bool> vars;

    /**
     * \brief Initial box of the parameters (can be set dynamically)
     */
    IntervalVector y_init;

    /**
     * \brief precision
     */
    double prec;

    /**
      * \brief the outer contractor / inner contractor has an impact
      */
//    bool impact_cin, impact_cout;

    /**
      * \brief store the firsr box contractor by the inner/ outer contractor
      */
//    IntervalVector *first_cin_boxes, *first_cout_boxes;
//    int n_in, n_out;

    /**
     * \brief a bisector
     */
    LargestFirst* bsc;

    /**
      * Number of bisection along y
      */
    int nb_cut;

private:
    bool fixpoint(IntervalVector &x, IntervalVector &x_out_res, IntervalVector &y, ImpactStatus &impact);
    bool proceed2(IntervalVector& x, IntervalVector& x_res, IntervalVector& y, ImpactStatus& impact, std::stack<TwoItv> &l );

    bool fixpoint2(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &x_out_mid, IntervalVector &y, ImpactStatus &impact);
    bool proceed3(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &x0, IntervalVector &x_res, IntervalVector &y, std::stack<IntervalVector> &l);
    IntervalVector separate_rec2(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &y, ImpactStatus &impact);
    void proceed4(IntervalVector &x_in, IntervalVector &x_out, IntervalVector &x_res, IntervalVector &y, std::queue<TwoItv> &l);
};

} // namespace ibex

#endif // __IBEX_SEP_PROJ_H__

