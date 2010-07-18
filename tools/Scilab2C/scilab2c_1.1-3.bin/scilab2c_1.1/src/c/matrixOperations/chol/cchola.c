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

#ifndef WITHOUT_LAPACK
#include "lapack.h"
#else
#include "multiplication.h"
#include "division.h"
#include "subtraction.h"
#include "sqrt.h"
#endif
#include "chol.h"


void cchola(floatComplex * in, int size, floatComplex *out){
	/* param in : input matrix (square matrix)
	   param size : number of rows or columns
	   param out : output upper triangular matrix
	   */
	
	/* 
	   We compute U as A=Ut*U
	   */
	   
	   
#ifndef WITHOUT_LAPACK	

	/*We have to use a double, copy of in,
	  cause dpotrf works only with double, not with float*/
	doubleComplex* tmp;
	int i=0, j=0, info=0;
	
	tmp=(doubleComplex*)malloc((unsigned int)(size*size)*sizeof(doubleComplex));
	for(i=0;i<size*size;i++) tmp[i]=DoubleComplex((double)creals(in[i]),(double)cimags(in[i]));
	
	zpotrf_("U", &size, tmp, &size, &info);

	for(i=0;i<size*size;i++) out[i]=FloatComplex((float)zreals(tmp[i]),(float)zimags(tmp[i]));
	
	/*Zeros in the lower triangular part*/
	for (i=0;i<size;i++){
		for (j=i+1;j<size;j++){
			out[j+i*size]=FloatComplex(0,0);
		}
	}
	free(tmp);
#else
	/* Do not use Lapack functions*/

	int i=0, j=0, k=0;
	floatComplex tmp,transposee;
	float accu;

	for (i=0;i<size;i++){
		accu=0.0f;
		for (j=0;j<i;j++){
			tmp=in[i*size+j];
			for (k=0;k<j;k++){
				transposee=FloatComplex(creals(out[j*size+k]),-cimags(out[j*size+k]));
				tmp = cdiffs(tmp,cmuls(out[i*size+k],transposee));
			}
			out[i*size+j]= crdivs(tmp,out[j*size+j]);
			accu += creals(out[i*size+j])*creals(out[i*size+j]);
			accu += cimags(out[i*size+j])*cimags(out[i*size+j]);
		}
		tmp = FloatComplex(creals(in[i*size+i])-accu,0);
		out[i*size+i]=csqrts(tmp);	
	}
	
	/*Zeros in the lower triangular part*/
	for (i=0;i<size;i++){
		for (j=i+1;j<size;j++){
			out[j+i*size]=FloatComplex(0,0);
		}
	}

#endif	
}


