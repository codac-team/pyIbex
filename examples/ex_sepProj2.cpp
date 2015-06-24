#include "vibes.h"
#include "ibex.h"
#include "ibex_SepProj.h"
#include "SIVIA.h"

using namespace ibex;
//nombre de bisetion 923
//paving 12.121
//nombre de bisetion 933
//paving 35.669

int main(int argc, char ** argv){

   vibes::beginDrawing();

   double eps = 0.005;
   Function f("p1", "p2", "t", "20*exp(-p1*t)-8*exp(-p2*t)");

   vector<Interval> Y;

    vector<IntervalVector> T;
   Interval y1(2.7,12.1); Y.push_back(y1);
   Interval y2(1.04,7.14); Y.push_back(y2);
   Interval y3(-0.13,3.61); Y.push_back(y3);
   Interval y4(-0.95,1.15); Y.push_back(y4);
   Interval y5(-4.85,-0.29); Y.push_back(y5);
   Interval y6(-5.06,-0.36); Y.push_back(y6);
   Interval y7(-4.1,-0.04); Y.push_back(y7);
   Interval y8(-3.16,0.3); Y.push_back(y8);
   Interval y9(-2.5,0.51); Y.push_back(y9);
   Interval y10(-2,0.67); Y.push_back(y10);

   IntervalVector t1(1,Interval(-0.25,1.75));   T.push_back(t1);
   IntervalVector t2(1,Interval(0.5,2.5));      T.push_back(t2);
   IntervalVector t3(1,Interval(1.25,3.25));    T.push_back(t3);
   IntervalVector t4(1,Interval(2,4));          T.push_back(t4);
   IntervalVector t5(1,Interval(5,7));          T.push_back(t5);
   IntervalVector t6(1,Interval(8,10));         T.push_back(t6);
   IntervalVector t7(1,Interval(12,14));        T.push_back(t7);
   IntervalVector t8(1,Interval(16,18));        T.push_back(t8);
   IntervalVector t9(1,Interval(20,22));        T.push_back(t9);
   IntervalVector t10(1,Interval(24,26));       T.push_back(t10);

   Array<Sep> arr_sp(T.size());
   for(int i = 0; i < T.size(); i++){
       SepFwdBwd *sep = new SepFwdBwd(f,Y[i]);
       SepProj *sp = new SepProj(*sep, T[i], eps, 120);
        arr_sp.set_ref(i, *sp);
   }
   SepInter Sproj(arr_sp);


   IntervalVector X(2,Interval(0,1.2));
   X[1] = Interval(0,0.5);

//   IntervalVector X(2,Interval(0,1.2));
//   X[0] = Interval(0.279431,0.398907);
//   X[1] = Interval(0.10125,0.156938);

//   X[0] = Interval(0.919063, 1);
//   X[1] = Interval(0.342263, 0.378);

//   X[0] = Interval(0.97, 0.98);
//   X[1] = Interval(0.358, 0.365);

//   IntervalVector X(2);
//   X[0] = Interval(-0.0707107, 0.279307);
//   X[1] = Interval(-0.0707107, 1.646966);


   SiviaS(X, Sproj, eps, "resOut", "k[b]", "k[r]", "k[y]");


   Bitset<unsigned int, float> vars(BitSet::empty(3));
   vars.add(true); vars.add(true); vars.add(false);
   Array<Sep> arr_sp2(T.size());
   for(int i = 0; i < T.size(); i++){
       CtcFwdBwd *fbOut = new CtcFwdBwd(f, Y[i]);
       CtcNotIn *fbIn = new CtcNotIn(f, Y[i]);

       CtcForAll  *ctcIn = new CtcForAll(*fbIn, vars, T[i], eps);
       CtcExist *ctcOut = new CtcExist(*fbOut, vars, T[i], eps);

       SepCtcPair *sp = new SepCtcPair(*ctcIn, *ctcOut);
       arr_sp2.set_ref(i, *sp);
   }
   SepInter Sproj2(arr_sp2);

   SiviaS(X, Sproj2, eps, "resOut2", "k[b]", "k[r]", "k[y]");
   vibes::endDrawing();
}
