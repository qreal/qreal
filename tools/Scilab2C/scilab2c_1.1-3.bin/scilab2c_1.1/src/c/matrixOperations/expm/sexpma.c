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

#include "matrixExponential.h"

void sexpma (float* in, float* out, int _iLeadDim){

    int iIndex1     = 0;
	int iMax		= 0;
	int iFlag		= 0;
	int iLoop1		= 0;
	int iSquare		= 0;


	float fltExp	= 0;
	float fltS		= 0;
	float fltS2	        = 0;
	float fltCst	= 0.5f;
    double dblCst  = 0.5;

	float *pfltMatrixA		= NULL;/*A'*/
	float *pfltMatrixX		= NULL;/*X*/
	float *pfltMatrixD		= NULL;/*D*/
	float *pfltMatrixcX	= NULL;/*cX*/
	float *pfltMatrixcA	= NULL;/*cX*/
	float *pfltMatrixEye	= NULL;/*Eye*/
	float *pfltMatrixTemp	= NULL;/*Temp*/
	float *pfltMatrixTemp2	= NULL;/*Temp2*/



	iSquare = _iLeadDim * _iLeadDim;

	pfltMatrixA			= (float*)malloc(sizeof(float) *(unsigned int)  iSquare);
	pfltMatrixX			= (float*)malloc(sizeof(float) *(unsigned int)  iSquare);
	pfltMatrixD			= (float*)malloc(sizeof(float) *(unsigned int)  iSquare);
	pfltMatrixcX		= (float*)malloc(sizeof(float) *(unsigned int)  iSquare);
	pfltMatrixcA		= (float*)malloc(sizeof(float) *(unsigned int)  iSquare);
	pfltMatrixEye		= (float*)malloc(sizeof(float) *(unsigned int)  iSquare);
	pfltMatrixTemp		= (float*)malloc(sizeof(float) *(unsigned int)  iSquare);
	pfltMatrixTemp2		= (float*)malloc(sizeof(float) *(unsigned int)  iSquare);



	/* Scale A by power of 2 so that its norm is < 1/2 .*/
	sfrexps( sinfnorma( in, _iLeadDim, _iLeadDim), &fltExp);
	fltS	= max(0, fltExp + 1);
	fltS2	= spows(2, fltS);

	/*A = A./2^s*/

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixA[iIndex1] =  in[iIndex1] / fltS2;

	/* Pade approximation for exp(A)*/
	/*X = A */
	/*C2F(dcopy)(&iSquare, pfltMatrixA, &iOne, pfltMatrixX, &iOne );*/

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixX[iIndex1] = pfltMatrixA[iIndex1] ;

	seyea(pfltMatrixEye, _iLeadDim, _iLeadDim);


	/*cA = A * c*/


    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixcA[iIndex1] = pfltMatrixA[iIndex1] * fltCst ;


	/*E = Eye + cA*/
    sadda (pfltMatrixEye , iSquare, pfltMatrixcA ,iSquare, out ) ;

	/*D = Eye - cA*/
    sdiffa (pfltMatrixEye , iSquare, pfltMatrixcA ,iSquare,pfltMatrixD ) ;

	iMax	= 6;
	iFlag	= 1;

	for(iLoop1 = 2 ; iLoop1 <= iMax ; iLoop1++)
	{


		dblCst	= dblCst * (iMax - iLoop1 + 1 ) / (iLoop1 * (2 * iMax - iLoop1 + 1));

        dblCst += 0 ;
		/*Temp = X*/
		/*C2F(dcopy)(&iSquare, pfltMatrixX, &iOne, pfltMatrixTemp, &iOne);*/
        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
                pfltMatrixTemp[iIndex1] = pfltMatrixX[iIndex1] ;
		/*X = A * Temp;*/

            smulma (  pfltMatrixA , _iLeadDim , _iLeadDim,
                    pfltMatrixTemp , _iLeadDim , _iLeadDim,
                    pfltMatrixX );

		/*cX = c * X*/

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixcX[iIndex1] = pfltMatrixX[iIndex1] * (float) dblCst ;

		/*E = E + cX*/
        sadda ( out, iSquare , pfltMatrixcX , iSquare , out ) ;

        if(iFlag == 1) /*D = D + cX*/
		    {
            sadda ( pfltMatrixD, iSquare , pfltMatrixcX , iSquare , pfltMatrixD ) ;
		    }
		else /*D = D - cX*/
		    {
            sdiffa ( pfltMatrixD, iSquare , pfltMatrixcX , iSquare , pfltMatrixD );
            }

		/*Toggle iFlag*/
		iFlag = !iFlag;
	}

	/*Temp = E*/
	/*C2F(dcopy)(&iSquare, out, &iOne, pfltMatrixTemp, &iOne);*/
    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixTemp[iIndex1] = out[iIndex1] ;

	/*E = D\E*/
    sldivma (  pfltMatrixD , _iLeadDim , _iLeadDim , pfltMatrixTemp , _iLeadDim , _iLeadDim , out );

	/* Undo scaling by repeated squaring*/
	for(iLoop1 = 0 ; iLoop1 < fltS ; iLoop1++)
	{
		/*Temp = E*/
		/*Temp2 = E*/
        /*C2F(dcopy)(&iSquare, out, &iOne, pfltMatrixTemp, &iOne);
		C2F(dcopy)(&iSquare, out, &iOne, pfltMatrixTemp2, &iOne);*/

        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
            {
            pfltMatrixTemp [iIndex1] = out[iIndex1] ;
            pfltMatrixTemp2[iIndex1] = out[iIndex1] ;
            }
		/* E = E*E*/
        smulma (  pfltMatrixTemp , _iLeadDim , _iLeadDim,
                    pfltMatrixTemp2 , _iLeadDim , _iLeadDim,
                    out );
	}

	free(pfltMatrixA);
	free(pfltMatrixX);
	free(pfltMatrixD);
	free(pfltMatrixcX);
	free(pfltMatrixcA);
	free(pfltMatrixEye);
	free(pfltMatrixTemp);
	free(pfltMatrixTemp2);



	return ;
}
