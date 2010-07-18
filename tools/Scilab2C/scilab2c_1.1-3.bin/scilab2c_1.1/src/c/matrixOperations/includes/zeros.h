/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef __ZEROS_H__
#define __ZEROS_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
** \brief create a float one value
*/
#define szeross(in)		0.0f

/*
** \brief create a Double one value
*/
#define dzeross(in)		0

/*
** \brief create a float complex one value
*/
#define czeross(in)		FloatComplex(0.0f, 0)

/*
** \brief create a Double complex one value
*/
#define zzeross(in)		DoubleComplex(0, 0)



/*
** \brief create a float matrix full of one
*/
EXTERN_MATOPS void szerosa ( float* in , int rows , int cols );
/*
** \brief create a float complex matrix full of one
*/
EXTERN_MATOPS void czerosa ( floatComplex* in , int rows ,int cols );
/*
** \brief create a double  matrix full of one
*/
EXTERN_MATOPS void dzerosa ( double* in , int rows ,int cols );
/*
** \brief create a double complex  matrix full of one
*/
EXTERN_MATOPS void zzerosa ( doubleComplex* in , int rows ,int cols );

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__ZEROS_H__ */
