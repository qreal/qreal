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

floatComplex cmeanfa(floatComplex *in1, int size, floatComplex *in2 ) {
  floatComplex accumulate = FloatComplex(0.0f, 0.0f);
  floatComplex freAccumulate =  FloatComplex(0.0f, 0.0f);
  int i = 0;

  for (i = 0; i < size; ++i)
    {
      accumulate = cadds(accumulate, cmuls (in1[i] , in2[i]));
      freAccumulate =  cadds(in2[i],freAccumulate);
    }
  return crdivs(accumulate ,freAccumulate) ;
}


