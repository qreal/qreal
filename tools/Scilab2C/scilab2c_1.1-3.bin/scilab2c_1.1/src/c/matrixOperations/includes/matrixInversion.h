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


#ifndef __MATRIXINVERSION_H__
#define __MATRIXINVERSION_H__

#include "abs.h"
#include "dynlib_matrixoperations.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*
** \brief Compute the matrix inverse for floats.
** \param in : input matrix.
** \param leadDimIn : the leading dimension of the matrix .
** \param out : the matrix inverse of the input .
*/

EXTERN_MATOPS void sinverma ( float* in,  float* out, int leadDimIn );

/*
** \brief Compute the matrix inverse for doubles.
** \param in : input matrix.
** \param leadDimIn : the leading dimension of the matrix .
** \param out : the matrix inverse of the input .
*/


EXTERN_MATOPS void dinverma ( double* in, double* out, int leadDimIn );

/*
** \brief Compute the matrix inverse for complex floats .
** \param in : input matrix.
** \param leadDimIn : the leading dimension of the matrix .
** \param out : the matrix inverse of the input .
*/


EXTERN_MATOPS void cinverma ( floatComplex* in, floatComplex* out, int leadDimIn );

/*
** \brief Compute the matrix inverse for complex doubles.
** \param in : input matrix.
** \param leadDimIn : the leading dimension of the matrix .
** \param out : the matrix inverse of the input .
*/


EXTERN_MATOPS void zinverma ( doubleComplex* in, doubleComplex* out, int leadDimIn );

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__MATRIXINVERSION_H__ */
