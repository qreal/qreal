#include "MatrixWork.h"

bool MatrixWork::StartWork()
{
#ifdef _MSC_VER
    if ( StartScilab(NULL,NULL,NULL) == FALSE )
#else
        if ( StartScilab(getenv("SCI") ,NULL,NULL) == FALSE )
#endif
            return 0;
}

int MatrixWork::GetElementSum(double A[], int An, int Am)
{
    WriteMatrix("A", &An, &Am, A);
    SendScilabJob("disp('A=');");
    SendScilabJob("disp(A);");
    SendScilabJob("disp('elements summ:');");
    SendScilabJob("x = sum(A)");
    SendScilabJob("disp(x);");
}

int MatrixWork::GetMatrixSum(double A[], int An, int Am, double B[], int Bn, int Bm)
{
    WriteMatrix("A", &An, &Am, A);
    WriteMatrix("B", &Bn, &Bm, B);
    SendScilabJob("disp('A=');");
    SendScilabJob("disp(A);");
    SendScilabJob("disp('B=');");
    SendScilabJob("disp(B);");
    SendScilabJob("disp('A + B');");
    SendScilabJob("x = A+B");

    SendScilabJob("disp(x);");
}
