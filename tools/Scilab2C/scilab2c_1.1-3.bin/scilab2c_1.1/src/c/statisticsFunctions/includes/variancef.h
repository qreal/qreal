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

#ifndef __VARIANCEF_H__
#define __VARIANCEF_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "dynlib_statisticsfunctions.h"
#include "subtraction.h"
#include "division.h"


#include "pow.h"
#include "sum.h"

#include "meanf.h"


#ifdef  __cplusplus
extern "C" {
#endif





/*
** \brief Variance of a scalar element, just returns it
*/
#define		svariancefs(in1,in2)			    0.0f
#define		srowvariancefs(in1,in2)			0.0f
#define		scolumnvariancefs(in1,in2)		0.0f


/*
** \brief Variance of a scalar element, just returns it
*/
#define		dvariancefs(in1,in2)			    0.0
#define		drowvariancefs(in1,in2)			0.0
#define		dcolumnvariancefs(in1,in2)		0.0


/*
** \brief Variance of a scalar element, just returns it
*/
#define		cvariancefs(in1,in2)			    FloatComplex(0.0f , 0.0f)
#define		crowvariancefs(in1,in2)			FloatComplex(0.0f , 0.0f)
#define		ccolumnvariancefs(in1,in2)		FloatComplex(0.0f , 0.0f)


/*
** \brief Variance of a scalar element, just returns it
*/
#define		zvariancefs(in1,in2)			    DoubleComplex(0.0 , 0.0)
#define		zrowvariancefs(in1,in2)			DoubleComplex(0.0 , 0.0)
#define		zcolumnvariancefs(in1,in2)		DoubleComplex(0.0 , 0.0)


/*
** \brief Variance of a float array
** \param in the float array to process
** \param size, the size of the array
** \returns the variancef.
*/
EXTERN_STATFUNC float		svariancefa(float *in1, int size, float *in2);
EXTERN_STATFUNC void		srowvariancefa(float *in1, int lines, int columns, float *in2, float* out);
EXTERN_STATFUNC void		scolumnvariancefa(float *in1, int lines, int columns, float *in2, float* out);

/*
** \brief Variance of a double array
** \param in the double array to process
** \param size, the size of the array
** \returns the variancef.
*/
EXTERN_STATFUNC double		dvariancefa(double *in1, int size, double *in2);
EXTERN_STATFUNC void		drowvariancefa(double *in1, int lines, int columns, double *in2, double* out);
EXTERN_STATFUNC void		dcolumnvariancefa(double *in1, int lines, int columns, double *in2, double* out);

/*
** \brief Variance of a float complex array
** \param in the float array to process
** \param size, the size of the array
** \returns the variancef.
*/
EXTERN_STATFUNC floatComplex	cvariancefa(floatComplex *in1, int size, floatComplex *in2);
EXTERN_STATFUNC void		crowvariancefa(floatComplex *in1, int lines, int columns, floatComplex *in2, floatComplex* out);
EXTERN_STATFUNC void		ccolumnvariancefa(floatComplex *in1, int lines, int columns, floatComplex *in2, floatComplex* out);

/*
** \brief Variance of a double complex array
** \param in the double array to process
** \param size, the size of the array
** \returns the variancef.
*/
EXTERN_STATFUNC doubleComplex	zvariancefa(doubleComplex *in1, int size, doubleComplex *in2);
EXTERN_STATFUNC void		zrowvariancefa(doubleComplex *in1, int lines, int columns, doubleComplex *in2, doubleComplex* out);
EXTERN_STATFUNC void		zcolumnvariancefa(doubleComplex *in1, int lines, int columns, doubleComplex *in2, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__VARIANCEF_H__ */
