/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __VARIANCE_H__
#define __VARIANCE_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "dynlib_statisticsfunctions.h"
#include "subtraction.h"
#include "division.h"

#include "pow.h"
#include "sum.h"

#include "mean.h"
#include "matrixTranspose.h"


#ifdef  __cplusplus
extern "C" {
#endif





/*
** \brief Variance of a scalar element, just returns it
*/
#define		svariances(in)			    in
#define		srowvariances(in)			in
#define		scolumnvariances(in)		in


/*
** \brief Variance of a scalar element, just returns it
*/
#define		dvariances(in)			    in
#define		drowvariances(in)			in
#define		dcolumnvariances(in)		in


/*
** \brief Variance of a scalar element, just returns it
*/
#define		cvariances(in)			    in
#define		crowvariances(in)			in
#define		ccolumnvariances(in)		in


/*
** \brief Variance of a scalar element, just returns it
*/
#define		zvariances(in)			    in
#define		zrowvariances(in)			in
#define		zcolumnvariances(in)		in


/*
** \brief Variance of a float array
** \param in the float array to process
** \param size, the size of the array
** \returns the variance.
*/
EXTERN_STATFUNC float		svariancea(float *in, int size);
EXTERN_STATFUNC void		srowvariancea(float *in, int lines, int columns, float* out);
EXTERN_STATFUNC void		scolumnvariancea(float *in, int lines, int columns, float* out);

/*
** \brief Variance of a double array
** \param in the double array to process
** \param size, the size of the array
** \returns the variance.
*/
EXTERN_STATFUNC double		dvariancea(double *in, int size);
EXTERN_STATFUNC void		drowvariancea(double *in, int lines, int columns, double* out);
EXTERN_STATFUNC void		dcolumnvariancea(double *in, int lines, int columns, double* out);

/*
** \brief Variance of a float complex array
** \param in the float array to process
** \param size, the size of the array
** \returns the variance.
*/
EXTERN_STATFUNC floatComplex	cvariancea(floatComplex *in, int size);
EXTERN_STATFUNC void		crowvariancea(floatComplex *in, int lines, int columns, floatComplex* out);
EXTERN_STATFUNC void		ccolumnvariancea(floatComplex *in, int lines, int columns, floatComplex* out);

/*
** \brief Variance of a double complex array
** \param in the double array to process
** \param size, the size of the array
** \returns the variance.
*/
EXTERN_STATFUNC doubleComplex	zvariancea(doubleComplex *in, int size);
EXTERN_STATFUNC void		zrowvariancea(doubleComplex *in, int lines, int columns, doubleComplex* out);
EXTERN_STATFUNC void		zcolumnvariancea(doubleComplex *in, int lines, int columns, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__VARIANCE_H__ */
