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

#ifndef __MATRIXDIVISION_H__
#define __MATRIXDIVISION_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"
#include "lapack.h" 
#include "cat.h"
#include "matrixTranspose.h"
#include "conj.h" 
#include "min.h" 
#include "max.h" 


#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_MATOPS void srdivma (	float* in1, int lines1, int columns1 ,
			    float* in2, int lines2, int columns2 ,
			 	float* out );		

EXTERN_MATOPS void sldivma (	float* in1, int lines1, int columns1 ,
			    float* in2, int lines2, int columns2 ,
			 	float* out );	

				
EXTERN_MATOPS void drdivma (	double* in1, int lines1, int columns1 ,
			    double* in2, int lines2, int columns2 ,
			 	double* out );		


EXTERN_MATOPS void dldivma (	double* in1, int lines1, int columns1 ,
				double* in2, int lines2, int columns2 ,
				double* out );

EXTERN_MATOPS void zrdivma(	doubleComplex* in1, int lines1, int columns1 ,
				doubleComplex* in2, int lines2, int columns2 ,
				doubleComplex* out );

EXTERN_MATOPS void zldivma(    doubleComplex* in1, int lines1, int columns1 ,
				doubleComplex* in2, int lines2, int columns2 ,
				doubleComplex* out );

EXTERN_MATOPS void crdivma(    floatComplex* in1, int lines1, int columns1 ,
				floatComplex* in2, int lines2, int columns2 ,
				floatComplex* out );

EXTERN_MATOPS void cldivma(	floatComplex* in1, int lines1, int columns1 ,
				floatComplex* in2, int lines2, int columns2 ,
				floatComplex* out );

/*special case  row vector /row vector => scalar */
/*
** \brief Compute a division for double.
** \param in1 : input array.
** \param in2 : input array.
** \param size : size of in2 array.
** \return : scalar that contains the right division of the two vectors = in1 .* in2.
*/
EXTERN_MATOPS double drdivv(double *in1, double *in2, int size2);

EXTERN_MATOPS doubleComplex zrdivv(doubleComplex *in1, doubleComplex *in2, int size2);

EXTERN_MATOPS doubleComplex zrdivzdv(doubleComplex *in1, double *in2, int size2);

EXTERN_MATOPS doubleComplex zrdivdzv(double *in1, doubleComplex *in2, int size2);

/*
** \brief Compute a division for float.
** \param in1 : input array.
** \param in2 : input array.
** \param size : size of in2 array.
** \return : scalar that contains the division of the two vectors = in1 .* in2.
*/
EXTERN_MATOPS float srdivv(float *in1, float *in2, int size2);

EXTERN_MATOPS floatComplex crdivv(floatComplex *in1, floatComplex *in2, int size2);

EXTERN_MATOPS floatComplex crdivcsv(floatComplex *in1, float *in2, int size2);

EXTERN_MATOPS floatComplex crdivscv(float *in1, floatComplex *in2, int size2);



#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__MATRIXDIVISION_H__ */
