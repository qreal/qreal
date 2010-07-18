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



void cspec2a(floatComplex* in, int rows, floatComplex* eigenvalues,floatComplex* eigenvectors){
	/* As we use Lapack to find the eigenvalues, we must cast the floatComplex input into doubleComplex
	and the doubleComplex outputs of dspeca into floatComplex*/
	
	int i;
	doubleComplex* dblin;
	doubleComplex* dbleigenvalues,*dbleigenvectors;
	
	dblin=(doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	dbleigenvalues=(doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));	
	dbleigenvectors=(doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	
	for (i=0;i<rows*rows;i++) dblin[i]=DoubleComplex((double)creals(in[i]),(double)cimags(in[i]));
	
	zspec2a(dblin,rows,dbleigenvalues,dbleigenvectors);
	
	for (i=0;i<rows*rows;i++) eigenvalues[i]=FloatComplex((float)zreals(dbleigenvalues[i]),(float)zimags(dbleigenvalues[i]));
	for (i=0;i<rows*rows;i++) eigenvectors[i]=FloatComplex((float)zreals(dbleigenvectors[i]),(float)zimags(dbleigenvectors[i]));
}
