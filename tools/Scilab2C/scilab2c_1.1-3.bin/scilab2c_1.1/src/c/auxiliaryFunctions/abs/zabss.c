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

#include <math.h>

#include "abs.h"
#include "sqrt.h"
#include "max.h"
#include "min.h"

double zabss(doubleComplex in) {
  double real = dabss(zreals(in));
  double imag = dabss(zimags(in));
  double zMax = max(real, imag);
  double zMin = min(real, imag);

  if(zMin == 0)
    {
      return zMax;
    }
  return zMax * (dsqrts(1 + (zMin / zMax) * (zMin / zMax)));
}
