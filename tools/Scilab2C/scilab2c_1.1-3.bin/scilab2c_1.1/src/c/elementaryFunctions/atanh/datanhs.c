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

#include "atanh.h"
#include "atan.h"
/*
//  METHOD
//     based on the formula  atanh(z) = i atan(-i z)
//
// Real case : atanh(x) = -imag(atan(-i x))
*/
double	       datanhs(double x) {
  return (-zimags(zatans(DoubleComplex(0.0, -x))));
}
