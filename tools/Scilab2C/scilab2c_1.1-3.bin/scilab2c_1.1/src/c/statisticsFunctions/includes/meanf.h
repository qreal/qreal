/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2009 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MEANF_H__
#define __MEANF_H__

#include "dynlib_statisticsfunctions.h"
#include "division.h"
#include "addition.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*
** \brief Meanf of a scalar element, just returns it
*/
#define		smeanfs(in1,in2)			in1
#define		srowmeanfs(in1,in2)			in1
#define		scolumnmeanfs(in1,in2)			in1
#define		smatlabmeanfs(in1,in2)			in1

/*
** \brief Meanf of a scalar element, just returns it
*/
#define		dmeanfs(in1,in2)			in1
#define		drowmeanfs(in1,in2)			in1
#define		dcolumnmeanfs(in1,in2)			in1
#define		dmatlabmeanfs(in1,in2)			in1

/*
** \brief Meanf of a scalar element, just returns it
*/
#define		cmeanfs(in1,in2)			in1
#define		crowmeanfs(in1,in2)			in1
#define		ccolumnmeanfs(in1,in2)			in1
#define		cmatlabmeanfs(in1,in2)			in1

/*
** \brief Meanf of a scalar element, just returns it
*/
#define		zmeanfs(in1,in2)			in1
#define		zrowmeanfs(in1,in2)			in1
#define		zcolumnmeanfs(in1,in2)			in1
#define		zmatlabmeanfs(in1,in2)			in1

/*
** \brief Meanf of a float array
** \param in the float array to process
** \param size, the size of the array
** \returns the meanf.
*/
EXTERN_STATFUNC float		smeanfa(float *in1, int size, float *in2);
EXTERN_STATFUNC void		srowmeanfa(float *in1, int lines, int columns, float *in2, float* out);
EXTERN_STATFUNC void		scolumnmeanfa(float *in1, int lines, int columns, float *in2, float* out);

/*
** \brief Meanf of a double array
** \param in the double array to process
** \param size, the size of the array
** \returns the meanf.
*/
EXTERN_STATFUNC double		dmeanfa(double *in1, int size, double *in2);
EXTERN_STATFUNC void		drowmeanfa(double *in1, int lines, int columns, double *in2, double* out);
EXTERN_STATFUNC void		dcolumnmeanfa(double *in1, int lines, int columns, double *in2, double* out);

/*
** \brief Meanf of a float complex array
** \param in the float array to process
** \param size, the size of the array
** \returns the meanf.
*/
EXTERN_STATFUNC floatComplex	cmeanfa(floatComplex *in1, int size, floatComplex *in2);
EXTERN_STATFUNC void		crowmeanfa(floatComplex *in1, int lines, int columns, floatComplex *in2, floatComplex* out);
EXTERN_STATFUNC void		ccolumnmeanfa(floatComplex *in1, int lines, int columns, floatComplex *in2, floatComplex* out);

/*
** \brief Meanf of a double complex array
** \param in the double array to process
** \param size, the size of the array
** \returns the meanf.
*/
EXTERN_STATFUNC doubleComplex	zmeanfa(doubleComplex *in1, int size, doubleComplex *in2);
EXTERN_STATFUNC void		zrowmeanfa(doubleComplex *in1, int lines, int columns, doubleComplex *in2, doubleComplex* out);
EXTERN_STATFUNC void		zcolumnmeanfa(doubleComplex *in1, int lines, int columns, doubleComplex *in2, doubleComplex* out);

/* for convenience with interface */
doubleComplex zmeanfzd (doubleComplex* in1 ,int lines , int columns , double* in2);
doubleComplex zmeanfdz (double* in1 ,int lines , int columns , doubleComplex* in2);

floatComplex cmeanfcs (floatComplex* in1 ,int lines , int columns , float* in2);
floatComplex cmeanfsc (float* in1 ,int lines , int columns , floatComplex* in2);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__MEAN_H__ */
