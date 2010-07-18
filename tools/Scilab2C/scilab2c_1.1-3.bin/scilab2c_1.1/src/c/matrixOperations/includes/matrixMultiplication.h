/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MATRIXMULTIPLICATION_H__
#define __MATRIXMULTIPLICATION_H__

#include "dynlib_matrixoperations.h"
#include "multiplication.h"
#include "addition.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
**
** WARNING WE ASSUME MATRIXES TO BE CONSCISTENT
** columns1 = lines2;
**
*/

/*
** \brief Compute a multiplication for floats matrixes.
** \param in1 : input matrix.
** \param lines1 : lines of in1 matrix.
** \param columns1 : columns of in1 matrix.
** \param in2 : input arry.
** \param lines2 : lines of in2 matrix.
** \param columns2 : columns of in2 matrix.
** \param out : Matrix that contains the multiplication in1 * in2.
*/
EXTERN_MATOPS void	smulma(float *in1, int lines1, int columns1,
	       float *in2, int lines2, int columns2,
	       float *out);

/*
** \brief Compute a multiplication for doubles matrixes.
** \param in1 : input matrix.
** \param lines1 : lines of in1 matrix.
** \param columns1 : columns of in1 matrix.
** \param in2 : input arry.
** \param lines2 : lines of in2 matrix.
** \param columns2 : columns of in2 matrix.
** \param out : Matrix that contains the multiplication in1 * in2.
*/
EXTERN_MATOPS void	dmulma(double *in1, int lines1, int columns1,
	       double *in2, int lines2, int columns2,
	       double *out);

/*
** \brief Compute a multiplication for floats complex matrixes.
** \param in1 : input matrix.
** \param lines1 : lines of in1 matrix.
** \param columns1 : columns of in1 matrix.
** \param in2 : input arry.
** \param lines2 : lines of in2 matrix.
** \param columns2 : columns of in2 matrix.
** \param out : Matrix that contains the multiplication in1 * in2.
*/
EXTERN_MATOPS void	cmulma(floatComplex *in1, int lines1, int columns1,
	       floatComplex *in2, int lines2, int columns2,
	       floatComplex *out);

/*
** \brief Compute a multiplication for doubles matrixes.
** \param in1 : input matrix.
** \param lines1 : lines of in1 matrix.
** \param columns1 : columns of in1 matrix.
** \param in2 : input arry.
** \param lines2 : lines of in2 matrix.
** \param columns2 : columns of in2 matrix.
** \param out : Matrix that contains the multiplication in1 * in2.
*/
EXTERN_MATOPS void	zmulma(doubleComplex *in1, int lines1, int columns1,
	       doubleComplex *in2, int lines2, int columns2,
	       doubleComplex *out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__MATRIXMULTIPLICATION_H__ */
