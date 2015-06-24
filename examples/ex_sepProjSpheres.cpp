#include "vibes.h"
#include "ibex.h"
#include "ibex_SepProj.h"
#include "ibex_SepQInter.h"
#include "ibex_SepCtcPairProj.h"
#include "SIVIA.h"

using namespace ibex;


int main(int argc, char ** argv){

   vibes::beginDrawing();

   double eps = 0.05;

   double theta[] = {0, 2*M_PI/3.0, -2*M_PI/3.0};
   double radius = 1;
   double R = 1.5;
   char buf[256];
   Array<Sep> sep_array(3);
   for (int i = 0;i < 3; i++){
       sprintf(buf, "(x-%f)^2+ (y-%f)^2 + z^2 - %f",radius*std::cos(theta[i]),
               radius*std::sin(theta[i]),
               R*R);
       Function *f1 = new Function("x","y","z",buf);
       SepFwdBwd *sfd = new SepFwdBwd(*f1, LEQ);
       sep_array.set_ref(i, *sfd);
   }

//   SepInter sep(sep_array);
   SepQInterProjF sepQ(sep_array);
   sepQ.setq(2);
   IntervalVector y_init(1, Interval(-20,20));
   SepProj sep(sepQ, y_init, eps, 40);

   IntervalVector X(2, Interval(-20,20));
   SiviaS(X, sep, eps, "resOut", "k[b]", "k[r]", "y");
   for (int i = 0;i < 3; i++){
        vibes::drawCircle(radius*std::cos(theta[i]), radius*std::sin(theta[i]), R);
   }
   vibes::endDrawing();
}
