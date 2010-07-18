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


#include "abs.h"
#include "sqrt.h"
#include "max.h"
#include "min.h"

float cabss(floatComplex in) {
  float real = sabss(creals(in));
  float imag = sabss(cimags(in));
  float zMax = max(real, imag);
  float zMin = min(real, imag);

  if(zMin == 0)
    {
      return zMax;
    }
  return zMax * (ssqrts(1 + (zMin / zMax) * (zMin / zMax)));
}
