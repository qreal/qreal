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



void cspeca(floatComplex* in, int rows, floatComplex* out){
	/* As we use Lapack to find the eigenvalues, we must cast the floatComplex input into doubleComplex
	and the doubleComplex output of dspeca into floatComplex*/
	
	int i;
	doubleComplex* dblin;
	doubleComplex* dblout;
	
	dblin=(doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	dblout=(doubleComplex*)malloc((unsigned int)rows*sizeof(doubleComplex));
	
	for (i=0;i<rows*rows;i++) dblin[i]=DoubleComplex((double)creals(in[i]),(double)cimags(in[i]));
	
	zspeca(dblin,rows,dblout);
	
	for (i=0;i<rows;i++) out[i]=FloatComplex((float)zreals(dblout[i]),(float)zimags(dblout[i]));
}
