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
#include <stdio.h>

void sconva(float *in1, int size1, float *in2,int size2, float *out){
	int i;
	floatComplex *in1Cpx, *in2Cpx, *result;
	
	in1Cpx=(floatComplex *)malloc(2*(unsigned int)size1*sizeof(float));	
	in2Cpx=(floatComplex *)malloc(2*(unsigned int)size2*sizeof(float));
	
	for (i=0;i<size1;i++){
	in1Cpx[i]=FloatComplex(in1[i],0);
	}

	for (i=0;i<size2;i++){
	in2Cpx[i]=FloatComplex(in2[i],0);
	}

	result=(floatComplex *)malloc(2*(unsigned int)(size1+size2-1)*sizeof(float));	

	cconva(in1Cpx,size1,in2Cpx,size2,result);

	creala(result,size1+size2-1,out);

	free(result);
	free(in1Cpx);
	free(in2Cpx);
}



