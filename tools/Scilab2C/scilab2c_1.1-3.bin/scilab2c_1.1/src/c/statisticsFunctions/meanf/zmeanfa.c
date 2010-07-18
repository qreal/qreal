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

#include "meanf.h"
#include "multiplication.h"
#include "division.h"

doubleComplex zmeanfa(doubleComplex *in1, int size, doubleComplex *in2 ) {
  doubleComplex accumulate = DoubleComplex(0.0, 0.0);
  doubleComplex freAccumulate = DoubleComplex(0.0, 0.0);
  int i = 0;

  for (i = 0; i < size; ++i)
    {
      accumulate = zadds(accumulate , zmuls( in1[i] ,in2[i]));
      freAccumulate = zadds(in2[i],freAccumulate);
    }
  return zrdivs(accumulate ,freAccumulate);
}

