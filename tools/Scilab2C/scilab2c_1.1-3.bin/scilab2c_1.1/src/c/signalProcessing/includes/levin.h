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

#ifndef __LEVIN_H__
#define __LEVIN_H__

#include "dynlib_signalprocessing.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_SIGPROC void dlevina (int n, double* cov, int lCov, int cCov, double* la, double* sig, double* lb);

EXTERN_SIGPROC void slevina (int n, float* cov, int lCov, int cCov, float* la, float* sig, float* lb);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /*__LEVIN_H__*/

