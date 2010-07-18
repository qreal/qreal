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

#ifndef __FFTSHIFT_H__
#define __FFTSHIFT_H__


#include "dynlib_signalprocessing.h"

/* 
   fftshift rearrange the result of fft(x) 
   it's call like that : fftshift(y), y=ff(x)
*/
#include "floatComplex.h"
#include "doubleComplex.h"


#define sfftshifts(in)					in
#define dfftshifts(in)					in
#define cfftshifts(in)					in
#define zfftshifts(in)					in

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_SIGPROC void sfftshifta(float* in,int rows,int columns,float* out);		

EXTERN_SIGPROC void dfftshifta(double* in,int rows,int columns,double* out);

EXTERN_SIGPROC void cfftshifta ( floatComplex* in , int rows, int cols, floatComplex* out);

EXTERN_SIGPROC void zfftshifta ( doubleComplex* in , int rows, int cols, doubleComplex* out);


#define srowfftshifts(in)					in
#define drowfftshifts(in)					in
#define crowfftshifts(in)					in
#define zrowfftshifts(in)					in

EXTERN_SIGPROC void srowfftshifta(float* in,int rows,int columns,float* out);		

EXTERN_SIGPROC void drowfftshifta(double* in,int rows,int columns,double* out);

EXTERN_SIGPROC void crowfftshifta ( floatComplex* in , int rows, int cols, floatComplex* out);

EXTERN_SIGPROC void zrowfftshifta ( doubleComplex* in , int rows, int cols, doubleComplex* out);

#define scolumnfftshifts(in)					in
#define dcolumnfftshifts(in)					in
#define ccolumnfftshifts(in)					in
#define zcolumnfftshifts(in)					in

EXTERN_SIGPROC void scolumnfftshifta(float* in,int rows,int columns,float* out);		

EXTERN_SIGPROC void dcolumnfftshifta(double* in,int rows,int columns,double* out);

EXTERN_SIGPROC void ccolumnfftshifta ( floatComplex* in , int rows, int cols, floatComplex* out);

EXTERN_SIGPROC void zcolumnfftshifta ( doubleComplex* in , int rows, int cols, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__FFTSHIFT_H__ */

