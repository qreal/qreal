/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MEAN_H__
#define __MEAN_H__

#include "dynlib_statisticsfunctions.h"
#include "division.h"
#include "addition.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*
** \brief Mean of a scalar element, just returns it
*/
#define		smeans(in)			in
#define		srowmeans(in)			in
#define		scolumnmeans(in)		in
#define		smatlabmeans(in)		in

/*
** \brief Mean of a scalar element, just returns it
*/
#define		dmeans(in)			in
#define		drowmeans(in)			in
#define		dcolumnmeans(in)		in
#define		dmatlabmeans(in)		in

/*
** \brief Mean of a scalar element, just returns it
*/
#define		cmeans(in)			in
#define		crowmeans(in)			in
#define		ccolumnmeans(in)		in
#define		cmatlabmeans(in)		in

/*
** \brief Mean of a scalar element, just returns it
*/
#define		zmeans(in)			in
#define		zrowmeans(in)			in
#define		zcolumnmeans(in)		in
#define		zmatlabmeans(in)		in

/*
** \brief Mean of a float array
** \param in the float array to process
** \param size, the size of the array
** \returns the mean.
*/
EXTERN_STATFUNC float		smeana(float *in, int size);
EXTERN_STATFUNC void		srowmeana(float *in, int lines, int columns, float* out);
EXTERN_STATFUNC void		scolumnmeana(float *in, int lines, int columns, float* out);

/*
** \brief Mean of a double array
** \param in the double array to process
** \param size, the size of the array
** \returns the mean.
*/
EXTERN_STATFUNC double		dmeana(double *in, int size);
EXTERN_STATFUNC void		drowmeana(double *in, int lines, int columns, double* out);
EXTERN_STATFUNC void		dcolumnmeana(double *in, int lines, int columns, double* out);

/*
** \brief Mean of a float complex array
** \param in the float array to process
** \param size, the size of the array
** \returns the mean.
*/
EXTERN_STATFUNC floatComplex	cmeana(floatComplex *in, int size);
EXTERN_STATFUNC void		crowmeana(floatComplex *in, int lines, int columns, floatComplex* out);
EXTERN_STATFUNC void		ccolumnmeana(floatComplex *in, int lines, int columns, floatComplex* out);

/*
** \brief Mean of a double complex array
** \param in the double array to process
** \param size, the size of the array
** \returns the mean.
*/
EXTERN_STATFUNC doubleComplex	zmeana(doubleComplex *in, int size);
EXTERN_STATFUNC void		zrowmeana(doubleComplex *in, int lines, int columns, doubleComplex* out);
EXTERN_STATFUNC void		zcolumnmeana(doubleComplex *in, int lines, int columns, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__MEAN_H__ */
