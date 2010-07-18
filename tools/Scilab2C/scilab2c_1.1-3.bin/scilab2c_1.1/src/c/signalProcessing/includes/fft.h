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

#ifndef __FFT_H__
#define __FFT_H__

#include "dynlib_signalprocessing.h"
#include "floatComplex.h"
#include "doubleComplex.h"


#define sffts(in)					in
#define dffts(in)					in
#define cffts(in)					in
#define zffts(in)					in

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_SIGPROC void sfftma(float* in,int rows,int columns,float* out);		

EXTERN_SIGPROC void dfftma(double* in,int rows,int columns,double* out);

/*
** compute the fast fourier transform of a vector
** param in  : the input matrix in complex float precision
** param rows: number of rows of the input matrix
** param cols: number of cols of the input matrix
** param out : the transformed matrix in complex float precision
*/

EXTERN_SIGPROC void cfftma ( floatComplex* in , int rows, int cols, floatComplex* out);
/*
** compute the fast fourier transform of a vector
** param in  : the input matrix in complex double precision
** param rows: number of rows of the input matrix
** param cols: number of cols of the input matrix
** param out : the transformed matrix in complex double precision
*/
EXTERN_SIGPROC void zfftma ( doubleComplex* in , int rows, int cols, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__FFT_H__ */

