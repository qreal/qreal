/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __FIND2D_H__
#define __FIND2D_H__

#include "dynlib_auxiliaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*
** \brief Float Find function
** max is an integer giving the maximum number of indices to return. (use -1 to search them all)
*/
EXTERN_AUXFUNCT void sfind2da(float* x, int rows, int columns, float* out1, int* sizeOut1, float* out2, int* sizeOut2,int max);

/*
** \brief Double Find function
** max is an integer giving the maximum number of indices to return. (use -1 to search them all)
*/
EXTERN_AUXFUNCT void dfind2da(double* x, int rows, int columns, double* out1, int* sizeOut1, double* out2, int* sizeOut2,int max);

/*
** \brief Float Complex Find function
** max is an integer giving the maximum number of indices to return. (use -1 to search them all)
*/
EXTERN_AUXFUNCT void cfind2da(floatComplex* z, int rows, int columns, float* out1, int* sizeOut1, float* out2, int* sizeOut2,int max);

/*
** \brief Double Complex Find function
** max is an integer giving the maximum number of indices to return. (use -1 to search them all)
*/
EXTERN_AUXFUNCT void zfind2da(doubleComplex* z, int rows, int columns, double* out1, int* sizeOut1, double* out2, int* sizeOut2,int max);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__FIND2D_H__ */
