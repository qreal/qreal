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
#include "matrixPow.h"
#include "spec.h"
#include "pow.h"
#include "matrixTranspose.h"
#include "conj.h"
#include "matrixInversion.h"
#include "matrixMultiplication.h"

void cpowma(floatComplex* in, int rows, floatComplex power, floatComplex* out){
	int i=0, j=0;
	int hermitian=0;
	floatComplex *eigenvalues,*eigenvectors,*tmp;
	
	/* Data initialization */
	eigenvalues = (floatComplex*)malloc((unsigned int)(rows*rows)*sizeof(floatComplex));
	eigenvectors = (floatComplex*)malloc((unsigned int)(rows*rows)*sizeof(floatComplex));
	tmp = (floatComplex*)malloc((unsigned int)(rows*rows)*sizeof(floatComplex));
	
	/* symmetric test*/
	for(i=0;i<rows;i++) {
		for (j=0;j<rows;j++)
			if ((creals(in[i*rows+j])!=creals(in[j*rows+i])) || (cimags(in[i*rows+j])!=-cimags(in[j*rows+i]))) break;
		
		if (j!=rows) break;
	}
	
	if ((i==rows)&&(j==rows)) hermitian=1;
	
	
	/* find eigenvalues and eigenvectors */
	cspec2a(in, rows, eigenvalues,eigenvectors);
	
	/* make operation on eigenvalues and eigenvectors */
	for (i=0;i<rows;i++)
			eigenvalues[i+i*rows]=cpows(eigenvalues[i+i*rows],power);
			
	cmulma(eigenvectors, rows, rows, eigenvalues, rows, rows, tmp);
	
	if (hermitian){		
		ctransposea(eigenvectors, rows,rows, eigenvalues);
		cconja(eigenvalues, rows*rows, eigenvalues);
	}
	else cinverma(eigenvectors, eigenvalues, rows);
	
	cmulma(tmp, rows, rows, eigenvalues, rows, rows, out);
	
	free(eigenvalues);
	free(eigenvectors);
	free(tmp);
	
}
