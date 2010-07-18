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

#ifndef __LPC2CEP_H__
#define __LPC2CEP_H__

#include "dynlib_signalprocessing.h"
#include "floatComplex.h"
#include "doubleComplex.h"

/* Matrices on input are squared because matricial logarithm 
   works only with squared matrices (and logm is used in lcp2cep)
   So we just have to pass one parametre on input for the size of
   the input matrix, this parametre is size and it's the numbers of
   rows(or columns)
*/

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_SIGPROC void slpc2cepa(float* in, int size, float* out);

EXTERN_SIGPROC void dlpc2cepa(double* in, int size, double* out);

EXTERN_SIGPROC void clpc2cepa(floatComplex* in, int size, floatComplex* out);

EXTERN_SIGPROC void zlpc2cepa(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /*__LPC2CEP_H__*/

