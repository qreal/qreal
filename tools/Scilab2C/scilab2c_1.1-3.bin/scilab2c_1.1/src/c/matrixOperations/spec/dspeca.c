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
#include "max.h"
#include "stdio.h"


void dspeca(double* in, int rows,double* out){
	int i=0, j=0;
	int symmetric=0;
	int INFO=0;
	int iWorkSize = 0;
	double* pdblWork;
	double* outReal;
	double* outImag;
	double* pdblLeftvectors;
	double* pdblRightvectors;
	double* inCopy;
	
	inCopy = (double*)malloc((unsigned int)(rows*rows) * sizeof(double));
	outReal = (double*)malloc((unsigned int)rows * sizeof(double));
	outImag = (double*)malloc((unsigned int)rows * sizeof(double));
	pdblLeftvectors=NULL;
	pdblRightvectors=NULL;
	
	
	iWorkSize = 3*rows;
	pdblWork = (double*)malloc((unsigned int)iWorkSize * sizeof(double));
	
	for(i=0;i<rows*rows;i++) inCopy[i]=in[i];
	/* look if the matrix is symmetric */
	for (i=0;i<rows;i++)
		for (j=0;j<rows;j++) 
			if (in[i*rows+j]!=in[i+j*rows]) break;
	
	/* the matrix is symmetric if the 2 loops goes to end i.e
		i==rows and j==rows */
	if ((i==rows)&&(j==rows)) symmetric=1;
	
	
	/* apply lapack function according to symmetry */
	if(symmetric){		
		C2F(dsyev)( "N", "U", &rows, in, &rows, outReal, pdblWork, &iWorkSize, &INFO );
	}
	else {
		C2F(dgeev)( "N", "N", &rows, inCopy, &rows, outReal, outImag,
        			pdblLeftvectors, &rows, pdblRightvectors, &rows, pdblWork, &iWorkSize, &INFO );
	}
	
	for (i=0;i<rows;i++) out[i]=outReal[i];


	free(inCopy);
	free(outReal);
	free(outImag);
	free(pdblWork);
	free(pdblLeftvectors);
	free(pdblRightvectors);
}

