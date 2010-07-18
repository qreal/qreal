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

void zpowma(doubleComplex* in, int rows, doubleComplex power, doubleComplex* out){
	int i=0, j=0;
	int hermitian=0;
	doubleComplex *eigenvalues,*eigenvectors,*tmp;
	
	/* Data initialization */
	eigenvalues = (doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	eigenvectors = (doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	tmp = (doubleComplex*)malloc((unsigned int)(rows*rows)*sizeof(doubleComplex));
	
	/* hermitian test*/
	for(i=0;i<rows;i++) {
		for (j=0;j<rows;j++)
			if ((zreals(in[i*rows+j])!=zreals(in[j*rows+i])) || (zimags(in[i*rows+j])!=-zimags(in[j*rows+i]))) break;
		
		if (j!=rows) break;
	}
	
	if ((i==rows)&&(j==rows)) hermitian=1;
	
	
	/* find eigenvalues and eigenvectors */
	zspec2a(in, rows, eigenvalues,eigenvectors);
	
	/* make operation on eigenvalues and eigenvectors */
	for (i=0;i<rows;i++)
			eigenvalues[i+i*rows]=zpows(eigenvalues[i+i*rows],power);
			
	zmulma(eigenvectors, rows, rows, eigenvalues, rows, rows, tmp);
	
	if (hermitian){		
		ztransposea(eigenvectors, rows,rows, eigenvalues);
		zconja(eigenvalues, rows*rows, eigenvalues);
	}
	else zinverma(eigenvectors, eigenvalues, rows);
	
	zmulma(tmp, rows, rows, eigenvalues, rows, rows, out);
	
	free(eigenvalues);
	free(eigenvectors);
	free(tmp);
	
}
