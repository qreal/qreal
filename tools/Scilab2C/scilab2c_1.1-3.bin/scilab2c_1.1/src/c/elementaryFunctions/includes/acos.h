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

#ifndef __ACOS_H__
#define __ACOS_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif


/*
** Compute ArcCosine for different types .
*/

/*
** \brief Float ArcCosine function
** \param in : input array value.
*/
EXTERN_ELEMFUNCT float		sacoss(float in);

/*
** \brief Double ArcCosine function
** \param in : input array value.
*/
EXTERN_ELEMFUNCT double		dacoss(double in);

/*
** \brief Float Complex ArcCosine function
** \param in : input array value.
*/
EXTERN_ELEMFUNCT floatComplex	cacoss(floatComplex in);

/*
** \brief Double Complex ArcCosine function
** \param in : input array value.
*/
EXTERN_ELEMFUNCT doubleComplex	zacoss(doubleComplex in);

/*
** \brief Float Matrix ArcCosine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		sacosa(float* in, int size, float* out);

/*
** \brief Double Matrix ArcCosine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		dacosa(double* in, int size, double* out);

/*
** \brief Float Complex Matrix ArcCosine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		cacosa(floatComplex* in, int size, floatComplex* out);

/*
** \brief Double Complex Matrix ArcCosine function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		zacosa(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__ACOS_H__ */
