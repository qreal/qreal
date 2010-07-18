
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

#ifndef __CROSSCORR_H__
#define __CROSSCORR_H__

#include "dynlib_signalprocessing.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_SIGPROC void scrossCorra(float* in1, int rows1, int cols1, float* in2, int rows2, int cols2, float* out);

EXTERN_SIGPROC void dcrossCorra(double* in1, int rows1, int cols1, double* in2, int rows2, int cols2, double* out);

EXTERN_SIGPROC void ccrossCorra(floatComplex* in1, int rows1, int cols1, floatComplex* in2, int rows2, int cols2, floatComplex* out);

EXTERN_SIGPROC void zcrossCorra(doubleComplex* in1, int rows1, int cols1, doubleComplex* in2, int rows2, int cols2, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* __CROSSCORR_H__ */
