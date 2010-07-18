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
#include "lpc2cep.h"
#include "fft.h"
#include "ifft.h"
#include "logm.h"

void slpc2cepa(float *in, int size, float*out){
	int i;
	floatComplex* inCpx;

	/* Copy in in a FloatComplex*/
	inCpx=(floatComplex*)malloc((unsigned int)(size*size)*sizeof(floatComplex));
	for (i=0;i<size*size;i++)
		inCpx[i]=FloatComplex(in[i],0);

	cfftma(inCpx,size,size,inCpx);
	clogma(inCpx,size,inCpx);
	cifftma(inCpx,size,size,inCpx);

	creala(inCpx,size*size,out);

 	free(inCpx);
}


