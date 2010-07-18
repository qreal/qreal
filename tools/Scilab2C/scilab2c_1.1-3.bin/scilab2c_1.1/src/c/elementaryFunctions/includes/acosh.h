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

#ifndef __ACOSH_H__
#define __ACOSH_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"


#ifdef  __cplusplus
extern "C" {
#endif


/*
** Compute Hyperbolic Hyperbolic ArcCosine for different types .
*/

/*
** \brief Float Hyperbolic ArcCosine function
** \param in : input array value.
*/
EXTERN_ELEMFUNCT float		sacoshs(float in);

/*
** \brief Double Hyperbolic ArcCosine function
** \param in : input array value.
*/
EXTERN_ELEMFUNCT double		dacoshs(double in);

/*
** \brief Float Complex Hyperbolic ArcCosine function
** \param in : input array value.
*/
EXTERN_ELEMFUNCT floatComplex	cacoshs(floatComplex in);

/*
** \brief Double Complex Hyperbolic ArcCosine function
** \param in : input array value.
*/
EXTERN_ELEMFUNCT doubleComplex	zacoshs(doubleComplex in);

/*
** \brief Float Matrix Hyperbolic ArcCosine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		sacosha(float* in, int size, float* out);

/*
** \brief Double Matrix Hyperbolic ArcCosine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		dacosha(double* in, int size, double* out);

/*
** \brief Float Complex Matrix Hyperbolic ArcCosine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		cacosha(floatComplex* in, int size, floatComplex* out);

/*
** \brief Double Complex Matrix Hyperbolic ArcCosine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		zacosha(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__ACOSH_H__ */
