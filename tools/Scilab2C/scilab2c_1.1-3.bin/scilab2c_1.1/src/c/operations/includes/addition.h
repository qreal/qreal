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

#ifndef __ADDITION_H__
#define __ADDITION_H__

#include "dynlib_operations.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
**
** WARNING WE ASSUME MATRIXES TO BE CONSCISTENT
** size1 = size2;
**
*/

/*
** \brief Compute an addition with floats.
** \param in1 : input float.
** \param in2 : input float.
** \return : in1 + in2
*/
EXTERN_OPERATIONS float	sadds(float in1, float in2);

/*
** \brief Compute an addition element ways for floats.
** \param in1 : input array.
** \param size1 : size of in1 array.
** \param in2 : input arry.
** \param size2 : size of in2 array.
** \param out : array that contains the addition in1 + in2.
*/
EXTERN_OPERATIONS void	sadda(float *in1, int size1,
	       float *in2, int size2,
	       float *out);

/*
** \brief Compute an addition with double.
** \param in1 : input double.
** \param in2 : input double.
** \return : in1 + in2
*/
EXTERN_OPERATIONS double	dadds(double in1, double in2);

/*
** \brief Compute an addition element ways for double.
** \param in1 : input array.
** \param size1 : size of in1 array.
** \param in2 : input arry.
** \param size2 : size of in2 array.
** \param out : array that contains the addition in1 + in2.
*/
EXTERN_OPERATIONS void	dadda(double *in1, int size1,
	       double *in2, int size2,
	       double * out);

/*
** \brief Compute an addition with floats Complex.
** \param in1 : input float complex.
** \param in2 : input float complex.
** \return : in1 + in2
*/
EXTERN_OPERATIONS floatComplex	cadds(floatComplex in1, floatComplex in2);

/*
** \brief Compute an addition element ways for complex single precision.
** \param in1 : input array.
** \param size1 : size of in1 array.
** \param in2 : input arry.
** \param size2 : size of in2 array.
** \param out : array that contains the addition in1 + in2.
*/
EXTERN_OPERATIONS void	cadda(floatComplex *in1, int size1,
	       floatComplex *in2, int size2,
	       floatComplex *out);

/*
** \brief Compute an addition with double complex.
** \param in1 : input double complex.
** \param in2 : input double conplex.
** \return : in1 + in2
*/
EXTERN_OPERATIONS doubleComplex	zadds(doubleComplex in1, doubleComplex in2);

/*
** \brief Compute an addition element ways for complex double precision.
** \param in1 : input array.
** \param size1 : size of in1 array.
** \param in2 : input arry.
** \param size2 : size of in2 array.
** \param out : array that contains the addition in1 + in2.
*/
EXTERN_OPERATIONS void	zadda(doubleComplex *in1, int size1,
	       doubleComplex *in2, int size2,
	       doubleComplex *out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__ADDITION_H__ */
