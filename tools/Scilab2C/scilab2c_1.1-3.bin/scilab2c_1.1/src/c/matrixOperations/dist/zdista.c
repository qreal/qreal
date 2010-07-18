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
#include "dist.h"
#include "matrixTranspose.h"
#include "matrixMultiplication.h"
#include "spec.h"
#include "sqrt.h"
#include "conj.h"
#include "subtraction.h"
#include "stdio.h"

double zdista(doubleComplex* in1,doubleComplex* in2, int lines, int columns){
	int i=0;
	double out = 0;
	doubleComplex *a, *at, *mul;
	doubleComplex *eigenvalues;
	
	
	/* FIXME : malloc here*/
	a=(doubleComplex*)malloc((unsigned int)(lines*columns)*sizeof(doubleComplex));
	at=(doubleComplex*)malloc((unsigned int)(lines*columns)*sizeof(doubleComplex));
	mul=(doubleComplex*)malloc((unsigned int)(lines*lines)*sizeof(doubleComplex));
	eigenvalues=(doubleComplex*)malloc((unsigned int)(lines)*sizeof(doubleComplex));
		
	for (i=0;i<lines*columns;i++) a[i]=zdiffs(in1[i],in2[i]);
	ztransposea(a,lines, columns,at);
	zconja(at,lines*columns,at);
	zmulma(a,lines,columns,at,columns,lines,mul);
	zspeca(mul,lines,eigenvalues);
	zsqrta(eigenvalues,lines,eigenvalues);
	
	/* Research of the higher value of eigenvalues*/
	for (i=0;i<lines;i++){
		if (zreals(eigenvalues[i])>out) out = zreals(eigenvalues[i]);
	}
		
	
	free(a);
	free(at);
	free(mul);
	free(eigenvalues);
	
	return out;
}
