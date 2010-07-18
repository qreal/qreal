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

#ifndef __ASIN_H__
#define __ASIN_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"


#ifdef  __cplusplus
extern "C" {
#endif

/*
** Compute ArcSine for different types .
*/

/*
** \brief Float ArcSine function
** \param in : input value.
*/
EXTERN_ELEMFUNCT float		sasins(float in);

/*
** \brief Double ArcSine function
** \param in : input value.
*/
EXTERN_ELEMFUNCT double		dasins(double in);

/*
** \brief Float Complex ArcSine function
** \param in : input value.
*/
EXTERN_ELEMFUNCT floatComplex	casins(floatComplex in);

/*
** \brief Double Complex ArcSine function
** \param in : input value.
*/
EXTERN_ELEMFUNCT doubleComplex	zasins(doubleComplex in);

/*
** \brief Float Matrix ArcSine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		sasina(float* in, int size, float* out);

/*
** \brief Double Matrix ArcSine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		dasina(double* in, int size, double* out);

/*
** \brief Float Complex Matrix ArcSine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		casina(floatComplex* in, int size, floatComplex* out);

/*
** \brief Double Complex Matrix ArcSine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		zasina(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__ASIN_H__ */
