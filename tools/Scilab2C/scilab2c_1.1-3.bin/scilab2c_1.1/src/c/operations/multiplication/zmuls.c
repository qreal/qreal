/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Arnaud TORSET
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

#include "doubleComplex.h"
#include "multiplication.h"


/*
** \function zmuls
** \brief Multiply 2 Complex numbers.
*/
doubleComplex zmuls(doubleComplex z1, doubleComplex z2) {
#ifndef STDC99
  return DoubleComplex(z1.real*z2.real - z1.imag*z2.imag,
		       z1.real*z2.imag + z2.real*z1.imag);
#else
  return z1 * z2;
#endif
}
