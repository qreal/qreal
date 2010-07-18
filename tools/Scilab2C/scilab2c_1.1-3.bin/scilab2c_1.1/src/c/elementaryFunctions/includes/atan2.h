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

#ifndef __ATAN2_H__
#define __ATAN2_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"
#include <math.h>

#ifdef  __cplusplus
extern "C" {
#endif

/*
** Compute arctan with two parameters  .
*/


/*
** \brief Float arc tangent function
*/
EXTERN_ELEMFUNCT float satan2s  ( float x , float y );

/*
** \brief Double arc tangent function
*/
EXTERN_ELEMFUNCT double datan2s ( double x , double y);


/*
** \brief array's Float arc tangent function 
*/
EXTERN_ELEMFUNCT void satan2a (float* in1, int size1, float* in2, int size2, float* out) ; 

/*
** \brief array's Double arc tangent function 
*/
EXTERN_ELEMFUNCT void datan2a (double* in1, int size1, double* in2, int size2, double* out) ;

#ifdef  __cplusplus
} /* extern "C" */
#endif



#endif /* !__EXP10_H__ */
