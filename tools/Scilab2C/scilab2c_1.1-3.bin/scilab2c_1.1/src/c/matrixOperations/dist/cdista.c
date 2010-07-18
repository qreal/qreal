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

float cdista(floatComplex* in1,floatComplex* in2, int lines, int columns){
	int i=0;
	float out = 0;
	floatComplex *a, *at, *mul;
	floatComplex *eigenvalues;
	
	
	/* FIXME : malloc here*/
	a=(floatComplex*)malloc((unsigned int)(lines*columns)*sizeof(floatComplex));
	at=(floatComplex*)malloc((unsigned int)(lines*columns)*sizeof(floatComplex));
	mul=(floatComplex*)malloc((unsigned int)(lines*lines)*sizeof(floatComplex));
	eigenvalues=(floatComplex*)malloc((unsigned int)(lines)*sizeof(floatComplex));
		
	for (i=0;i<lines*columns;i++) a[i]=cdiffs(in1[i],in2[i]);
	ctransposea(a,lines, columns,at);
	cconja(at,lines*columns,at);
	cmulma(a,lines,columns,at,columns,lines,mul);
	cspeca(mul,lines,eigenvalues);
	csqrta(eigenvalues,lines,eigenvalues);
	
	/* Research of the higher value of eigenvalues*/
	for (i=0;i<lines;i++){
		if (creals(eigenvalues[i])>out) out = creals(eigenvalues[i]);
	}
		
	
	free(a);
	free(at);
	free(mul);
	free(eigenvalues);
	
	return out;
}
