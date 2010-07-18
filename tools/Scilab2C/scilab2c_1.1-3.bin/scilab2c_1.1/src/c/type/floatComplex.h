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

#ifndef __FLOATCOMPLEX_H__
#define __FLOATCOMPLEX_H__

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
** Hand made Float Complex definition
** {
*/
struct  float_complex
{
  float real;
  float imag;
};

typedef struct float_complex floatComplex;
/*
** }
*/
#else
/*
** C99 Standard
** {
*/
#include <complex.h>

typedef float complex floatComplex;
/*
** }
*/
#endif

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_TYPE float		creals(floatComplex);
EXTERN_TYPE float		cimags(floatComplex);
EXTERN_TYPE void		creala(floatComplex *in, int size, float *out);
EXTERN_TYPE void		cimaga(floatComplex *in, int size, float *out);
EXTERN_TYPE floatComplex	FloatComplex(float, float);
EXTERN_TYPE floatComplex*	FloatComplexMatrix(float*, float*, int);
EXTERN_TYPE bool		cisreals(floatComplex);
EXTERN_TYPE bool		cisimags(floatComplex);

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__FLOATCOMPLEX_H__ */
