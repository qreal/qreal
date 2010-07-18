/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __EXP10_H__
#define __EXP10_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"


#ifdef  __cplusplus
extern "C" {
#endif
/*
** Compute 10 based Exponential for different types .
*/

/*
** \brief Float 10 based Exponential function
** \param in : input value.
*/
EXTERN_ELEMFUNCT float		sexp10s(float in);

/*
** \brief Double Exp10onential function
** \param in : input value.
*/
EXTERN_ELEMFUNCT double		dexp10s(double in);

/*
** \brief Float Complex 10 based Exponential function
** \param in : input value.
*/
EXTERN_ELEMFUNCT floatComplex	cexp10s(floatComplex in);

/*
** \brief Double Complex 10 based Exponential function
** \param in : input value.
*/
EXTERN_ELEMFUNCT doubleComplex	zexp10s(doubleComplex in);

/*
** \brief Float Matrix 10 based Exponential function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		sexp10a(float* in, int size, float* out);

/*
** \brief Double Matrix 10 based Exponential function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		dexp10a(double* in, int size, double* out);

/*
** \brief Float Complex Matrix 10 based Exponential function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		cexp10a(floatComplex* in, int size, floatComplex* out);

/*
** \brief Double Complex Matrix 10 based Exponential function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		zexp10a(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__EXP10_H__ */

