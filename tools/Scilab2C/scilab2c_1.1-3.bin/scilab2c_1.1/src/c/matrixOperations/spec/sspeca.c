/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Arnaud TORSET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <stdlib.h>
#include "spec.h"



void sspeca(float* in, int rows, float* out){
	/* As we use Lapack to find the eigenvalues, we must cast the float input into double
	and the doubleComplex output of dspeca into floatComplex*/
	
	int i;
	double* dblin;
	double* dblout;
	
	dblin=(double*)malloc((unsigned int)(rows*rows)*sizeof(double));
	dblout=(double*)malloc((unsigned int)rows*sizeof(double));
	
	for (i=0;i<rows*rows;i++) dblin[i]=(double)in[i];
	
	dspeca(dblin,rows,dblout);
	
	for (i=0;i<rows;i++) out[i]=(float)dblout[i];
}
