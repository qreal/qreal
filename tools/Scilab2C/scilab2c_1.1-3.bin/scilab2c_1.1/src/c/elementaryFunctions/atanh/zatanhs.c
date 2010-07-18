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

/*
//  METHOD
//     based on the formula  atanh(z) = i atan(-i z)
*/

#include "atanh.h"
#include "atan.h"

doubleComplex		zatanhs(doubleComplex z) {
  doubleComplex minus_i_z = DoubleComplex(zimags(z), -zreals(z));
  doubleComplex atan_minus_i_z = zatans(minus_i_z);
  return DoubleComplex(-zimags(atan_minus_i_z), zreals(atan_minus_i_z));
}
