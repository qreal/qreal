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




#ifndef __DIST_H__
#define __DIST_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif
	/* Computes the euclidian distance 
	   between 2 scalars/arrays.
	   We assume both arrays have the same
	   numbers of lines and columns.*/

EXTERN_MATOPS float sdists( float in1, float in2);
EXTERN_MATOPS float sdista( float* in1, float* in2, int lines, int columns);

EXTERN_MATOPS double ddists( double in1, double in2);
EXTERN_MATOPS double ddista( double* in1, double* in2, int lines, int columns);

EXTERN_MATOPS float cdists( floatComplex in1, floatComplex in2);
EXTERN_MATOPS float cdista( floatComplex* in1, floatComplex* in2, int lines, int columns);

EXTERN_MATOPS double zdists( doubleComplex in1, doubleComplex in2);
EXTERN_MATOPS double zdista( doubleComplex* in1, doubleComplex* in2, int lines, int columns);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /*__DIST_H__*/

