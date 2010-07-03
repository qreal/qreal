#include <QtCore/QCoreApplication>
#include "api_scilab.h"
#include "stack-c.h"
#include "call_scilab.h"
#include "MatrixWork.cpp"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    

    MatrixWork mw;
    
    static double A[]={1,2,3,4};  int mA=2,nA=2;
    static double B[]={4,5,6,7};  int mB=2,nB=2;
    
    mw.StartWork();
    mw.GetElementSum(A, nA, mA);
    mw.GetMatrixSum(A, nA, mA, B, nB, mB);
    
    return a.exec();

}
