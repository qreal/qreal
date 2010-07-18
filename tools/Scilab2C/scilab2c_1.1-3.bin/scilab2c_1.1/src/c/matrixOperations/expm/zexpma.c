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

void zexpma(doubleComplex * in, doubleComplex * out, int _iLeadDim)
{
	int iIndex1		= 0;
	int iMax		= 0;
	int iFlag		= 0;
	int iLoop1		= 0;
	int iSquare		= 0;


	double dblExp	= 0;
	double dblS		= 0;
	double dblS2	= 0;
	double dblCst	= 0.5;

    doubleComplex  zdblCst ;

	doubleComplex *pdblMatrixA		= NULL;
	doubleComplex *pdblMatrixX		= NULL;
	doubleComplex *pdblMatrixD		= NULL;
	doubleComplex *pdblMatrixcX 	= NULL;
	doubleComplex *pdblMatrixcA 	= NULL;
	doubleComplex *pdblMatrixEye	= NULL;
	doubleComplex *pdblMatrixTemp	= NULL;
	doubleComplex *pdblMatrixTemp2	= NULL;




	iSquare = _iLeadDim * _iLeadDim;

	pdblMatrixA			= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int) iSquare);
	pdblMatrixX			= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int) iSquare);
	pdblMatrixD			= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int) iSquare);
	pdblMatrixcX		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int) iSquare);
	pdblMatrixcA		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int) iSquare);
	pdblMatrixEye		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int) iSquare);
	pdblMatrixTemp		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int) iSquare);
	pdblMatrixTemp2		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int) iSquare);

    zdblCst = DoubleComplex ( 0.5 , 0 );

	/*// Scale A by power of 2 so that its norm is < 1/2 .*/
	dfrexps(  zinfnorma( in, _iLeadDim, _iLeadDim) , &dblExp);
	dblS	= max(0, dblExp + 1);
	dblS2	= pow(2, dblS);

	/*A = A./2^s */

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixA[iIndex1] = zrdivs ( in[iIndex1] , DoubleComplex ( dblS2 , 0 ));


	/* Pade approximation for exp(A)
	//X = A */
	/*C2F(zcopy)(&iSquare, pdblMatrixA, &iOne, pdblMatrixX, &iOne );*/
     for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixX[iIndex1] = pdblMatrixA[iIndex1] ;


	zeyea(pdblMatrixEye, _iLeadDim, _iLeadDim);

   /* zmulma ( & zdblCst , 1 ,1,
         pdblMatrixA , _iLeadDim, _iLeadDim,
         pdblMatrixcA);*/

    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixcA[iIndex1] = zmuls ( pdblMatrixA[iIndex1] , zdblCst ) ;

    /* cA = A * c   */


	/*E = Eye + cA*/

    zadda (pdblMatrixEye , iSquare, pdblMatrixcA ,iSquare, out ) ;

	/* D = Eye - cA */

    zdiffa (pdblMatrixEye , iSquare, pdblMatrixcA ,iSquare,pdblMatrixD ) ;

	iMax	= 6;
	iFlag	= 1;

	for(iLoop1 = 2 ; iLoop1 <= iMax ; iLoop1++)
	{
		dblCst	= dblCst * (iMax - iLoop1 + 1 ) / (iLoop1 * (2 * iMax - iLoop1 + 1));
        zdblCst = DoubleComplex( dblCst , 0);

		/*Temp = X */
		/*C2F(zcopy)(&iSquare, pdblMatrixX, &iOne, pdblMatrixTemp, &iOne);*/
        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
                pdblMatrixTemp[iIndex1] = pdblMatrixX[iIndex1] ;
		/* X = A * Temp; */

                  zmulma (  pdblMatrixA , _iLeadDim , _iLeadDim,
                    pdblMatrixTemp , _iLeadDim , _iLeadDim,
                    pdblMatrixX );
		/* cX = c * X */

        /*    zmulma ( & zdblCst , 1 ,1,
                     pdblMatrixX , _iLeadDim, _iLeadDim,
                     pdblMatrixcX);*/

        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
            pdblMatrixcX[iIndex1] = zmuls ( pdblMatrixX[iIndex1] , zdblCst ) ;

		/* E = E + cX */

        zadda ( out, iSquare , pdblMatrixcX , iSquare , out ) ;

		if(iFlag == 1) /* D = D + cX */
		{
            zadda ( pdblMatrixD, iSquare , pdblMatrixcX , iSquare , pdblMatrixD ) ;
		}
		else /* D = D - cX */
		{
            zdiffa ( pdblMatrixD, iSquare , pdblMatrixcX , iSquare , pdblMatrixD );
		}

		/* Toggle iFlag */
		iFlag = !iFlag;
	}

	/* Temp = E */
	/*C2F(zcopy)(&iSquare, out, &iOne, pdblMatrixTemp, &iOne);*/
    for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
        pdblMatrixTemp[iIndex1] = out[iIndex1] ;

	/* E = D\E */
    zldivma (  pdblMatrixD , _iLeadDim , _iLeadDim , pdblMatrixTemp , _iLeadDim , _iLeadDim , out ) ;

	/*/ Undo scaling by repeated squaring */
	for(iLoop1 = 0 ; iLoop1 < dblS ; iLoop1++)
	{
		/*//Temp = E */
        /*//Temp2 = E */

		/*C2F(zcopy)(&iSquare, out, &iOne, pdblMatrixTemp, &iOne);
		C2F(zcopy)(&iSquare, out, &iOne, pdblMatrixTemp2, &iOne);*/
        for ( iIndex1 = 0 ; iIndex1 < iSquare ; iIndex1++ )
            {
            pdblMatrixTemp [iIndex1] = out[iIndex1] ;
            pdblMatrixTemp2[iIndex1] = out[iIndex1] ;
            }

		/* E = E*E*/
          zmulma (  pdblMatrixTemp ,  _iLeadDim , _iLeadDim,
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
