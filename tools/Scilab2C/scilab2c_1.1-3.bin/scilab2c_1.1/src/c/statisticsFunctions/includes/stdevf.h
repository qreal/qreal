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

#ifndef __STDEVF_H__
#define __STDEVF_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "dynlib_statisticsfunctions.h"
#include "subtraction.h"
#include "division.h"


#include "pow.h"
#include "sum.h"
#include "size.h"
#include "sqrt.h"
#include "meanf.h"



#ifdef  __cplusplus
extern "C" {
#endif





/*
** \brief Standard deviation of a scalar element, just returns it
*/
#define		sstdevfs(in1,in2)			    0.0f
#define		srowstdevfs(in1,in2)			0.0f
#define		scolumnstdevfs(in1,in2)		0.0f


/*
** \brief Standard deviation of a scalar element, just returns it
*/
#define		dstdevfs(in1,in2)			    0.0
#define		drowstdevfs(in1,in2)			0.0
#define		dcolumnstdevfs(in1,in2)		0.0


/*
** \brief Standard deviation of a scalar element, just returns it
*/
#define		cstdevfs(in1,in2)			    FloatComplex(0.0f , 0.0f)
#define		crowstdevfs(in1,in2)			FloatComplex(0.0f , 0.0f)
#define		ccolumnstdevfs(in1,in2)		FloatComplex(0.0f , 0.0f)


/*
** \brief Standard deviation of a scalar element, just returns it
*/
#define		zstdevfs(in1,in2)			    DoubleComplex(0.0 , 0.0)
#define		zrowstdevfs(in1,in2)			DoubleComplex(0.0 , 0.0)
#define		zcolumnstdevfs(in1,in2)		DoubleComplex(0.0 , 0.0)


/*
** \brief Standard deviation of a float array
** \param in the float array to process
** \param size, the size of the array
** \returns the stdevf.
*/
EXTERN_STATFUNC float		sstdevfa(float *in1, int lines, int columns, float *in2);
EXTERN_STATFUNC void		srowstdevfa(float *in1, int lines, int columns, float *in2, float* out);
EXTERN_STATFUNC void		scolumnstdevfa(float *in1, int lines, int columns, float *in2, float* out);

/*
** \brief Standard deviation of a double array
** \param in the double array to process
** \param size, the size of the array
** \returns the stdevf.
*/
EXTERN_STATFUNC double		dstdevfa(double *in1, int lines, int columns, double *in2);
EXTERN_STATFUNC void		drowstdevfa(double *in1, int lines, int columns, double *in2, double* out);
EXTERN_STATFUNC void		dcolumnstdevfa(double *in1, int lines, int columns, double *in2, double* out);

/*
** \brief Standard deviation of a float complex array
** \param in the float array to process
** \param size, the size of the array
** \returns the stdevf.
*/
EXTERN_STATFUNC floatComplex	cstdevfa(floatComplex *in1, int lines, int columns, floatComplex *in2);
EXTERN_STATFUNC void		crowstdevfa(floatComplex *in1, int lines, int columns, floatComplex *in2, floatComplex* out);
EXTERN_STATFUNC void		ccolumnstdevfa(floatComplex *in1, int lines, int columns, floatComplex *in2, floatComplex* out);

/*
** \brief Standard deviation of a double complex array
** \param in the double array to process
** \param size, the size of the array
** \returns the stdevf.
*/
EXTERN_STATFUNC doubleComplex	zstdevfa(doubleComplex *in1, int lines, int columns, doubleComplex *in2);
EXTERN_STATFUNC void		zrowstdevfa(doubleComplex *in1, int lines, int columns, doubleComplex *in2, doubleComplex* out);
EXTERN_STATFUNC void		zcolumnstdevfa(doubleComplex *in1, int lines, int columns, doubleComplex *in2, doubleComplex* out);

/*
**  convenience functions
*/

EXTERN_STATFUNC floatComplex	cstdevfcs(floatComplex *in1, int lines, int columns, float *in2);
EXTERN_STATFUNC floatComplex	cstdevfsc(float *in1, int lines, int columns, floatComplex *in2);

EXTERN_STATFUNC doubleComplex	zstdevfzd(doubleComplex *in1, int lines, int columns, double *in2);
EXTERN_STATFUNC doubleComplex	zstdevfdz(double *in1, int lines, int columns, doubleComplex *in2);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__STDEVF_H__ */
