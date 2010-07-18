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

#ifndef __LEV_H__
#define __LEV_H__

#include "dynlib_signalprocessing.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* 
   function Scilab : 
   [out1,y,out3]=lev(in)
   y : result of the function
*/
EXTERN_SIGPROC float sleva (float* in,int size, float* out1, float* out3);
EXTERN_SIGPROC double dleva (double* in, int size, double* out1, double* out3);
EXTERN_SIGPROC floatComplex cleva (floatComplex* in,int size, floatComplex* out1, floatComplex* out3);
EXTERN_SIGPROC doubleComplex zleva (doubleComplex* in,int size, doubleComplex* out1, doubleComplex* out3);


/* 
   function Scilab : 
   [out1,y]=lev(in)
   y : result of the function
*/
EXTERN_SIGPROC float sleva2 (float* in,int size, float* out1);
EXTERN_SIGPROC double dleva2 (double* in, int size, double* out1);
EXTERN_SIGPROC floatComplex cleva2 (floatComplex* in,int size, floatComplex* out1);
EXTERN_SIGPROC doubleComplex zleva2 (doubleComplex* in,int size, doubleComplex* out1);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /*__LEV_H__*/

