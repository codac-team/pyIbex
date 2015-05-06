#include <ibex_IntervalVector.h>
#include <ibex_LargestFirst.h>
#include <ibex_Ctc.h>
#include <ibex_Sep.h>
#include <ibex_EmptyBoxException.h>
#include <stack>
#include <string>
#include "vibes.h"

using namespace std;
using namespace ibex;

namespace ibex {

void drawBoxDiff(IntervalVector &X0, IntervalVector& X, const string& color);
void contract_and_draw(Ctc& c, IntervalVector& X, const string& color);
void separate_and_draw(Sep &sep,IntervalVector &Xin, IntervalVector &Xout,
                              const string& colorIn, const string& colorOut);

int SiviaC(IntervalVector& X0, Ctc& ctc, double eps, std::string figureName, string fillOut, string fillBorder );
void SiviaS(IntervalVector& X0, Sep& sep, double eps, std::string figureName,
               string fillOut, string fillIn, string fillBorder);



};

