// Author : Luc Jaulin
//---------------------------------------------------------------------------
#ifndef PavingH
#define PavingH
#include <vector>
#include <iostream>
#include <ibex_Interval.h>
#include <ibex_IntervalVector.h>
#include <ibex_BoolInterval.h>
#include <ibex_Pdc.h>
#include <ibex_LargestFirst.h>
#include <ibex_SetVisitor.h>

// using ibex::Interval;
// using ibex::IntervalVector;
// using ibex::BoolInterval;
// using ibex::MAYBE;
// using ibex::YES;
// using ibex::NO;
// using ibex::EMPTY;
// using ibex::Pdc;
using std::vector;
using std::ostream;
using std::cout;

namespace ibex {

    typedef BoolInterval(*BOOLEAN_OP)(const BoolInterval&,const BoolInterval&);
    typedef BoolInterval(*BOOLEAN_OP_UN)(const BoolInterval&);
class Paving
{
public:
    vector <IntervalVector> B;
    vector <int> left;
    vector <int> right;
    vector <BoolInterval> val;


    Paving(int dim) ;
    Paving(IntervalVector& v,BoolInterval b) ;
    Paving(IntervalVector&v, Pdc& pdc, double eps);

    void ctcOutside(IntervalVector &box, int k = 0);
    void ctcInside(IntervalVector &box, int k = 0);

    Paving& Expand(int);
    Paving& Remove_sons(int i); 
    Paving& Clean();
    Paving& Reunite(int i=0);

    BoolInterval contains(const IntervalVector& box);

    Paving& Sivia(Pdc& pdc,BOOLEAN_OP op,double eps);
    // Paving& Sivia(IntervalVector(*F)(const IntervalVector&),Paving& X,BoolInterval(*op)(const BoolInterval&, const BoolInterval&),double eps);

    Paving& Clear(BoolInterval);
    void visit(ibex::SetVisitor & visitor, int k=0);

    void change(BoolInterval src, BoolInterval dst, int k=0);

    friend ostream& operator<<(ostream&, const Paving&);

    //~Paving ();
    //Paving& operator=(const Paving&);
};

    void  op_binaire(Paving& Z,Paving& X,Paving& Y, BOOLEAN_OP op,int k=0, int i=0,int j=0) ;
    void  op_unaire(Paving& Z,Paving& X,BOOLEAN_OP_UN op,int k=0,int i=0);
    void Contract_distance_gt_Paving(Paving& Z, double z, IntervalVector& X, int k=0);
    void  Set_Contractor_Inter(Paving& X,Paving& Y,Paving& Z);
    void  Set_Contractor_Subset(Paving& X,Paving& Y);
    void  Set_Contractor_Disjoint(Paving& X,Paving& Y);
    void  Set_Contractor_Overlap(Paving& X,Paving& Y);
    BoolInterval Overlap(Paving& X,Paving& Y);
    Paving  Sqcap(Paving& X,Paving& Y);
    Paving  Complement(Paving& X);

} // end Namespace Ibex;
//----------------------------------------------------------
#endif
