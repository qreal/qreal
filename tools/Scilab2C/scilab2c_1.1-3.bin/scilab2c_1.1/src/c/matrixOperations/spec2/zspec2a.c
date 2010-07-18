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
#include "spec.h"
#include "lapack.h"
#include "zeros.h"
#include "stdio.h"
#include "conj.h"


void zspec2a(doubleComplex* in, int rows,doubleComplex* eigenvalues, doubleComplex* eigenvectors){
	int i = 0, j = 0;
	int hermitian = 0;
	int INFO = 0;
	int iWorkSize = 0;
	doubleComplex* pdblWork;
	doubleComplex* pdblLeftvectors;
	doubleComplex* pdblRightvectors;
	doubleComplex* pdblRWork;
	double* pdblRWork2;
	double* outReal;
	double* outImag;
	doubleComplex* inCopy;
	
	inCopy = (doubleComplex*)malloc((unsigned int)(rows*rows) * sizeof(doubleComplex));
	outReal = (double*)malloc((unsigned int)rows * sizeof(double));
	outImag = (double*)malloc((unsigned int)rows * sizeof(double));
	pdblLeftvectors=NULL;
	pdblRightvectors=NULL;
	
	iWorkSize = 2*rows;
	pdblWork = (doubleComplex*)malloc((unsigned int)iWorkSize * sizeof(doubleComplex));
	pdblRWork = NULL;	
	pdblRWork2 = NULL;
	
	for(i=0;i<rows*rows;i++) {
		inCopy[i]=DoubleComplex(zreals(in[i]),zimags(in[i]));
		eigenvectors[i]=DoubleComplex(zreals(in[i]),zimags(in[i]));
	}	
	zzerosa(eigenvalues,1,rows*rows);
	
	
	/* look if the matrix is symmetric */
	for (i=0;i<rows;i++){
		for (j=0;j<rows;j++) 
			if ( (zreals(in[i*rows+j])!=zreals(zconjs(in[i+j*rows]))) ||
			     (zimags(in[i*rows+j])!=zimags(zconjs(in[i+j*rows])))  )
			     break;
		if (j!=rows) break;
	}
		
	
	/* the matrix is symmetric if the 2 loops goes to end i.e
		i==rows and j==rows */
	if ((i==rows)&&(j==rows)) hermitian=1;
	
	
	/* apply lapack function according to symmetry */
	if(hermitian){
		pdblRWork2 = (double*)malloc((unsigned int)(3*rows) * sizeof(double));
		C2F(zheev)( "V", "U", &rows, eigenvectors, &rows, outReal, pdblWork, &iWorkSize, pdblRWork2, &INFO );
		dzerosa(outImag,1,rows);
		for (i=0;i<rows;i++) eigenvalues[i+i*rows]=DoubleComplex(outReal[i],outImag[i]);
	}
	else {
		pdblRWork = (doubleComplex*)malloc((unsigned int)(3*rows) * sizeof(doubleComplex));	
		C2F(zgeev)( "N", "V", &rows, inCopy, &rows, eigenvalues, 
		pdblLeftvectors, &rows, eigenvectors, &rows, pdblWork, &iWorkSize,
		pdblRWork, &INFO );
		for (i=1;i<rows;i++) {
			eigenvalues[i+i*rows]=DoubleComplex(zreals(eigenvalues[i]),zimags(eigenvalues[i]));
			eigenvalues[i]=DoubleComplex(0,0);
		}
	}
	
	


	free(inCopy);
	free(outReal);
	free(outImag);
	free(pdblWork);
	free(pdblRWork);
	free(pdblRWork2);
	free(pdblLeftvectors);
	free(pdblRightvectors);
}
