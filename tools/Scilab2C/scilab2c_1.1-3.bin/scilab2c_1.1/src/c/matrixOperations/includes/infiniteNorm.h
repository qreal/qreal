/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __INFINITENORM_H__
#define __INFINITENORM_H__

#include "dynlib_matrixoperations.h"
#include "sign.h" 
#include "pythag.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
** \brief Compute the infinite norm of a given floats matrix.
** \param in : input matrix.
** \param _iRows : number of rows of the matrix .
** \param _iCols : number of columns of the matrix .
*/

EXTERN_MATOPS float  sinfnorma(float* in, int _iRows, int _iCols);

/*
** \brief Compute the infinite norm of a given doubles matrix.
** \param in : input matrix.
** \param leadDimIn : the leading dimension of the matrix .
** \param out : the matrix inverse of the input .
*/

EXTERN_MATOPS double dinfnorma(double* in, int _iRows, int _iCols);

/*
** \brief Compute the infinite norm of a given complex floats  matrix.
** \param in : input matrix.
** \param _iRows : number of rows of the matrix .
** \param _iCols : number of columns of the matrix .
*/

EXTERN_MATOPS float  cinfnorma(floatComplex* in, int _iRows, int _iCols);

/*
** \brief Compute the infinite norm of a given complex doubles matrix.
** \param in : input matrix.
** \param _iRows : number of rows of the matrix .
** \param _iCols : number of columns of the matrix .
*/

EXTERN_MATOPS double zinfnorma(doubleComplex* in, int _iRows, int _iCols);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__INFINITENORM_H__ */

