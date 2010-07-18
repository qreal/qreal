/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Arnaud TORSET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef __LOGM_H__
#define __LOGM_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_MATOPS void slogma (float* in, int size, float* out);

EXTERN_MATOPS void dlogma (double* in, int size, double* out);

EXTERN_MATOPS void clogma (floatComplex* in, int size, floatComplex* out);

EXTERN_MATOPS void zlogma (doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* __LOGM_H__ */
