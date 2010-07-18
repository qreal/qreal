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
#include "ifft.h"
#include "zeros.h"


void sifftma ( float* in , int rows, int cols, float* out){
	float* ZEROS;
	floatComplex* inCpx;
	floatComplex* outCpx;

	ZEROS = (float*)malloc((unsigned int)(rows*cols)*sizeof(float));
	outCpx = (floatComplex*)malloc((unsigned int)(rows*cols)*sizeof(floatComplex));

	szerosa(ZEROS,rows,cols);
	inCpx=FloatComplexMatrix(in,ZEROS,rows*cols);

	cifftma(inCpx,rows,cols,outCpx);

	creala(outCpx, rows*cols, out);

	free(ZEROS);
	free(outCpx);
}
