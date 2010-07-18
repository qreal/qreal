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
#include "multiplication.h"
#include "division.h"
#include "subtraction.h"
#include "sqrt.h"
#endif

#include "chol.h"
#include <malloc.h>
#include <stdio.h>

void zchola(doubleComplex * in, int size, doubleComplex *out){
	/* param in : input matrix (square matrix)
	   param size : number of rows or columns
	   param out : output upper triangular matrix
	*/
	
#ifndef WITHOUT_LAPACK	
	int i=0, j=0, info=0;
	
	for(i=0;i<size*size;i++)	out[i]=in[i];

	C2F(zpotrf)("U", &size, out, &size, &info);
	
	/*Zeros in the lower triangular part*/
	for (i=0;i<size;i++){
		for (j=i+1;j<size;j++){
			out[j+i*size]=DoubleComplex(0,0);
		}
	}

#else
	/* Do not use Lapack functions*/

	int i=0, j=0, k=0;
	doubleComplex tmp,transposee;
	double accu;
	
	for (i=0;i<size;i++){
		accu=0.0f;
		for (j=0;j<i;j++){
			tmp=in[i*size+j];
			for (k=0;k<j;k++){
				transposee=DoubleComplex(zreals(out[j*size+k]),-zimags(out[j*size+k]));
				tmp = zdiffs(tmp,zmuls(out[i*size+k],transposee));
			}
			out[i*size+j]= zrdivs(tmp,out[j*size+j]);
			accu += zreals(out[i*size+j])*zreals(out[i*size+j]);
			accu += zimags(out[i*size+j])*zimags(out[i*size+j]);
		}
		tmp = DoubleComplex(zreals(in[i*size+i])-accu,0);
		out[i*size+i]=zsqrts(tmp);	
	}
	
	/*Zeros in the lower triangular part*/
	for (i=0;i<size;i++){
		for (j=i+1;j<size;j++){
			out[j+i*size]=DoubleComplex(0,0);
		}
	}


	
#endif		
}


