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

#include <malloc.h>
#include "logm.h"


void slogma (float* in, int rows, float* out){
	doubleComplex *inCpx, *outCopy;
	int i;
	
	inCpx=(doubleComplex *)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	outCopy=(doubleComplex *)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	
	for (i=0;i<rows*rows;i++) inCpx[i] = DoubleComplex(in[i],0);
	
	zlogma(inCpx, rows, outCopy);
	
	for(i=0;i<rows*rows;i++)
		out[i]=(float)zreals(outCopy[i]);
		
	free(inCpx);
	free(outCopy);	
}
