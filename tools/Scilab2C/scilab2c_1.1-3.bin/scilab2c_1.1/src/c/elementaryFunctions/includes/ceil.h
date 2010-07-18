/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - INRIA - Arnaud Torset
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CEIL_H__
#define __CEIL_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"


#ifdef  __cplusplus
extern "C" {
#endif
/*
	ceil(x) returns an integer matrix made of rounded up elements 
*/

EXTERN_ELEMFUNCT float	sceils(float in);

EXTERN_ELEMFUNCT double dceils(double in);

EXTERN_ELEMFUNCT floatComplex cceils(floatComplex in);

EXTERN_ELEMFUNCT doubleComplex zceils(doubleComplex in);

EXTERN_ELEMFUNCT void	sceila(float* in, int size, float* out);

EXTERN_ELEMFUNCT void	dceila(double* in, int size, double* out);

EXTERN_ELEMFUNCT void	cceila(floatComplex* in, int size, floatComplex* out);

EXTERN_ELEMFUNCT void	zceila(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__CEIL_H__ */

