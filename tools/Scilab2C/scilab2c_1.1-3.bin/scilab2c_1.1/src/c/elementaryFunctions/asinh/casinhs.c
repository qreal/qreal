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

/*
//  METHOD
//     based on the formula  asinh(z) = -i asin(i z)
*/

#include "asinh.h"
#include "asin.h"

floatComplex		casinhs(floatComplex z) {
  floatComplex i_z = FloatComplex(-cimags(z), creals(z));
  floatComplex asin_i_z = casins(i_z);

  return FloatComplex(cimags(asin_i_z), -creals(asin_i_z));
}
