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
#include <stdio.h>
#include "spec.h"
#include "lapack.h"
#include "zeros.h"
#include "conj.h"


void zspeca(doubleComplex* in, int rows,doubleComplex* out){
	int i = 0, j = 0;
	int hermitian = 0;
	int INFO = 0;
	int iWorkSize = 0;
	doubleComplex* pdblWork;
	doubleComplex* pdblLeftvectors;
	doubleComplex* pdblRightvectors;
	doubleComplex* pdblRWork;
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
	pdblRWork = (doubleComplex*)malloc((unsigned int)(3*rows) * sizeof(doubleComplex));	
	
	for(i=0;i<rows*rows;i++) inCopy[i]=DoubleComplex(zreals(in[i]),zimags(in[i]));
	
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
		C2F(zheev)( "N", "U", &rows, inCopy, &rows, outReal, pdblWork, &iWorkSize, outImag, &INFO );
		dzerosa(outImag,1,rows);
		for (i=0;i<rows;i++) out[i]=DoubleComplex(outReal[i],outImag[i]);
	}
	else {
		C2F(zgeev)( "N", "N", &rows, inCopy, &rows, out, 
		pdblLeftvectors, &rows, pdblRightvectors, &rows, pdblWork, &iWorkSize,
		pdblRWork, &INFO );
	}
	


	free(inCopy);
	free(outReal);
	free(outImag);
	free(pdblWork);
	free(pdblRWork);
	free(pdblLeftvectors);
	free(pdblRightvectors);
}
