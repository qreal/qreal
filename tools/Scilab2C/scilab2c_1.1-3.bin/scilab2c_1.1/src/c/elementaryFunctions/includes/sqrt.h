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

#ifndef __SQRT_H__
#define __SQRT_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*
** Compute Square Root for different types .
*/

/*
** \brief Float Square Root function
** \param in : input value.
*/
EXTERN_ELEMFUNCT float		ssqrts(float in);

/*
** \brief Double Square Root function
** \param in : input value.
*/
EXTERN_ELEMFUNCT double		dsqrts(double in);

/*
** \brief Float Complex Square Root function
** \param in : input value.
*/
EXTERN_ELEMFUNCT floatComplex	csqrts(floatComplex in);

/*
** \brief Double Complex Square Root function
** \param in : input value.
*/
EXTERN_ELEMFUNCT doubleComplex	zsqrts(doubleComplex in);

/*
** \brief Float Matrix Square Root function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		ssqrta(float* in, int size, float* out);

/*
** \brief Double Matrix Square Root function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		dsqrta(double* in, int size, double* out);

/*
** \brief Float Complex Matrix Square Root function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		csqrta(floatComplex* in, int size, floatComplex* out);

/*
** \brief Double Complex Matrix Square Root function
** \param in : input array value.
** \param out : output array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		zsqrta(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__SQRT_H__ */
