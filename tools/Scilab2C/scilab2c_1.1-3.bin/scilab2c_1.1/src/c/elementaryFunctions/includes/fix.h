/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - INRIA - Arnaud Torset
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __FIX_H__
#define __FIX_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*
	fix(x) returns an integer matrix made of nearest rounded integers toward zero,i.e, y=sign(x).*floor(abs(x)). Same as int.
	In these functions, I take the formula : 
		if x>=0 y=floor(x)
		else y=ceil(x)
*/

EXTERN_ELEMFUNCT float	sfixs(float in);

EXTERN_ELEMFUNCT double dfixs(double in);

EXTERN_ELEMFUNCT floatComplex cfixs(floatComplex in);

EXTERN_ELEMFUNCT doubleComplex zfixs(doubleComplex in);

EXTERN_ELEMFUNCT void	sfixa(float* in, int size, float* out);

EXTERN_ELEMFUNCT void	dfixa(double* in, int size, double* out);

EXTERN_ELEMFUNCT void	cfixa(floatComplex* in, int size, floatComplex* out);

EXTERN_ELEMFUNCT void	zfixa(doubleComplex* in, int size, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__FIX_H__ */

