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
#include "matrixInversion.h" 
#include "lapack.h" 
#include <stdio.h>

void cinverma ( floatComplex* in, floatComplex* out, int leadDimIn )
{

	int i = 0 ;
	/* these 3 variable are created to permit to use the value in the fortran functions 
	   because they need doubleComplex matrix as arguments and we can't cast directly the pointers
	   without having problems , i know that's ugly */
	doubleComplex *dblin	= NULL;
	doubleComplex *dblout	= NULL;	

	
	
	/* Array allocations*/
	dblin		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int)(leadDimIn * leadDimIn));			 
	dblout		= (doubleComplex*)malloc(sizeof(doubleComplex) * (unsigned int)(leadDimIn * leadDimIn));



	/*copy and cast all the floatComplex value into doubleComplex value */								
	for ( i = 0 ; i < (leadDimIn * leadDimIn) ; i ++ )
		{        
		dblin[i] = DoubleComplex ( (double) creals( in[i]) , (double) cimags ( in[i])) ;
		}
    
    zinverma ( dblin, dblout, leadDimIn );

                     
	for ( i = 0 ; i < (leadDimIn * leadDimIn) ; i++ )
        {
		out[i] = FloatComplex ((float)  zreals ( dblout[i]) , (float) zimags ( dblout[i])) ;

        }							

    free ( dblin);
    free ( dblout);                    

}
