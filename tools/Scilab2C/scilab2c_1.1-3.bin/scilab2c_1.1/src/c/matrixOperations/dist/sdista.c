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
#include "dist.h"
#include "matrixTranspose.h"
#include "matrixMultiplication.h"
#include "spec.h"
#include "sqrt.h"


float sdista(float* in1,float* in2, int lines, int columns){
	int i=0;
	float out=0;
	float *a, *at, *mul;
	floatComplex *eigenvalues,*mulCpx;
	
	
	/* FIXME : malloc here*/
	a=(float*)malloc((unsigned int)(lines*columns)*sizeof(float));
	at=(float*)malloc((unsigned int)(lines*columns)*sizeof(float));
	mul=(float*)malloc((unsigned int)(lines*lines)*sizeof(float));
	eigenvalues=(floatComplex*)malloc((unsigned int)(lines)*sizeof(floatComplex));
	mulCpx=(floatComplex*)malloc((unsigned int)(lines*lines)*sizeof(floatComplex));
		
	for (i=0;i<lines*columns;i++) a[i]=in1[i]-in2[i];
	stransposea(a,lines, columns,at);
	smulma(a,lines,columns,at,columns,lines,mul);
	for (i=0;i<lines*lines;i++) mulCpx[i]=FloatComplex(mul[i],0);
	cspeca(mulCpx,lines,eigenvalues);
	csqrta(eigenvalues,lines,eigenvalues);
	
	/* Research of the higher value of eigenvalues*/
	for (i=0;i<lines;i++){
		if (creals(eigenvalues[i])>out) out = creals(eigenvalues[i]);
	}
	
	free(a);
	free(at);
	free(mul);
	free(eigenvalues);
	free(mulCpx);
	
	return out;
}
