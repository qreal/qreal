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

#ifndef __TAN_H__
#define __TAN_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*
** Compute Tangeant for different types .
*/

/*
** \brief Float Tangeant function
** \param in : input value.
*/
EXTERN_ELEMFUNCT float		stans(float in);

/*
** \brief Double Tangeant function
** \param in : input value.
*/
EXTERN_ELEMFUNCT double		dtans(double in);

/*
** \brief Float Complex Tangeant function
** \param in : input value.
*/
EXTERN_ELEMFUNCT floatComplex	ctans(floatComplex in);

/*
** \brief Double Complex Tangeant function
** \param in : input value.
*/
EXTERN_ELEMFUNCT doubleComplex	ztans(doubleComplex in);

/*
** \brief Float Matrix Tangeant function
** \param in : input array value.
** \param out : input array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		stana(float* in, int size, float* out);

/*
** \brief Double Matrix Tangeant function
** \param in : input array value.
** \param out : input array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		dtana(double* in, int size, double* out);

/*
** \brief Float Complex Matrix Tangeant function
** \param in : input array value.
** \param out : input array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		ctana(floatComplex* in, int size, floatComplex* out);

/*
** \brief Double Complex Matrix Tangeant function
** \param in : input array value.
** \param out : input array value.
** \param size : the size of in and out arrays.
*/
EXTERN_ELEMFUNCT void		ztana(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__TAN_H__ */

