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


#ifndef __FILL_H__
#define __FILL_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*filling of a array with constant*/

EXTERN_MATOPS void dfilla (double* in, int rows, int cols, double constant);

EXTERN_MATOPS void sfilla (float* in, int rows, int cols, float constant);

EXTERN_MATOPS void cfilla (floatComplex* in, int rows, int cols, floatComplex constant);

EXTERN_MATOPS void zfilla (doubleComplex* in, int rows, int cols, doubleComplex constant);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* __FILL_H__ */


