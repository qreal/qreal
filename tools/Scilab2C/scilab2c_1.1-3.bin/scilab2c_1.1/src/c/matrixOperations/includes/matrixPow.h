
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

#ifndef __MATRIXPOW_H__
#define __MATRIXPOW_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* 
   powm is only working on square matrix
   so the size is limited to rows
*/
EXTERN_MATOPS void spowma(float* in, int rows, float expand, float* out);

EXTERN_MATOPS void dpowma(double* in, int rows, double expand, double* out);

EXTERN_MATOPS void cpowma(floatComplex* in, int rows, floatComplex expand, floatComplex* out);

EXTERN_MATOPS void zpowma(doubleComplex* in, int rows, doubleComplex expand, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif



#endif/*__MATRIXPOW_H__*/
