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



void sspec2a(float* in, int rows, float* eigenvalues,float* eigenvectors){
	/* As we use Lapack to find the eigenvalues, we must cast the float input into double
	and the doubleComplex outputs of dspec2a into floatComplex*/
	
	int i;
	double* dblin;
	double *dbleigenvalues,*dbleigenvectors;
	
	dblin=(double*)malloc((unsigned int)(rows*rows)*sizeof(double));
	dbleigenvalues = (double*)malloc((unsigned int)(rows*rows)*sizeof(double));	
	dbleigenvectors = (double*)malloc((unsigned int)(rows*rows)*sizeof(double));
	
	for (i=0;i<rows*rows;i++) dblin[i]=(double)in[i];
	
	dspec2a(dblin,rows,dbleigenvalues,dbleigenvectors);
	
	for (i=0;i<rows*rows;i++) eigenvalues[i]=(float)dbleigenvalues[i];
	for (i=0;i<rows*rows;i++) eigenvectors[i]=(float)dbleigenvectors[i];
}
