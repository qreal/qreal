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


#ifndef WITHOUT_LAPACK
#include "lapack.h"
#else
#include "sqrt.h"
#endif
#include "chol.h"
#include <malloc.h>
#include <stdio.h>

void schola(float * in, int size, float *out){
	/* param in : input matrix (square matrix)
	   param size : number of rows or columns
	   param U : output upper triangular matrix
	*/
	
#ifndef WITHOUT_LAPACK

	/*We have to use a double, copy of in,
	  cause dpotrf works only with double, not with float*/
	double* tmp;
	int i=0,j=0,info=0;
	
	tmp=(double*)malloc((unsigned int)(size*size)*sizeof(double));
	for (i=0;i<size*size;i++) tmp[i]=(double)in[i];
	
	C2F(dpotrf)("U", &size, tmp, &size, &info);
	
	for (i=0;i<size*size;i++) out[i]=(float)tmp[i];
	
	free(tmp);
	
	
	/*Zeros in the lower triangular part*/
	for (i=0;i<size;i++){
		for (j=i+1;j<size;j++){
			out[j+i*size]=0;
		}
	}
	
#else
	/* Do not use Lapack functions*/
	int i=0, j=0, k=0;
	float tmp=0, accu=0;
	for (i=0;i<size;i++){
		accu=0;
		for (j=0;j<i;j++){
			tmp=in[i*size+j];
			for (k=0;k<j;k++){
				tmp-=out[i*size+k]*out[j*size+k];
			}
			out[i*size+j]=tmp/out[j*size+j];
			accu+=out[i*size+j]*out[i*size+j];
		}
		out[i*size+i]=ssqrts(in[i*size+i]-accu);	
	}
	
	/*Zeros in the lower triangular part*/
	for (i=0;i<size;i++){
		for (j=i+1;j<size;j++){
			out[j+i*size]=0;
		}
	}

#endif
	
}



