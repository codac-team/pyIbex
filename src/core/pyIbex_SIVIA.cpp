// #include <ibex_IntervalVector.h>
// #include <ibex_LargestFirst.h>
// #include <ibex_Ctc.h>
// #include <ibex_Sep.h>
// #include <stack>
// #include <string>
// #include "vibes.h"

#include "SIVIA.h"
#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace ibex;

// namespace ibex {

// void drawBoxDiff(IntervalVector &X0, IntervalVector& X, const string& color) {
//     if (X==X0) return;     // nothing contracted.
//     IntervalVector* rest;
//     int n=X0.diff(X,rest); // calculate the set difference
//     for (int i=0; i<n; i++) {     // display the boxes
//         vibes::drawBox(rest[i], color);
//     }
//     delete[] rest;
// }
// void contract_and_draw(Ctc& c, IntervalVector& X, const string& color) {
//     IntervalVector X0=X;       // get a copy
//     try {
//         c.contract(X);
//         drawBoxDiff(X0, X, color);
//     } catch(EmptyBoxException&) {
//         vibes::drawBox(X0, color);
//     }
// }

// void separate_and_draw(Sep &sep,IntervalVector &Xin, IntervalVector &Xout,
//                               const string& colorIn, const string& colorOut){
//     IntervalVector box = Xin & Xout;
//     try {
//         sep.separate(Xin,Xout);
//         drawBoxDiff(box,Xin,colorIn);
//         drawBoxDiff(box,Xout,colorOut);
//     } catch(EmptyBoxException&) {

//     }
// }


// int SiviaC(IntervalVector& X0, Ctc& ctc, double eps, std::string figureName, string fillOut, string fillBorder ){

//     vibes::beginDrawing();
//     vibes::newFigure(figureName);
//     LargestFirst lf(eps);
//     stack<IntervalVector> s;
//     int k = 0;
//     s.push(X0);
//     while (!s.empty()) {
//         k++;
//         IntervalVector box=s.top();
//         s.pop();
//         try{
//             contract_and_draw(ctc, box, fillOut);
//         } catch(EmptyBoxException &e) {}
//         if (box.is_empty()) { continue; }

//         if (box.max_diam()<eps) {
//             vibes::drawBox(box, fillBorder);
//         } else {
//             pair<IntervalVector,IntervalVector> boxes=lf.bisect(box);
//             s.push(boxes.first);
//             s.push(boxes.second);
//         }
//     }
//     cerr << "Test " << k << " boxes\n";
//     vibes::endDrawing();
// }



// void SiviaS(IntervalVector& X0, Sep& sep, double eps, std::string figureName,
//                string fillOut, string fillIn, string fillBorder ){
//     vibes::beginDrawing();
//     vibes::newFigure(figureName);
//     LargestFirst lf(eps);
//     stack<IntervalVector> s;
//     int k = 0;
//     s.push(X0);
//     while (!s.empty()) {
//         IntervalVector box=s.top();
//         s.pop();
//         IntervalVector Xin(box), Xout(box);
//         separate_and_draw(sep,Xin,Xout, fillIn, fillOut);
//         k++;

//         box = Xin & Xout;
//             if (box.is_empty()) { continue; }

//             if (box.max_diam()<eps) {
//                vibes::drawBox(box, fillBorder);
//             } else {
//                 pair<IntervalVector,IntervalVector> boxes=lf.bisect(box);
//                 s.push(boxes.first);
//                 s.push(boxes.second);
//             }
// //            if (k == 1) break;

//     }

//     std::cerr << "nombre de bisetion " << k;
// }

// }

namespace python = boost::python;
void export_algorithms(){
    
    def("SIVIA", &ibex::SiviaC, (python::arg("X0"), python::arg("ctc"), python::arg("eps")=0.1,
                                python::arg("figureName")=string("default"),
                                python::arg("fillOut")=string("[b]"),
                                python::arg("fillBorder")=string("[y]"))
        );
    def("SIVIA", &ibex::SiviaS, (python::arg("X0"), python::arg("Sep"), python::arg("eps")=0.1,
                                    python::arg("figureName")=string("default"),
                                    python::arg("fillOut")=string("[b]"),
                                    python::arg("fillIn")=string("[r]"),
                                    python::arg("fillBorder")=string("[y]"))
        );

}

