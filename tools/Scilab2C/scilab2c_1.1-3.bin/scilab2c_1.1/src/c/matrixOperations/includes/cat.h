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

#ifndef __CAT_H__
#define __CAT_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"


#ifdef  __cplusplus
extern "C" {
#endif

/*
** \brief concat 2 floats scalars
*/
EXTERN_MATOPS void		srowcats(float in1, float in2, float *out);
EXTERN_MATOPS void		scolumncats(float in1, float in2, float *out);

/*
** \brief concat 2 double scalars
*/
EXTERN_MATOPS void		drowcats(double in1, double in2, double *out);
EXTERN_MATOPS void		dcolumncats(double in1, double in2, double *out);

/*
** \brief concat 2 floats complex scalars
*/
EXTERN_MATOPS void		crowcats(floatComplex in1, floatComplex in2, floatComplex *out);
EXTERN_MATOPS void		ccolumncats(floatComplex in1, floatComplex in2, floatComplex *out);

/*
** \brief concat 2 double complex scalars
*/
EXTERN_MATOPS void		zrowcats(doubleComplex in1, doubleComplex in2, doubleComplex *out);
EXTERN_MATOPS void		zcolumncats(doubleComplex in1, doubleComplex in2, doubleComplex *out);

/*
** \brief Concat float arrays
** \param in1 the float array to process
** \param lines1
** \param columns1
** \param in2 the float array to process to concat
** \param lines2
** \param columns2
** \param out the concatenation
*/
EXTERN_MATOPS void		srowcata(float *in1, int lines1, int columns1, float *in2, int lines2, int columns2, float* out);
EXTERN_MATOPS void		scolumncata(float *in1, int lines1, int columns1, float *in2, int lines2, int columns2, float* out);

/*
** \brief Concat double arrays
** \param in1 the double array to process
** \param lines1
** \param columns1
** \param in2 the double array to process to concat
** \param lines2
** \param columns2
** \param out the concatenation
*/
EXTERN_MATOPS void		drowcata(double *in1, int lines1, int columns1, double *in2, int lines2, int columns2, double* out);
EXTERN_MATOPS void		dcolumncata(double *in1, int lines1, int columns1, double *in2, int lines2, int columns2, double* out);

/*
** \brief Concat Complex float arrays
** \param in1 the Complex float array to process
** \param lines1
** \param columns1
** \param in2 the Complex float array to process to concat
** \param lines2
** \param columns2
** \param out the concatenation
*/
EXTERN_MATOPS void		crowcata(floatComplex *in1, int lines1, int columns1, floatComplex *in2, int lines2, int columns2, floatComplex* out);
EXTERN_MATOPS void		ccolumncata(floatComplex *in1, int lines1, int columns1, floatComplex *in2, int lines2, int columns2, floatComplex* out);

/*
** \brief Concat Complex double arrays
** \param in1 the Complex double array to process
** \param lines1
** \param columns1
** \param in2 the Complex double array to process to concat
** \param lines2
** \param columns2
** \param out the concatenation
*/
EXTERN_MATOPS void		zrowcata(doubleComplex *in1, int lines1, int columns1, doubleComplex *in2, int lines2, int columns2, doubleComplex* out);
EXTERN_MATOPS void		zcolumncata(doubleComplex *in1, int lines1, int columns1, doubleComplex *in2, int lines2, int columns2, doubleComplex* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__CAT_H__ */
