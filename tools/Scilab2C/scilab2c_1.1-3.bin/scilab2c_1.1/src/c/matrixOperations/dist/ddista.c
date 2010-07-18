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


double ddista(double* in1,double* in2, int lines, int columns){
	int i=0;
	double out=0;
	double *a, *at, *mul;
	doubleComplex *eigenvalues,*mulCpx;
	
	
	/* FIXME : malloc here*/
	a=(double*)malloc((unsigned int)(lines*columns)*sizeof(double));
	at=(double*)malloc((unsigned int)(lines*columns)*sizeof(double));
	mul=(double*)malloc((unsigned int)(lines*lines)*sizeof(double));
	eigenvalues=(doubleComplex*)malloc((unsigned int)(lines)*sizeof(doubleComplex));
	mulCpx=(doubleComplex*)malloc((unsigned int)(lines*lines)*sizeof(doubleComplex));
		
	for (i=0;i<lines*columns;i++) a[i]=in1[i]-in2[i];
	dtransposea(a,lines, columns,at);
	dmulma(a,lines,columns,at,columns,lines,mul);
	for (i=0;i<lines*lines;i++) mulCpx[i]=DoubleComplex(mul[i],0);
	zspeca(mulCpx,lines,eigenvalues);
	zsqrta(eigenvalues,lines,eigenvalues);
	
	/* Research of the higher value of eigenvalues*/
	for (i=0;i<lines;i++){
		if (zreals(eigenvalues[i])>out) out = zreals(eigenvalues[i]);
	}
	
	free(a);
	free(at);
	free(mul);
	free(eigenvalues);
	free(mulCpx);
	
	return out;
}
