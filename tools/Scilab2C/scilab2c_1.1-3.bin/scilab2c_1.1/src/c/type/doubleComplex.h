/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DOUBLECOMPLEX_H__
#define __DOUBLECOMPLEX_H__

#include "dynlib_type.h"

#ifdef __STDC_VERSION__
#  ifndef STDC
#    define STDC
#  endif
#  if __STDC_VERSION__ >= 199901L
#    ifndef STDC99
#      define STDC99
#    endif
#  endif
#endif

#ifndef _MSC_VER
#include <stdbool.h>
#endif

#ifndef STDC99
/*
** Hand made Double Complex definition
** {
*/
struct  double_complex
{
  double real;
  double imag;
};

typedef struct double_complex doubleComplex;
/*
** }
*/
#else
/*
** Standard C99 Complex
** {
*/
#include <complex.h>

typedef double complex doubleComplex;
/*
** }
*/
#endif

#ifdef  __cplusplus
extern "C" {
#endif
EXTERN_TYPE double		zreals(doubleComplex);
EXTERN_TYPE double		zimags(doubleComplex);
EXTERN_TYPE void		zreala(doubleComplex *in, int size, double *out);
EXTERN_TYPE void		zimaga(doubleComplex *in, int size, double *out);
EXTERN_TYPE doubleComplex	DoubleComplex(double, double);
EXTERN_TYPE doubleComplex*	DoubleComplexMatrix(double*, double*, int);
EXTERN_TYPE bool		zisreals(doubleComplex);
EXTERN_TYPE bool		zisimags(doubleComplex);
EXTERN_TYPE doubleComplex	zdevides(doubleComplex, doubleComplex);
#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__DOUBLECOMPLEX_H__ */
