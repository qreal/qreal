
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
#include "crossCorr.h"
#include "conv2d.h"
#include "conj.h"

void zcrossCorra(doubleComplex* in1, int rows1, int cols1, doubleComplex* in2, int rows2, int cols2, doubleComplex* out){
	doubleComplex *in2Copy;
	int i;

	in2Copy=(doubleComplex *)malloc((unsigned int)(rows2*cols2)*sizeof(doubleComplex));

	/* We change in2 to be in appropriate form in in2Copy*/
	for (i=0;i<(rows2*cols2);i++) in2Copy[i]=zconjs(in2[rows2*cols2-1-i]);

	zconv2da(in1, rows1, cols1, in2Copy, rows2, cols2, out);

	free(in2Copy);

}


