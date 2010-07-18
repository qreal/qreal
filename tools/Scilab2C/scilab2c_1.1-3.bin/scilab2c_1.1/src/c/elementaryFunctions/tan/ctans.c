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

#include <math.h>
#include "lapack.h"
#include "tan.h"
#include "sqrt.h"
#include "cos.h"
#include "sinh.h"
#include "sin.h"
#include "log.h"
#include "abs.h"

#define localSign(x) x >= 0 ? 1.0f : -1.0f

floatComplex		ctans(floatComplex z) {
  float Temp = 0.0f;
  float Lim = 1.0f + slogs(2.0f / ssqrts((float) getRelativeMachinePrecision()));
  float RealIn = creals(z);
  float ImagIn = cimags(z);
  float RealOut = 0.0f;
  float ImagOut = 0.0f;

  Temp = powf(scoss(RealIn), 2.0f) + powf(ssinhs(ImagIn), 2.0f);
  RealOut = 0.5f * ssins(2 * RealIn) / Temp;
  if(sabss(ImagIn) < Lim)
    {
      ImagOut = 0.5f * ssinhs(2.0f * ImagIn) / Temp;
    }
  else
    {
      ImagOut = localSign(ImagIn);
    }

  return FloatComplex(RealOut, ImagOut);
}
