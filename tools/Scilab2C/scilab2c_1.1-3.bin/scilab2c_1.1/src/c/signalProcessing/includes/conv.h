/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Arnaud TORSET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CONV_H__
#define __CONV_H__

#include <math.h>
#include <malloc.h>
#include "dynlib_signalprocessing.h"
#include "floatComplex.h"
#include "doubleComplex.h"
#include "multiplication.h"
#include "ifft.h"
#include "fft.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*  Computes the convolution between VECTORS */

EXTERN_SIGPROC void sconva(float *in1, int size1, float *in2,int size2, float *out);

EXTERN_SIGPROC void dconva(double *in1, int size1, double *in2,int size2, double *out);

EXTERN_SIGPROC void cconva(floatComplex *in1, int size1, floatComplex *in2,int size2, floatComplex *out);

EXTERN_SIGPROC void zconva(doubleComplex *in1, int size1, doubleComplex *in2,int size2, doubleComplex *out);
#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__CONV_H__ */
