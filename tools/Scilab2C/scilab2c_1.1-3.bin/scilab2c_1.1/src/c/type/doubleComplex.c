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

#include <stdlib.h>
#include "doubleComplex.h"

#ifndef STDC99
/*
** \function zreals
** \brief Return a Complex Real Part .
*/
double zreals(doubleComplex z) {
  return z.real;
}

/*
** \function zimags
** \brief Return a Complex Imaginary Part .
*/
double zimags(doubleComplex z) {
  return z.imag;
}
#else
/*
** \function zreals
** \brief Return a Complex Real Part .
*/
double zreals(doubleComplex z) {
  return creal(z);
}

/*
** \function zimags
** \brief Return a Complex Imaginary Part .
*/
double zimags(doubleComplex z) {
  return cimag(z);
}
#endif

/*
** \function creala
** \brief Return a Complex Real Part array.
*/
void zreala(doubleComplex* z, int size, double* out) {
  int i = 0;

  for (i = 0 ; i < size ; ++i)
    {
      out[i] = zreals(z[i]);
    }
}

/*
** \function cimaga
** \brief Return a Complex Imaginary Part array.
*/
void zimaga(doubleComplex* z, int size, double* out) {
  int i = 0;

  for (i = 0 ; i < size ; ++i)
    {
      out[i] = zimags(z[i]);
    }
}

/*
** \function DoubleComplex
** \brief construct a Double Complex .
*/
doubleComplex DoubleComplex(double real, double imag) {
  doubleComplex z;
#ifndef STDC99
  z.real = real;
  z.imag = imag;
#else
  z = real + I * imag;
#endif
  return z;
}

/*
** \function DoubleComplexMatrix
** \brief construct a Double Complex Matrix.
*/
doubleComplex *DoubleComplexMatrix(double* real, double* imag, int size) {
  doubleComplex *z = (doubleComplex *) malloc((unsigned int) size * sizeof(doubleComplex));
  int i = 0;

  for(i = 0; i < size; ++i)
    {
      z[i] = DoubleComplex(real[i], imag[i]);
    }
  return z;
}

/*
** \function isreal
** \brief check if complex is real .
*/
bool zisreals(doubleComplex z) {
  if (zimags(z) == 0)
    return true;
  return false;
}

/*
** \function isimag
** \brief check if complex is pure imaginary .
*/
bool zisimags(doubleComplex z) {
  if (zreals(z) == 0)
    return true;
  return false;
}



