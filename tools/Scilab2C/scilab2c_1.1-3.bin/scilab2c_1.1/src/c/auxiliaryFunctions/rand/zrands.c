/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "rand.h"

doubleComplex zrands(void) {
  double Real_rand=drands();
  double Imag_rand=drands();
  return DoubleComplex(Real_rand,Imag_rand );
  
  /* We can't do return DoubleComplex(drands(),drands()) 
  cause the result is false; it give imag+real*i instead of real+imag*i */
}
