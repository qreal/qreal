/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Arnaud Torset
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <malloc.h>
#include "fft.h"
#include "zeros.h"


void dfftma ( double* in , int rows, int cols, double* out){
	double* ZEROS;
	doubleComplex* inCpx;
	doubleComplex* outCpx;

	ZEROS = (double*) malloc((unsigned int)(rows*cols)*sizeof(double));
	outCpx = (doubleComplex*) malloc((unsigned int)(rows*cols)*sizeof(doubleComplex));

	dzerosa(ZEROS,rows,cols);
	inCpx=DoubleComplexMatrix(in,ZEROS,rows*cols);

	zfftma(inCpx,rows,cols,outCpx);

	zreala(outCpx, rows*cols, out);
}
