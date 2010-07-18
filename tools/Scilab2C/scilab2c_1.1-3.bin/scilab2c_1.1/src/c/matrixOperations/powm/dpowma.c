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
#include "zeros.h"
#include "matrixInversion.h"
#include "matrixMultiplication.h"
#include <stdio.h>

void dpowma(double* in, int rows, double power, double* out){
	int i=0, j=0;
	int symmetric=0;
	doubleComplex *eigenvalues,*eigenvectors,*tmp;
	double* ZEROS;
	
	/* Data initialization */
	eigenvalues = (doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	eigenvectors = (doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	tmp = (doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	ZEROS=(double*)malloc((unsigned int)(rows*rows)*sizeof(double));
	
	/* symmetric test*/
	for(i=0;i<rows;i++) {
		for (j=0;j<rows;j++)
			if (in[i*rows+j]!=in[j*rows+i]) break;
		
		if (j!=rows) break;
	}
	
	if ((i==rows)&&(j==rows)) symmetric=1;
	
	
	
	dzerosa(ZEROS,rows,rows);
	tmp = DoubleComplexMatrix(in,ZEROS,rows*rows);
	/* find eigenvalues and eigenvectors */
	zspec2a(tmp, rows, eigenvalues,eigenvectors);
/*	for (i=0;i<rows*rows;i++) printf("%f+%f*i\n",zreals(eigenvalues[i]),zimags(eigenvalues[i])); */
	/* make operation on eigenvalues and eigenvectors */
	for (i=0;i<rows;i++)
			eigenvalues[i+i*rows]=zpows(eigenvalues[i+i*rows],DoubleComplex(power,0));
			
	zmulma(eigenvectors, rows, rows, eigenvalues, rows, rows, tmp);
	
	if (symmetric){		
		ztransposea(eigenvectors, rows,rows, eigenvalues);
		zconja(eigenvalues, rows*rows, eigenvalues);
	}
	else zinverma(eigenvectors, eigenvalues, rows);
	
	zmulma(tmp, rows, rows, eigenvalues, rows, rows, eigenvectors);
	
	for (i=0;i<rows*rows;i++) out[i]=zreals(eigenvectors[i]);
	
	free(eigenvalues);
	free(eigenvectors);
	free(tmp);
	
}
