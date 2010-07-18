
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

#include "conv.h"

void zconva(doubleComplex *in1, int size1, doubleComplex *in2,int size2, doubleComplex *out){
	int m1,i;
	doubleComplex *in1b, *in2b, *result;	


	m1=(int) floor(log(size1+size2-1.0) / log(2.0) + 1.0);
	m1=(int) pow(2.0, m1);

	in1b=(doubleComplex *)malloc(2*(unsigned int)m1*sizeof(double));
	for(i=0;i<m1;i++){
		if (i<size1) in1b[i]=in1[i];
		else in1b[i]=DoubleComplex(0,0);
	}
	
	in2b=(doubleComplex *)malloc(2*(unsigned int)m1*sizeof(double));
	for(i=0;i<m1;i++){
		if (i<size2) in2b[i]=in2[i];
		else in2b[i]=DoubleComplex(0,0);
	}

	
	zfftma(in1b,m1,1,in1b);
	zfftma(in2b,m1,1,in2b);

	result=(doubleComplex *)malloc(2*(unsigned int)m1*sizeof(double));
	zmula(in1b,in2b,m1,result);
	zifftma(result,m1,1,result);	
	
	for (i=0;i<size1+size2-1;i++){
		out[i]=result[i];
	}
	
	free(result);
	free(in2b);
	free(in1b);
}
