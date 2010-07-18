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
/****FIXME****/
/**** WARNING NOT WORK AT ALL FOR THE MOMENT  ***/
/**** Because of problem of conversion float-> double ****/
#include "matrixDivision.h" 
#include "lapack.h" 
#include <string.h>
#include <stdio.h>
void srdivma (	float* in1, int lines1, int columns1 ,
			    float* in2, int lines2, int columns2 ,
			 	float* out ){

	int i = 0 ;
	/* these 3 variable are created to permit to use the value in the fortran functions 
	   because they need double matrix as arguments and we can't cast directly the pointers
	   without having problems , i know that's ugly */
	double *dblin1	= NULL;
	double *dblin2	= NULL;
	double *dblout	= NULL;	

	
	
	/* Array allocations*/
	dblin1		= (double*)malloc(sizeof(double) * (unsigned int)columns1 * (unsigned int)lines1);			 
	dblin2 		= (double*)malloc(sizeof(double) * (unsigned int)columns2 * (unsigned int)lines2);
	dblout		= (double*)malloc(sizeof(double) * (unsigned int)lines1 * (unsigned int)lines2);



	/*copy and cast all the float value into double value */								
	for ( i = 0 ; i < lines1 * columns1 ; i ++ )
		{        
		dblin1[i] = (double) in1[i] ;
		}

	for ( i = 0 ; i < lines2 * columns2 ; i ++ )
        {
         dblin2[i] = (double) in2[i] ;
		}       
                     
        drdivma( dblin1 , lines1 , columns1 , dblin2 , lines2 , columns2 , dblout );

                     
	for ( i = 0 ; i < min(lines2,columns2) * lines1 ; i++ )
        {
		out[i] = (float) dblout[i] ;

        }							

    free ( dblin1);
    free ( dblin2);
    free ( dblout);                    

}
