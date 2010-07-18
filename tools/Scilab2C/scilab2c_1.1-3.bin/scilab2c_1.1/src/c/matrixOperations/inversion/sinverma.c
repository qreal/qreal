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


#include <stdio.h>
#include <stdlib.h>
#include "matrixInversion.h"
#include "lapack.h"

void sinverma ( float* in, float* out, int leadDimIn )
{

	int i = 0 ;
	/* these 3 variable are created to permit to use the value in the fortran functions
	   because they need double matrix as arguments and we can't cast directly the pointers
	   without having problems , i know that's ugly */
	double *dblin	= NULL;
	double *dblout	= NULL;



	/* Array allocations*/
	dblin		= (double*)malloc(sizeof(double) * (unsigned int)(leadDimIn * leadDimIn));
	dblout		= (double*)malloc(sizeof(double) * (unsigned int)(leadDimIn * leadDimIn));



	/*copy and cast all the float value into double value */
	for ( i = 0 ; i < (leadDimIn * leadDimIn) ; i ++ )
		{
		dblin[i] = (double) in[i]  ;
		}

    dinverma ( dblin, dblout, leadDimIn );


	for ( i = 0 ; i < (leadDimIn * leadDimIn) ; i++ )
        {
		out[i] =(float) dblout[i] ;

        }

    free ( dblin);
    free ( dblout);

}
