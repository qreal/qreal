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

#define localSign(x) x >= 0 ? 1.0f : -1.0f

floatComplex		cacoshs(floatComplex z) {
  floatComplex acos_z = cacoss(z);
  float sign = localSign(-cimags(acos_z));

  return FloatComplex(-sign * cimags(acos_z), sign * creals(acos_z));
}
