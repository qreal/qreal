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

void dexpma (double* in, double* out, int _iLeadDim){

    int iIndex1     = 0;
	int iMax		= 0;
	int iFlag		= 0;
	int iLoop1		= 0;
	int iSquare		= 0;


	double dblExp	= 0;
	double dblS		= 0;
	double dblS2	= 0;
	double dblCst	= 0.5;

	double *pdblMatrixA		= NULL;/*A'*/
	double *pdblMatrixX		= NULL;/*X*/
	double *pdblMatrixD		= NULL;/*D*/
	double *pdblMatrixcX	= NULL;/*cX*/
	double *pdblMatrixcA	= NULL;/*cX*/
	double *pdblMatrixEye	= NULL;/*Eye*/
	double *pdblMatrixTemp	= NULL;/*Temp*/
	double *pdblMatrixTemp2	= NULL;/*Temp2*/



	iSquare = _iLeadDim * _iLeadDim;

	pdblMatrixA			= (double*)malloc(sizeof(double) *(unsigned int)  iSquare);
	pdblMatrixX			= (double*)malloc(sizeof(double) *(unsigned int)  iSquare);
	pdblMatrixD			= (double*)malloc(sizeof(double) *(unsigned int)  iSquare);
	pdblMatrixcX		= (double*)malloc(sizeof(double) *(unsigned int)  iSquare);
	pdblMatrixcA		= (double*)malloc(sizeof(double) *(unsigned int)  iSquare);
	pdblMatrixEye		= (double*)malloc(sizeof(double) *(unsigned int)  iSquare);
	pdblMatrixTemp		= (double*)malloc(sizeof(double) *(unsigned int)  iSquare);
	pdblMatrixTemp2		= (double*)malloc(sizeof(double) *(unsigned int)  iSquare);



	/* Scale A by power of 2 so that its norm is < 1/2 .*/
	dfrexps( dinfnorma( in, _iLeadDim, _iLeadDim), &dblExp);
	dblS	= max(0, dblExp + 1);
	dblS2	= dpows(2, dblS);

	/*A = A./2^s*/

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixA[iIndex1] =  in[iIndex1] / dblS2 ;

	/* Pade approximation for exp(A)*/
	/*X = A */
	/*C2F(dcopy)(&iSquare, pdblMatrixA, &iOne, pdblMatrixX, &iOne );*/

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixX[iIndex1] = pdblMatrixA[iIndex1] ;

	deyea(pdblMatrixEye, _iLeadDim, _iLeadDim);


	/*cA = A * c*/


    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixcA[iIndex1] = pdblMatrixA[iIndex1] * dblCst ;


	/*E = Eye + cA*/
    dadda (pdblMatrixEye , iSquare, pdblMatrixcA ,iSquare, out ) ;

	/*D = Eye - cA*/
    ddiffa (pdblMatrixEye , iSquare, pdblMatrixcA ,iSquare,pdblMatrixD ) ;

	iMax	= 6;
	iFlag	= 1;

	for(iLoop1 = 2 ; iLoop1 <= iMax ; iLoop1++)
	{
		dblCst	= dblCst * (iMax - iLoop1 + 1 ) / (iLoop1 * (2 * iMax - iLoop1 + 1));

		/*Temp = X*/
		/*C2F(dcopy)(&iSquare, pdblMatrixX, &iOne, pdblMatrixTemp, &iOne);*/
        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
                pdblMatrixTemp[iIndex1] = pdblMatrixX[iIndex1] ;
		/*X = A * Temp;*/

            dmulma (  pdblMatrixA , _iLeadDim , _iLeadDim,
                    pdblMatrixTemp , _iLeadDim , _iLeadDim,
                    pdblMatrixX );

		/*cX = c * X*/

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixcX[iIndex1] = pdblMatrixX[iIndex1] * dblCst ;

		/*E = E + cX*/
        dadda ( out, iSquare , pdblMatrixcX , iSquare , out ) ;

        if(iFlag == 1) /*D = D + cX*/
		    {
            dadda ( pdblMatrixD, iSquare , pdblMatrixcX , iSquare , pdblMatrixD ) ;
		    }
		else /*D = D - cX*/
		    {
            ddiffa ( pdblMatrixD, iSquare , pdblMatrixcX , iSquare , pdblMatrixD );
            }

		/*Toggle iFlag*/
		iFlag = !iFlag;
	}

	/*Temp = E*/
	/*C2F(dcopy)(&iSquare, out, &iOne, pdblMatrixTemp, &iOne);*/
    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixTemp[iIndex1] = out[iIndex1] ;

	/*E = D\E*/
    dldivma (  pdblMatrixD , _iLeadDim , _iLeadDim , pdblMatrixTemp , _iLeadDim , _iLeadDim , out );

	/* Undo scaling by repeated squaring*/
	for(iLoop1 = 0 ; iLoop1 < dblS ; iLoop1++)
	{
		/*Temp = E*/
		/*Temp2 = E*/
        /*C2F(dcopy)(&iSquare, out, &iOne, pdblMatrixTemp, &iOne);
		C2F(dcopy)(&iSquare, out, &iOne, pdblMatrixTemp2, &iOne);*/

        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
            {
            pdblMatrixTemp [iIndex1] = out[iIndex1] ;
            pdblMatrixTemp2[iIndex1] = out[iIndex1] ;
            }
		/* E = E*E*/
        dmulma (  pdblMatrixTemp , _iLeadDim , _iLeadDim,
                    pdblMatrixTemp2 , _iLeadDim , _iLeadDim,
                    out );
	}

	free(pdblMatrixA);
	free(pdblMatrixX);
	free(pdblMatrixD);
	free(pdblMatrixcX);
	free(pdblMatrixcA);
	free(pdblMatrixEye);
	free(pdblMatrixTemp);
	free(pdblMatrixTemp2);



	return ;
}
