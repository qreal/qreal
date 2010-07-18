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
 
 
#ifndef __SPEC_H__
#define __SPEC_H__

#include "dynlib_matrixoperations.h"
#include "doubleComplex.h"
#include "floatComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* spec gives only the eigenvalues
   If you want the eigenvalues and the eigenvectors, use spec2 */

/* spec */
#define sspecs(in) 	in
#define dspecs(in) 	in
#define cspecs(in) 	in
#define zspecs(in) 	in

EXTERN_MATOPS void sspeca(float* in, int rows, float* out);
EXTERN_MATOPS void dspeca(double* in, int rows, double* out);
EXTERN_MATOPS void cspeca(floatComplex* in, int rows, floatComplex* out);
EXTERN_MATOPS void zspeca(doubleComplex* in, int rows,doubleComplex* out);



/* spec2 */
#define sspec2s(in,out)  	sspecs(1);*out=in; 
#define dspec2s(in,out)  	dspecs(1);*out=in; 
#define cspec2s(in,out)   	cspecs(FloatComplex(1,0));*out=FloatComplex(creals(in),cimags(in)); 
#define zspec2s(in,out)  	zspecs(DoubleComplex(1,0));*out=DoubleComplex(zreals(in),zimags(in)); 

EXTERN_MATOPS void sspec2a(float* in, int rows, float* eigenvalues,float* eigenvectors);
EXTERN_MATOPS void dspec2a(double* in, int rows, double* eigenvalues,double* eigenvectors);
EXTERN_MATOPS void cspec2a(floatComplex* in, int rows, floatComplex* eigenvalues,floatComplex* eigenvectors);
EXTERN_MATOPS void zspec2a(doubleComplex* in, int rows,doubleComplex* eigenvalues,doubleComplex* eigenvectors);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* __SPEC_H__ */
