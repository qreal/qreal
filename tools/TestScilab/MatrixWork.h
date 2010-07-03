#ifndef MATRIXWORK_H
#define MATRIXWORK_H

#endif // MATRIXWORK_H
#include "api_scilab.h"
#include "stack-c.h"
#include "call_scilab.h"

class MatrixWork
{
    public:
        bool StartWork();
        int GetElementSum(double A[], int An, int Am);
        int GetMatrixSum(double A[], int An, int Am, double B[], int Bn, int Bm);
};
