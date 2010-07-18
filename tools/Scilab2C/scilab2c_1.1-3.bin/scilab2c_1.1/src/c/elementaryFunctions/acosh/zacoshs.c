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
//     based on the formula :
//
//     acosh(z) = sign(-imag(acos(z)) i acos(z)
//
//	sign(x) =	1	if x >= 0
//		|	-1	if x < 0
*/

#include "acosh.h"
#include "acos.h"

#define localSign(x) x >= 0 ? 1.0 : -1.0

doubleComplex		zacoshs(doubleComplex z) {
  doubleComplex acos_z = zacoss(z);
  double sign = localSign(-zimags(acos_z));

  return DoubleComplex(-sign * zimags(acos_z), sign * zreals(acos_z));
}
