/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __POW_H__
#define __POW_H__

#include "dynlib_elementaryfunctions.h"
#include "multiplication.h"

#ifdef  __cplusplus
extern "C" {
#endif
EXTERN_ELEMFUNCT float		spows(float value, float expand);

EXTERN_ELEMFUNCT double		dpows(double value, double expand);

EXTERN_ELEMFUNCT floatComplex	cpows(floatComplex value, floatComplex expand);

EXTERN_ELEMFUNCT doubleComplex	zpows(doubleComplex value, doubleComplex expand);

EXTERN_ELEMFUNCT void		spowa(float *value, int size, float* expand, float *out);

EXTERN_ELEMFUNCT void		dpowa(double *value, int size, double* expand, double *out);

EXTERN_ELEMFUNCT void		cpowa(floatComplex *value, int size, floatComplex* expand, floatComplex *out);

EXTERN_ELEMFUNCT void		zpowa(doubleComplex *value, int size, doubleComplex* expand, doubleComplex *out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__POW_H__ */
