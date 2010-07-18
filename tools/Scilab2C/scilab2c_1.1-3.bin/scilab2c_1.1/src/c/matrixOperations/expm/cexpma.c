/*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

void cexpma(floatComplex * in, floatComplex * out, int _iLeadDim)
{
	int iIndex1		= 0;
	int iMax		= 0;
	int iFlag		= 0;
	int iLoop1		= 0;
	int iSquare		= 0;


	float fltExp	= 0;
	float fltS		= 0;
	float fltS2	= 0;
	double fltCst	= 0.5;



	floatComplex *pfltMatrixA		= NULL;
	floatComplex *pfltMatrixX		= NULL;
	floatComplex *pfltMatrixD		= NULL;
	floatComplex *pfltMatrixcX 	= NULL;
	floatComplex *pfltMatrixcA 	= NULL;
	floatComplex *pfltMatrixEye	= NULL;
	floatComplex *pfltMatrixTemp	= NULL;
	floatComplex *pfltMatrixTemp2	= NULL;




	iSquare = _iLeadDim * _iLeadDim;

	pfltMatrixA			= (floatComplex*)malloc(sizeof(floatComplex) * (unsigned int) iSquare);
	pfltMatrixX			= (floatComplex*)malloc(sizeof(floatComplex) * (unsigned int) iSquare);
	pfltMatrixD			= (floatComplex*)malloc(sizeof(floatComplex) * (unsigned int) iSquare);
	pfltMatrixcX		= (floatComplex*)malloc(sizeof(floatComplex) * (unsigned int) iSquare);
	pfltMatrixcA		= (floatComplex*)malloc(sizeof(floatComplex) * (unsigned int) iSquare);
	pfltMatrixEye		= (floatComplex*)malloc(sizeof(floatComplex) * (unsigned int) iSquare);
	pfltMatrixTemp		= (floatComplex*)malloc(sizeof(floatComplex) * (unsigned int) iSquare);
	pfltMatrixTemp2		= (floatComplex*)malloc(sizeof(floatComplex) * (unsigned int) iSquare);


	/*// Scale A by power of 2 so that its norm is < 1/2 .*/
	sfrexps(  cinfnorma( in, _iLeadDim, _iLeadDim) , &fltExp);
	fltS	= max(0, fltExp + 1);
	fltS2	= spows(2.0f,  fltS);

	/*A = A./2^s */

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixA[iIndex1] = crdivs ( in[iIndex1] , FloatComplex ( fltS2 , 0 ));


	/* Pade approximation for exp(A)
	//X = A */
	/*C2F(zcopy)(&iSquare, pfltMatrixA, &iOne, pfltMatrixX, &iOne );*/
     for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixX[iIndex1] = pfltMatrixA[iIndex1] ;


	ceyea(pfltMatrixEye, _iLeadDim, _iLeadDim);

    /*cmulma ( & cfltCst , 1 ,1,
         pfltMatrixA , _iLeadDim, _iLeadDim,
         pfltMatrixcA);*/

    /* cA = A * c   */
    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixcA[iIndex1] = cmuls ( pfltMatrixA[iIndex1] , FloatComplex((float) fltCst , 0) ) ;

	/*E = Eye + cA*/

    cadda (pfltMatrixEye , iSquare, pfltMatrixcA ,iSquare, out ) ;

	/* D = Eye - cA */

    cdiffa (pfltMatrixEye , iSquare, pfltMatrixcA ,iSquare,pfltMatrixD ) ;

	iMax	= 6;
	iFlag	= 1;

	for(iLoop1 = 2 ; iLoop1 <= iMax ; iLoop1++)
	{
		fltCst	= fltCst * (iMax - iLoop1 + 1 ) / (iLoop1 * (2 * iMax - iLoop1 + 1));

		/*Temp = X */
		/*C2F(zcopy)(&iSquare, pfltMatrixX, &iOne, pfltMatrixTemp, &iOne);*/
        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
                pfltMatrixTemp[iIndex1] = pfltMatrixX[iIndex1] ;
		/* X = A * Temp; */

                  cmulma (  pfltMatrixA , _iLeadDim , _iLeadDim,
                    pfltMatrixTemp , _iLeadDim , _iLeadDim,
                    pfltMatrixX );
		/* cX = c * X */

        /*    cmulma ( & cfltCst , 1 ,1,
                     pfltMatrixX , _iLeadDim, _iLeadDim,
                     pfltMatrixcX);*/

        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
            pfltMatrixcX[iIndex1] = cmuls ( pfltMatrixX[iIndex1] , FloatComplex((float) fltCst , 0) ) ;

		/* E = E + cX */

        cadda ( out, iSquare , pfltMatrixcX , iSquare , out ) ;

		if(iFlag == 1) /* D = D + cX */
		{
            cadda ( pfltMatrixD, iSquare , pfltMatrixcX , iSquare , pfltMatrixD ) ;
		}
		else /* D = D - cX */
		{
            cdiffa ( pfltMatrixD, iSquare , pfltMatrixcX , iSquare , pfltMatrixD );
		}

		/* Toggle iFlag */
		iFlag = !iFlag;
	}

	/* Temp = E */
	/*C2F(zcopy)(&iSquare, out, &iOne, pfltMatrixTemp, &iOne);*/
    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pfltMatrixTemp[iIndex1] = out[iIndex1] ;

	/* E = D\E */
    cldivma (  pfltMatrixD , _iLeadDim , _iLeadDim , pfltMatrixTemp , _iLeadDim , _iLeadDim , out ) ;

	/*/ Undo scaling by repeated squaring */
	for(iLoop1 = 0 ; iLoop1 < fltS ; iLoop1++)
	{
		/*//Temp = E */
        /*//Temp2 = E */

		/*C2F(zcopy)(&iSquare, out, &iOne, pfltMatrixTemp, &iOne);
		C2F(zcopy)(&iSquare, out, &iOne, pfltMatrixTemp2, &iOne);*/
        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
            {
            pfltMatrixTemp [iIndex1] = out[iIndex1] ;
            pfltMatrixTemp2[iIndex1] = out[iIndex1] ;
            }

		/* E = E*E*/
          cmulma (  pfltMatrixTemp ,  _iLeadDim , _iLeadDim,
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
