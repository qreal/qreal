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
#include "lapack.h" 

#include <string.h>

void dldivma (double* in1, int lines1, int columns1 ,
				double* in2, int lines2, int columns2 ,
				double* out ){


	char cNorm	= 0;
	int iExit	= 0;

	/*temporary variables*/
	int iWork		= 0;
	int iInfo		= 0;
	int iMax		= 0;
	double dblRcond	= 0;

	double dblEps	= 0;
	double dblAnorm	= 0;

	double *pAf		= NULL;
	double *pXb		= NULL;
	double *pDwork	= NULL;

	int *pRank	= NULL;
	int *pIpiv	= NULL;
	int *pJpvt	= NULL;
	int *pIwork	= NULL;

	iWork	= max(4 * columns1, max(min(lines1, columns1) + 3 * lines1 + 1, 2 * min(lines1, columns1) + columns2));


        lines2 = 0 ;

	/* Array allocations*/
	pAf			= (double*)malloc(sizeof(double) * (unsigned int) lines1 * (unsigned int) columns1);
	pXb			= (double*)malloc(sizeof(double) * (unsigned int) max(lines1,columns1) * (unsigned int) columns2);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) *(unsigned int) columns1);
	pJpvt		= (int*)malloc(sizeof(int) *(unsigned int) columns1);
	pIwork		= (int*)malloc(sizeof(int) *(unsigned int) columns1);
	


	cNorm		= '1';
	pDwork		= (double*)malloc(sizeof(double) *(unsigned int)iWork);
	dblEps		= getRelativeMachinePrecision() ;
	
	dblAnorm	= dlange_(&cNorm, &lines1, &columns1, in1, &lines1, pDwork);
	if(lines1 == columns1)
	{
		cNorm		= 'F';
		dlacpy_(&cNorm, &columns1, &columns1, in1, &columns1, pAf, &columns1);
		dgetrf_(&columns1, &columns1, pAf, &columns1, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(dgecon)(&cNorm, &columns1, pAf, &columns1, &dblAnorm, &dblRcond, pDwork, pIwork, &iInfo);
			if(dblRcond > sqrt(dblEps))
			{
				cNorm	= 'N';
				C2F(dgetrs)(&cNorm, &columns1, &columns2, pAf, &columns1, pIpiv, in2, &columns1, &iInfo);
				cNorm	= 'F';
				C2F(dlacpy)(&cNorm, &columns1, &columns2, in2, &columns1, out, &columns1);
				iExit = 1;
			}
		}

	}

	if(iExit == 0)
	{
		dblRcond = sqrt(dblEps);
		cNorm = 'F';
		iMax = max(lines1, columns1);
		C2F(dlacpy)(&cNorm, &lines1, &columns2, in2, &lines1, pXb, &iMax);
		memset(pJpvt, 0x00,(unsigned int) sizeof(int) * (unsigned int) columns1);
		C2F(dgelsy)(	&lines1, &columns1, &columns2, in1, &lines1, pXb, &iMax,
				pJpvt, &dblRcond, &pRank[0], pDwork, &iWork, &iInfo);

		if(iInfo == 0)
		{

			cNorm = 'F';
			C2F(dlacpy)(&cNorm, &columns1, &columns2, pXb, &iMax, out, &columns1);
		}
	}

	free(pAf);
	free(pXb);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pIwork);
	free(pDwork);
}
