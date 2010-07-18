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

#ifndef __ATANH_H__
#define __ATANH_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"


#ifdef  __cplusplus
extern "C" {
#endif

/*
** Compute Hyperbolic ArcTangeant for different types .
*/

/*
** \brief Float Hyperbolic ArcTangeant function
** \param in : input value.
*/
EXTERN_ELEMFUNCT float		satanhs(float in);

/*
** \brief Double Hyperbolic ArcTangeant function
** \param in : input value.
*/
EXTERN_ELEMFUNCT double		datanhs(double in);

/*
** \brief Float Complex Hyperbolic ArcTangeant function
** \param in : input value.
*/
EXTERN_ELEMFUNCT floatComplex	catanhs(floatComplex in);

/*
** \brief Double Complex Hyperbolic ArcTangeant function
** \param in : input value.
*/
EXTERN_ELEMFUNCT doubleComplex	zatanhs(doubleComplex in);

/*
** \brief Float Matrix Hyperbolic ArcTangeant function
** \param in : input array value.
** \param out : input array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		satanha(float* in, int size, float* out);

/*
** \brief Double Matrix Hyperbolic ArcTangeant function
** \param in : input array value.
** \param out : input array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		datanha(double* in, int size, double* out);

/*
** \brief Float Complex Matrix Hyperbolic ArcTangeant function
** \param in : input array value.
** \param out : input array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		catanha(floatComplex* in, int size, floatComplex* out);

/*
** \brief Double Complex Matrix Hyperbolic ArcTangeant function
** \param in : input array value.
** \param out : input array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		zatanha(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__ATANH_H__ */

