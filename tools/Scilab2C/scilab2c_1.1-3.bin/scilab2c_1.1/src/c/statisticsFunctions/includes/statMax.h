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

#ifndef __STAT_MAX_H__
#define __STAT_MAX_H__

#include "dynlib_statisticsfunctions.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
** \brief Sum of a scalar element, just returns it
*/
#define		smaxs(in)				in
#define		srowmaxs(in)			in
#define		scolumnmaxs(in)			in
#define		smatlabmaxs(in)			in

/*
** \brief Sum of a scalar element, just returns it
*/
#define		dmaxs(in)				in
#define		drowmaxs(in)			in
#define		dcolumnmaxs(in)			in
#define		dmatlabmaxs(in)			in

/*
** \brief Sum of a float array
** \param in the float array to process
** \param size, the size of the array
** \returns the max.
*/
EXTERN_STATFUNC float		smaxa(float *in, int size);
EXTERN_STATFUNC void		srowmaxa(float *in, int lines, int columns, float* out);
EXTERN_STATFUNC void		scolumnmaxa(float *in, int lines, int columns, float* out);

/*
** \brief Sum of a double array
** \param in the double array to process
** \param size, the size of the array
** \returns the max.
*/
EXTERN_STATFUNC double		dmaxa(double *in, int size);
EXTERN_STATFUNC void		drowmaxa(double *in, int lines, int columns, double* out);
EXTERN_STATFUNC void		dcolumnmaxa(double *in, int lines, int columns, double* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__STAT_MAX_H__ */
