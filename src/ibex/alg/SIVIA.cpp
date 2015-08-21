//============================================================================
//                                P Y I B E X
// File        : SIVIA.cpp
// Author      : Benoit Desrochers
// Copyright   : Ensta Bretagne
// License     : See the LICENSE file
// Created     : Dec 28, 2014
//============================================================================

#include "SIVIA.h"
#include <sys/time.h>
#include <iostream>
using namespace std;
using namespace ibex;

namespace ibex {

void drawBoxDiff(IntervalVector &X0, IntervalVector& X, const string& color) {
    if (X==X0) return;     // nothing contracted.
    IntervalVector* rest;
    int n=X0.diff(X,rest); // calculate the set difference
    for (int i=0; i<n; i++) {     // display the boxes
        vibes::drawBox(rest[i], color);
    }
    delete[] rest;
}

void contract_and_draw(Ctc& c, IntervalVector& X, const string& color) {
    IntervalVector X0=X;       // get a copy
    c.contract(X);
    if(!X.is_empty()){
        drawBoxDiff(X0, X, color);
    } else {
        vibes::drawBox(X0, color);
    }
}

void separate_and_draw(Sep &sep,IntervalVector &Xin, IntervalVector &Xout,
                              const string& colorIn, const string& colorOut){
    IntervalVector box = Xin & Xout;
    
    sep.separate(Xin,Xout);
    drawBoxDiff(box,Xout,colorOut);
    drawBoxDiff(box,Xin,colorIn);
    
}

int SiviaC(IntervalVector& X0, Ctc& ctc, double eps, std::string figureName, string fillOut, string fillBorder ){

    vibes::beginDrawing();
    vibes::newFigure(figureName);
	LargestFirst lf(eps);
    stack<IntervalVector> s;
    int k = 0;
    s.push(X0);
    while (!s.empty()) {
        k++;
        IntervalVector box=s.top();
        s.pop();
        contract_and_draw(ctc, box, fillOut);
        
        if (box.is_empty()) { continue; }

        if (box.max_diam()<eps) {
            vibes::drawBox(box, fillBorder);
        } else {
            pair<IntervalVector,IntervalVector> boxes=lf.bisect(box);
            s.push(boxes.first);
            s.push(boxes.second);
        }
    }

    cerr << "Test " << k << " boxes\n";
    vibes::endDrawing();
}



void SiviaS(IntervalVector& X0, Sep& sep, double eps, std::string figureName,
               string fillOut, string fillIn, string fillBorder ){

    struct timeval tbegin,tend;
    double texec=0.;
    gettimeofday(&tbegin, NULL);

    vibes::beginDrawing();
    vibes::newFigure(figureName);
    LargestFirst lf(eps);
    stack<IntervalVector> s;
    int k = 0;
    s.push(X0);    
    while (!s.empty()) {
        IntervalVector box=s.top();
        s.pop();
        IntervalVector Xin(box), Xout(box);
        separate_and_draw(sep,Xin,Xout, fillIn, fillOut);
        k++;

        box = Xin & Xout;
            if (box.is_empty()) { continue; }

            if (box.max_diam()<eps) {
               vibes::drawBox(box, fillBorder);
            } else {
                pair<IntervalVector,IntervalVector> boxes=lf.bisect(box);
                s.push(boxes.first);
                s.push(boxes.second);
            }
    }
    gettimeofday(&tend, NULL);
    std::cerr << "nombre de bisetion " << k << "\n";
    texec=((double)(1000*(tend.tv_sec-tbegin.tv_sec)+((tend.tv_usec-tbegin.tv_usec)/1000)))/1000.;
    std::cerr << "paving " << texec << std::endl;
}

}
