
/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <malloc.h>
#include "matrixDivision.h"
#include "string.h"
#include "lapack.h"

void zldivma(	doubleComplex* in1, int lines1, int columns1 ,
				doubleComplex* in2, int lines2, int columns2 ,
				doubleComplex* out )
{

	
	char cNorm	= 0;
	int iExit	= 0;

	/*temporary variables*/
	int iWork		= 0;
	int iInfo		= 0;
	int iMax		= 0;
    int iRank		= 0;
    
	double dblRcond	= 0;

	double dblEps	= 0;
	double dblAnorm	= 0;

	doubleComplex *pAf		= NULL;
	doubleComplex *pXb		= NULL;
	doubleComplex *pDwork	= NULL;


	double *pRwork			= NULL;


	int *pIpiv				= NULL;
	int *pJpvt				= NULL;

	iWork	= max(2*columns2, min(lines2, columns2) + max(2 * min(lines2, columns2), max(lines2 + 1, min(lines2, columns2) + lines1)));



	/* Array allocations*/


	pAf			= (doubleComplex*)malloc(sizeof(doubleComplex) *(unsigned int) lines1 *(unsigned int)columns1);
	pXb			= (doubleComplex*)malloc(sizeof(doubleComplex) *(unsigned int) max(lines1,columns1) *(unsigned int) columns2);


	pIpiv		= (int*)malloc(sizeof(int) * (unsigned int)columns1);
	pJpvt		= (int*)malloc(sizeof(int) * (unsigned int)columns1);
	pRwork		= (double*)malloc(sizeof(double) * (unsigned int)columns1*2);


	cNorm		= '1';
	pDwork		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int)iWork);
	dblEps		=  getRelativeMachinePrecision() ;
	dblAnorm	= C2F(zlange)(&cNorm, &lines1, &columns1, in1, &lines1, pDwork);

	if(lines1 == columns1)
	{
		cNorm		= 'F';
		C2F(zlacpy)(&cNorm, &columns1, &columns1,	in1, &columns1, pAf, &columns1);

		C2F(zlacpy)(&cNorm, &columns1, &columns2,	in2, &columns1, pXb, &columns1);
		C2F(zgetrf)(&columns1, &columns1, pAf, &columns1, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(zgecon)(&cNorm, &columns1, pAf, &columns1, &dblAnorm, &dblRcond, pDwork, pRwork, &iInfo);
			if(dblRcond > sqrt(dblEps))
			{
				cNorm	= 'N';
				C2F(zgetrs)(&cNorm, &columns1, &columns2, pAf, &columns1, pIpiv, pXb, &columns1, &iInfo);
				cNorm	= 'F';
				C2F(zlacpy)(&cNorm, &columns1, &columns2, pXb, &columns1, out, &columns1);
				
				iExit = 1;
			}

		}
	}

	if(iExit == 0)
	{
		dblRcond = sqrt(dblEps);
		cNorm = 'F';
		iMax = max(lines1, columns1);
		C2F(zlacpy)(&cNorm, &lines1, &columns2, in2, &lines1, pXb, &iMax);
		memset(pJpvt, 0x00,(unsigned int) sizeof(int) * (unsigned int)columns1);
		C2F(zgelsy)(	&lines1, &columns1, &columns2, in1, &lines1, pXb, &iMax,
			pJpvt, &dblRcond, &iRank, pDwork, &iWork, pRwork, &iInfo);
		

		if(iInfo == 0)
		{

			cNorm = 'F';
			C2F(zlacpy)(&cNorm, &columns1, &columns2, pXb, &iMax, out, &columns1);
			
		}
	}





	free(pAf);
	free(pXb);
	free(pIpiv);
	free(pJpvt);
	free(pRwork);
	free(pDwork);

}
