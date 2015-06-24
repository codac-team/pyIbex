#include "vibes.h"
#include "ibex.h"
#include "ibex_SepProj.h"
#include "ibex_SepCtcPairProj.h"
#include "SIVIA.h"

using namespace ibex;
// Resultats
// Ancient alg
// nombre de bisetion 31327
// paving 341.748
// Nouveau
// nombre de bisetion 27935
// paving 260.055
//

int main(int argc, char ** argv){

   vibes::beginDrawing();

   double eps = 0.005;
   Interval a(-3, 2);
   cerr << pow(a,3) << "\n"; 
//   Function f("x", "y", "z", "x^2 + y^2 + z^2 - 1");
   Function f("x", "y", "z", "2*x^2 + 2*x*y + y^2 + z^2 - 1");
//   Function f("x", "y", "z", "2*x^2 + 2*x*y + y^2 + 2*x*z + z^2 - 1");

   SepFwdBwd sep(f, LEQ);
   IntervalVector y_init(1, Interval(1/std::sqrt(2), 2));
   SepProj sep1(sep, y_init, eps, 15);

   IntervalVector X(2,Interval(-10,10));
//   IntervalVector X(2,Interval(-0.10,0.10));

//   IntervalVector X(2);
//   X[0] = Interval(-0.425318, 0.27707);
//   X[1] = Interval(-0.173082, 0.281722);




   CtcFwdBwd ctcFdIn(f, GEQ);
   CtcFwdBwd ctcFdOut(f, LEQ);
//   Bitset<unsigned int, float> vars(BitSet::empty(3));
//   vars.add(true); vars.add(true); vars.add(false);

//   vars[0] = true; vars[1] = true; vars[2] = false;
//   CtcExist ctcOut(ctcFdIn,vars, y_init,eps);
//   CtcForAll ctcIn(ctcFdOut,vars, y_init,eps);

   SepCtcPairProj sep2(ctcFdIn, ctcFdOut, y_init, eps);
//   SiviaS(X, sep2, eps, "resOut2", "k[b]", "k[r]", "k[y]");
   SiviaS(X, sep1, eps, "resOut", "k[b]", "k[r]", "k[y]");
   vibes::endDrawing();
}
