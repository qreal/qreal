/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __RAND_H__
#define __RAND_H__

#include <math.h>
#include "dynlib_auxiliaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
 ** \brief Float Rand function
 ** \return A random float.
 **/
EXTERN_AUXFUNCT float		srands(void);

/**
 ** \brief Double Rand function
 ** \return A random double.
 **/
EXTERN_AUXFUNCT double		drands(void);

/**
 ** \brief Float Complex Rand function
 ** \return A random float complex.
 **/
EXTERN_AUXFUNCT floatComplex	crands(void);

/**
 ** \brief Double Complex Rand function
 ** \return A random double complex.
 **/
EXTERN_AUXFUNCT doubleComplex	zrands(void);

/**
 ** \brief Float Array Rand function
 ** \return A random float array.
 **/
EXTERN_AUXFUNCT void		sranda(float *out, int size);

/**
 ** \brief Double Array Rand function
 ** \return A random double array.
 **/
EXTERN_AUXFUNCT void		dranda(double *out, int size);

/**
 ** \brief Float Complex Array Rand function
 ** \return A random float complex array.
 **/
EXTERN_AUXFUNCT void		cranda(floatComplex *out, int size);

/**
 ** \brief Double Complex Array Rand function
 ** \return A random double complex array.
 **/
EXTERN_AUXFUNCT void		zranda(doubleComplex *out, int size);

#ifdef  __cplusplus
} /* extern "C" */
#endif



#endif /* !__RAND_H__ */
