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

/*
//         cosh(z) = 0.5 (exp(|z|) + exp(-|z|))
//                 = 0.5 ( y + 1/y ) with y = exp(|z|)
*/

#include <math.h>
#include "cosh.h"
#include "exp.h"
#include "abs.h"

float		scoshs(float x) {
  float y = sexps(sabss(x));
  return (0.5f * (y + 1.0f / y));
}
