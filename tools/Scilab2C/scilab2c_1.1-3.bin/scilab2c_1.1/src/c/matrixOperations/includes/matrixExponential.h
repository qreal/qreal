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

#ifndef __MATRIXEXPONENTIAL_H__
#define __MATRIXEXPONENTIAL_H__

#include <math.h>
#include <stdlib.h>
#include "dynlib_matrixoperations.h"
#include "lapack.h"
#include "blas.h"
#include "abs.h"
#include "exp.h"
#include "max.h"
#include "pow.h"
#include "matrixDivision.h"
#include "matrixMultiplication.h"
#include "addition.h"
#include "subtraction.h"
#include "eye.h"
#include "infiniteNorm.h"
#include "frexp.h"
#include "division.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_MATOPS void sexpma (float* in, float* out, int _iLeadDim);

EXTERN_MATOPS void dexpma (double* in, double* out, int _iLeadDim);

EXTERN_MATOPS void cexpma(floatComplex * in, floatComplex * out, int _iLeadDim);

EXTERN_MATOPS void zexpma (doubleComplex * in, doubleComplex * out, int _iLeadDim);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__MATRIXEXPONENTIAL_H__ */

