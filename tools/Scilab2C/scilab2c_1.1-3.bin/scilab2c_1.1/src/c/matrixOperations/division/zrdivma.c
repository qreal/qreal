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

#include <string.h>

void	zrdivma(	doubleComplex* in1, int lines1, int columns1 ,
				doubleComplex* in2, int lines2, int columns2 ,
				doubleComplex* out )
{

	char cNorm	= 0;
	int iExit	= 0;

	/*temporary variables*/
	int iWork		= 0;
	int iInfo		= 0;
	int iMax		= 0;
	double dblRcond	= 0;

	double dblEps	= 0;
	double dblAnorm	= 0;


	doubleComplex *poAf		= NULL;
	doubleComplex *poAt		= NULL;
	doubleComplex *poBt		= NULL;
	doubleComplex *poDwork	= NULL;

	int *pRank	= NULL;
	int *pIpiv	= NULL;
	int *pJpvt	= NULL;
	double *pRwork	= NULL;

	iWork	= max(2*columns2, min(lines2, columns2) + max(2 * min(lines2, columns2), max(lines2 + 1, min(lines2, columns2) + lines1)));



	/* Array allocations*/
	
	
	poAf		= (doubleComplex*)malloc(sizeof(doubleComplex) *(unsigned int) lines2 *(unsigned int) columns2);
	poAt		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int)lines2 *(unsigned int) columns2);
	poBt		= (doubleComplex*)malloc(sizeof(doubleComplex) *(unsigned int) max(lines2, columns2) *(unsigned int) lines1);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) *(unsigned int) columns2);
	pJpvt		= (int*)malloc(sizeof(int) *(unsigned int) lines2);
	pRwork		= (double*)malloc(sizeof(double) * 2 *(unsigned int) lines2);



	cNorm		= '1';
	poDwork		= (doubleComplex*)malloc(sizeof(doubleComplex) *(unsigned int) iWork);
	dblEps		= getRelativeMachinePrecision() ;
	dblAnorm	= C2F(zlange)(&cNorm, &lines2, &columns2, in2, &lines2, poDwork);

	/* hermitian tranpose A   (transpose + conjugate )*/

	ztransposea(in2, lines2, columns2, poAt);
    zconja ( poAt , lines2*columns2 , poAt );
		{
		int i,j,ij,ji;
		for(j = 0 ; j < lines1 ; j++)
		{
			for(i = 0 ; i < columns2 ; i++)
			{
				ij = i + j * max(lines2, columns2);
				ji = j + i * lines1;
                
                poBt[ij] = DoubleComplex (zreals ( in1[ji] ) , - zimags ( in1[ji] ) );
			}
		}
	}

	if(lines2 == columns2)
	{
		cNorm		= 'F';
		C2F(zlacpy)(&cNorm, &columns2, &columns2, poAt, &columns1, poAf, &columns2);
		C2F(zgetrf)(&columns2, &columns2, poAf, &columns2, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(zgecon)(&cNorm, &columns2, poAf, &columns2, &dblAnorm,
						&dblRcond, poDwork, pRwork, &iInfo);
			if(dblRcond > sqrt(dblEps))
			{
				cNorm	= 'N';
				C2F(zgetrs)(&cNorm, &columns2, &lines1, poAf, &columns2, pIpiv, poBt, &columns2, &iInfo);
				{
					int i,j,ij,ji;
					for(j = 0 ; j < lines2 ; j++)
					{
						for(i = 0 ; i < lines1 ; i++)
						{
							ij = i + j * lines1;
							ji = j + i * max(lines2, columns2);
							out[ij] 	=  DoubleComplex (  zreals( poBt[ji]) , -zimags ( poBt[ji]));

		             		}
		             	}
                   	}
				
				iExit = 1;
			}
		}

	}

	if(iExit == 0)
	{
		dblRcond = sqrt(dblEps);
		cNorm = 'F';
		iMax = max(lines2, columns2);
		
		memset(pJpvt, 0x00,(unsigned int) sizeof(int) *(unsigned int) lines2);
		C2F(zgelsy)(&columns2, &lines2, &lines1, poAt, &columns2, poBt, &iMax,
			pJpvt, &dblRcond, &pRank[0], poDwork, &iWork, pRwork, &iInfo);

		if(iInfo == 0)
		{

		/*//	TransposeRealMatrix(pBt, lines1, lines2, _pdblRealOut, Max(lines1,columns1), lines2);

			//Mega caca de la mort qui tue des ours a mains nues 
			//mais je ne sais pas comment le rendre "beau" :(*/
			{
				int i,j,ij,ji;
				for(j = 0 ; j < lines2 ; j++)
				{
					for(i = 0 ; i < lines1 ; i++)
					{
						ij = i + j * lines1;
						ji = j + i * max(lines2, columns2);
						out[ij] 	=  DoubleComplex (  zreals( poBt[ji]) , -zimags ( poBt[ji]));
		                  }
				}
			}
		}
	}



	free(poAf);
	free(poAt);
	free(poBt);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pRwork);
	free(poDwork);

}


