
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

#ifndef __SQUAREDMAGNITUDE_H__
#define __SQUAREDMAGNITUDE_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_MATOPS float ssquMagns(float in);
EXTERN_MATOPS float ssquMagna(float* in, int rows, int cols);

EXTERN_MATOPS double dsquMagns(double in);
EXTERN_MATOPS double dsquMagna(double* in, int rows, int cols);

EXTERN_MATOPS float csquMagns(floatComplex in);
EXTERN_MATOPS float csquMagna(floatComplex* in, int rows, int cols);

EXTERN_MATOPS double zsquMagns(doubleComplex in);
EXTERN_MATOPS double zsquMagna(doubleComplex* in, int rows, int cols);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* __SQUAREDMAGNITUDE_H__*/
