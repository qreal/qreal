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
 
 
#ifndef __CHOL_H__
#define __CHOL_H__
 
#include "dynlib_matrixoperations.h"
#include "doubleComplex.h"
#include "floatComplex.h"
 
#ifdef  __cplusplus
extern "C" {
#endif
	
EXTERN_MATOPS double dchols(double in);
EXTERN_MATOPS void dchola(double *in, int size, double *out);
 
EXTERN_MATOPS float schols(float in);
EXTERN_MATOPS void schola(float *in, int size, float *out);
 
#define zchols(in) 		DoubleComplex(dchols(zreals(in)),0)

EXTERN_MATOPS void zchola (doubleComplex *in, int size, doubleComplex *out); 
 
#define cchols(in) 		FloatComplex(schols(creals(in)),0)
EXTERN_MATOPS void cchola(floatComplex *in, int size, floatComplex *out); 

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* __CHOL_H__ */
 
