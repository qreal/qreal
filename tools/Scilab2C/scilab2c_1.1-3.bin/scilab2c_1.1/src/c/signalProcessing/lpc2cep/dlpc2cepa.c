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
#include "logm.h"
#include "ifft.h"

void dlpc2cepa(double* in, int size, double* out){
	int i;
	doubleComplex *inCpx;

	inCpx = (doubleComplex *) malloc ((unsigned int)(size*size)*sizeof(doubleComplex));

	for (i=0;i<size*size;i++)
		inCpx[i]=DoubleComplex(in[i],0);


	zfftma(inCpx,size, size, inCpx);
	zlogma(inCpx,size,inCpx);
	zifftma(inCpx,size, size, inCpx);

 	zreala(inCpx,size*size,out);

 	free(inCpx);
}

