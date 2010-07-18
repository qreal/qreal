
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

#ifndef __MAGNITUDE_H__
#define __MAGNITUDE_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_MATOPS float smagns(float in);
EXTERN_MATOPS float smagna(float* in, int rows, int cols);

EXTERN_MATOPS double dmagns(double in);
EXTERN_MATOPS double dmagna(double* in, int rows, int cols);

EXTERN_MATOPS float cmagns(floatComplex in);
EXTERN_MATOPS float cmagna(floatComplex* in, int rows, int cols);

EXTERN_MATOPS double zmagns(doubleComplex in);
EXTERN_MATOPS double zmagna(doubleComplex* in, int rows, int cols);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* __MAGNITUDE_H__*/
