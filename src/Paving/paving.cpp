// Author : Luc Jaulin
#include <list>
#include "paving.h"
using std::list;
using ibex::And;
using ibex::Or;
using ibex::Restrict;

//IntervalVector(*F_sivia) (const IntervalVector&);  // Global variable used in SIVIA;
//Paving X_sivia;

namespace ibex {
    
Paving::Paving(int dim)
{
    IntervalVector p(dim);
    B.clear(); left.clear();  right.clear();  val.clear();
    B.push_back(p);
    left.push_back(-1);
    right.push_back(-1);
    val.push_back(MAYBE);
};



Paving::Paving(IntervalVector& p,BoolInterval b)
{
    B.clear(); left.clear();  right.clear();  val.clear();
    B.push_back(p);
    left.push_back(-1);
    right.push_back(-1);
    val.push_back(b);
}

Paving::Paving(ibex::IntervalVector &v, ibex::Pdc &pdc, double eps)
{
    B.clear(); left.clear();  right.clear();  val.clear();
    B.push_back(v);
    left.push_back(-1);
    right.push_back(-1);
    val.push_back(YES);
    Sivia(pdc, And, eps);
    Reunite();
    Clean();

};

//    Paving::~Paving ();

//----------------------------------------------------------------------
ostream& operator<<(ostream& os, const Paving& X)
{
    cout << "Paving :"<<"\t dim="<< X.B.front().size() <<"\n";
    /*for (int i=0;i<X.B.size();i++)
       {
    os <<i<< "\t : left= "<<X.left[i]<<",\t right= "<<X.right[i]<<",\t val= "<<X.val[i]<<", \t X=["<< X.B[i][1]<<","<< X.B[i][2]<<"] \n";
       }*/
    return (os);
}
//----------------------------------------------------------------------
Paving& Paving::Clear(BoolInterval b)        // After Clear(), the Paving contains a single IntervalVector.
{
    IntervalVector P(B.front());
    B.clear();
    left.clear();
    right.clear();
    val.clear();
    B.push_back(P);
    left.push_back(-1);
    right.push_back(-1);
    val.push_back(b);
    return (*this);
}

void Paving::visit(ibex::SetVisitor &visitor, int k)
{
    if(k == 0){
        visitor.pre_visit();
    }
    visitor.visit_node(B[k]);
    if(left[k] == -1 || right[k] == -1){
        visitor.visit_leaf(B[k], val[k]);
    } else {
        visit(visitor, left[k]);
        visit(visitor, right[k]);
    }
    if(k == 0){
        visitor.post_visit();
    }
}

void Paving::change(ibex::BoolInterval src, ibex::BoolInterval dst, int k)
{
    if (left[k] == -1 || right[k] == -1){
        val[k] = (val[k] == src) ? dst : val[k];
    } else {
        change(src, dst, left[k]);
        change(src, dst, right[k]);
    }
}
//----------------------------------------------------------------------
Paving& Paving::Remove_sons(int i)
{
    if (i<0) return (*this);
    Remove_sons(left[i]);
    Remove_sons(right[i]);
    //if (left[i]!=-1)  val[left[i]]=EMPTY;
    //if (right[i]!=-1)  val[right[i]]=EMPTY;
    left[i]=-1;         right[i]=-1;
    return (*this);
}

//----------------------------------------------------------------------
Paving& Paving::Reunite(int i)
{
    if ((i<0)||(left[i]==-1)) return (*this);
    Reunite(left[i]);
    Reunite(right[i]);
    if (val[right[i]]==val[left[i]])
    { val[i]=val[left[i]];
        if ((left[left[i]]==-1)&&(left[right[i]]==-1)) Remove_sons(i);
    };
    if (i==0) Clean();
    return (*this);
}


//----------------------------------------------------------------------
Paving& Paving::Clean()    //garbage collector
{
    Paving X(*this);
    Clear(MAYBE);
    list<int> LX;
    list<int> LY;
    LX.push_back(0);
    LY.push_back(0);
    while (!LX.empty())
    { int i=LX.back();
        LX.pop_back();
        int j=LY.back();
        LY.pop_back();
        val[j]=X.val[i];
        B[j]=X.B[i];
        if (X.left[i]!=-1)
        { LX.push_back(X.right[i]);
            LX.push_back(X.left[i]);
            Expand(j);
            LY.push_back(right[j]);
            LY.push_back(left[j]);
        }
    };
    return (*this);
}
//----------------------------------------------------------------------
Paving& Paving::Expand(int i){  	
    if (left[i]==-1){
        ibex::LargestFirst lf;
        std::pair<IntervalVector, IntervalVector> bsc = lf.bisect(B[i]);
        IntervalVector &P1 = bsc.first;
        IntervalVector &P2 = bsc.second;
        int n=B.size();
        left[i]=n;              right[i]=n+1;
        left.push_back(-1);     right.push_back(-1);
        left.push_back(-1);     right.push_back(-1);
        val.push_back(val[i]);  val.push_back(val[i]);
        B.push_back(P1);        B.push_back(P2);
    }
    return (*this);
}


//----------------------------------------------------------------------
// BoolInterval  Inside(Paving& Z, const IntervalVector& X,int k=0)   // returns 0, if outside, 1 if inside, EMPTY if X is EMPTY, and MAYBE otherwize
// {
//     if (X.is_empty())
//         return ibex::EMPTY; // A cause de l'Union ci-dessous
// //    if (!X.is_subset(Z.B[k]))
// //        return MAYBE; // n'arrive jamais, sauf pour k=0
//     if (Z.val[k]!=MAYBE) return Z.val[k];
//     int kleft=Z.left[k];
//     int kright=Z.right[k];
//     if ((kleft!=-1)||(kright!=-1))
//     { IntervalVector Xleft =X & Z.B[kleft];
//         IntervalVector Xright=X & Z.B[kright];
//         BoolInterval I1= Inside(Z,Xleft,kleft);
//         BoolInterval I2= Inside(Z,Xright,kright);

//         return(ibex::Union(I1 ,I2));
//     }
//     return (MAYBE);
// }

//Afin de traiter un problème du type f-1(A) subset A, A in B
//faire un algo récursif du type  (A Verfier l'algo)
//Pour le Tester, inverser tout d'abord un pavage.
BoolInterval  Inside(Paving& Z, const IntervalVector& X,int k=0)   // returns 0, if outside, 1 if inside, empty if X is empty, and iperhaps otherwize
{  if (X.is_empty())
        return EMPTY; // A cause de l'Union ci-dessous
   if (!X.is_subset(Z.B[k]))
      return MAYBE; // n'arrive jamais, sauf pour k=0
   if (Z.val[k]!=MAYBE) return Z.val[k];
   int kleft=Z.left[k];
   int kright=Z.right[k];
   if ((kleft!=-1)||(kright!=-1))
   { IntervalVector Xleft =X & Z.B[kleft];
     IntervalVector Xright=X & Z.B[kright];
     BoolInterval I1= Inside(Z,Xleft,kleft);
     BoolInterval I2= Inside(Z,Xright,kright);

     return(Union(I1,I2));
   }
   return (MAYBE);
}

//----------------------------------------------------------------------
ibex::BoolInterval Paving::contains(const ibex::IntervalVector &box)
{
    return Inside(*this, box, 0);
}
//----------------------------------------------------------------------
// void Contract_distance_gt_Paving(Paving& A, double z, IntervalVector& X,int k) //Contract X with respect to distance to a subPaving lower than z.
// {  if (X.IsEMPTY())  return;
//    if (A.val[k]==NO)
//        {X=EMPTYBox(X);return;}                  // NE MARCHE PAS ! A REFAIRE !
//    int kleft=A.left[k];
//    int kright=A.right[k];
//    if (((A.val[k]==MAYBE)&&(kleft==-1)&&(kright==-1))||(A.val[k]==YES))
//    {  interval X1=X[1];
//       interval X2=X[2];
//       interval A1=(A.B[k])[1];
//       interval A2=(A.B[k])[2];
//       interval Z(0,z*z);    //X1^2+X2^2=Z
//       interval D1=X1-A1;
//       interval D2=X2-A2;
//       interval D12=Sqr(D1);
//       interval D22=Sqr(D2);
//       Cplus(Z,D12,D22,1);
//       Cplus(Z,D12,D22,-1);
//       Csqr(D22,D2,-1);
//       Csqr(D12,D1,-1);
//       Cmoins(D2,X2,A2,-1);
//       Cmoins(D1,X1,A1,-1);
//       X=IntervalVector(X1,X2);
//       return;
//    }
//      IntervalVector X1(X);
//      IntervalVector X2(X);
//      Contract_distance_gt_Paving(A,z,X1,kleft);
//      Contract_distance_gt_Paving(A,z,X2,kright);
//      X=Union(X1,X2);
//      return;
// }
//----------------------------------------------------------------------
// Operation between Pavings
void  op_binaire(Paving& Z, Paving& X,Paving& Y, BOOLEAN_OP op,int k,int i,int j)
{
    if (k==0) {Z.B[0]=X.B[0]; Z.Clear(MAYBE);};
    BoolInterval vali = op(X.val[i],Y.val[j]);
    Z.val[k]=vali;
    int ileft=X.left[i];
    int jleft=Y.left[j];
    int iright=X.right[i];
    int jright=Y.right[j];
    if (((ileft!=-1)||(jleft!=-1))&&(vali==MAYBE))
    { if (ileft==-1) {ileft=i; iright=i;}
        if (jleft==-1) {jleft=j; jright=j;}
        Z.Expand(k);
        op_binaire(Z,X,Y,op,Z.left[k],ileft,jleft);
        op_binaire(Z,X,Y,op,Z.right[k],iright,jright);
    }
   if (k==0) {Z.Clean();Z.Reunite();};
    return;
}
//----------------------------------------------------------------------
// Unary operation between Pavings
void  op_unaire(Paving& Z, Paving& X, BOOLEAN_OP_UN op,int k,int i)
{
    if (k==0) {Z.B[0]=X.B[0]; Z.Clear(MAYBE);};
    BoolInterval vali = op(X.val[i]);
    Z.val[k]=vali;
    int ileft=X.left[i];
    int iright=X.right[i];
    if ((ileft!=-1)&&(vali==MAYBE))
    { Z.Expand(k);
        op_unaire(Z,X,op,Z.left[k],ileft);
        op_unaire(Z,X,op,Z.right[k],iright);
    }
   if (k==0) {Z.Clean();Z.Reunite();};
    return;
}


//----------------------------------------------------------------------
Paving& Paving::Sivia(ibex::Pdc &pdc, BOOLEAN_OP op, double eps)
{
    list<int> L;
    L.push_back(0);
    while (!L.empty())
    { int i=L.front();    L.pop_front();
        BoolInterval testBi=pdc.test(B[i]);
        BoolInterval vali = op(val[i],testBi);
        if (vali!=MAYBE)
        { Remove_sons(i);  }
        else
            if  ((B[i].max_diam()>eps) && (testBi==MAYBE))    //Luc
            {  Expand(i);
                L.push_back(left[i]);
                L.push_back(right[i]);
            }
        val[i]=vali;
    };
    Clean();
   Reunite();
    return (*this);
}

//BoolInterval Test_sivia(const IntervalVector& P)     //used only in Sivia below
//{   IntervalVector Y=F_sivia(P);
//    return (Inside(X_sivia,Y,0));
//}

//----------------------------------------------------------------------
//Paving& Paving::Sivia(IntervalVector(*F)(const IntervalVector&),Paving& X,BoolInterval(*op)(BoolInterval,BoolInterval),double eps)
//{  X_sivia=X;
//    F_sivia=F;
//    Sivia(Test_sivia,op,eps);
//}





//----------------------------------------------------------------------
Paving  Sqcap(Paving& X,Paving& Y)
{
    Paving Z(X.B[0].size());
    op_binaire(Z,X,Y,ibex::Inter);
    return (Z);
}
//----------------------------------------------------------------------
void  Set_Contractor_Inter(Paving& Z,Paving& X,Paving& Y)   //project with respect to Z = X inter Y
{    //op_binaire(Z,X,Y,Inter);  //Z=X inter Y

    //D= Paving(X.B[0]);
    //op_binaire(Z,X,Y,Restrict);  //Z=X inter Y


    //Paving Z1=Z;
    //     op_binaire(D,0,X,0,Y,0,0);     // and
    //     op_binaire(Z,0,Z1, 0,D,0,2);   // intersect
    //A FINIR
}

//----------------------------------------------------------------------
void  Set_Contractor_Overlap(Paving& X,Paving& Y)   //project with respect to X and Y overlap
{
    Paving XinterY(X.B[0].size());
    op_binaire(XinterY,X,Y,And);
    if (XinterY.val[0]==NO)
    {  X.Clear(EMPTY);  Y.Clear(EMPTY);}
}
//----------------------------------------------------------------------
BoolInterval Overlap(Paving& X,Paving& Y)   //returns if  X and Y overlap
{
    Paving XinterY(X.B[0].size());
    op_binaire(XinterY,X,Y,And);
    if (XinterY.val[0]==NO) return NO;
    for (int k=0;k<XinterY.B.size();k++)
        if (XinterY.val[0]==YES) return YES;
    return MAYBE;
}

//----------------------------------------------------------------------
void  Set_Contractor_Subset(Paving& X,Paving& Y)   //project with respect to X \subset Y
{
    Paving Z(X.B[0].size());
    // Solution 1
    //op_binaire(Z,X,Y,leq);  //X=X inter Y
    //X=Z;
    //op_binaire(Z,Y,X,geq);  //Y=X union Y
    //Y=Z ;
    //  ------
    // Solution 2
    op_binaire(Z,X,Y,And);  //X=X inter Y
    X=Sqcap(X,Z);
    op_binaire(Z,Y,X,Or);  //Y=X union Y
    Y=Sqcap(Y,Z);
}
//----------------------------------------------------------------------
void  Set_Contractor_Disjoint(Paving& X,Paving& Y)   //project with respect to X inter Y = EMPTY
{
    Paving All(X.B[0],MAYBE);
    Paving Z1(X.B[0].size());
    op_binaire(Z1,All,Y,Restrict);
    Paving Z2(X.B[0].size());
    Z2=Sqcap(X,Z1);

    Paving Z3(X.B[0].size());
    op_binaire(Z3,All,X,Restrict);
    Paving Z4(X.B[0].size());
    Z4=Sqcap(Y,Z3);

    X=Z2; Y=Z4;
}

} // end namespace ibex