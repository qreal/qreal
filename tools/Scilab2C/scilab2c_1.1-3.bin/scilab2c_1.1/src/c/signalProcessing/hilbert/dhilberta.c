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
 

#include "hilbert.h"
#include "fft.h"
#include "ifft.h"
#include "multiplication.h"
#include <stdio.h>

void dhilberta (double* in, int rows, int cols, doubleComplex *out){
	int i=0,size=rows*cols;
	

	for (i=0;i<rows*cols;i++) out[i]=DoubleComplex(in[i],0);


	zfftma(out, rows, cols, out);

	for (i=0;i<size;i++){
		if ((i>0)&&(i<((size+1)/2))) out[i] = zmuls(out[i],DoubleComplex(2,0));
		if (i>(size/2)) out[i] = zmuls(out[i],DoubleComplex(0,0));
	}

	zifftma(out, rows, cols,out);
}


