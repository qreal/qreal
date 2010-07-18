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
 
 
#ifndef __DETERM_H__
#define __DETERM_H__

#include "dynlib_matrixoperations.h"
#include "doubleComplex.h"
#include "floatComplex.h"

 
#ifdef  __cplusplus
extern "C" {
#endif
	
EXTERN_MATOPS double ddeterma (double *in, int rows);
 
EXTERN_MATOPS float sdeterma (float *in, int rows);
 
EXTERN_MATOPS doubleComplex zdeterma (doubleComplex *in, int rows); 
 
EXTERN_MATOPS floatComplex cdeterma (floatComplex *in, int rows); 

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* __DETERM_H__ */
 
