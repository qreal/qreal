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

floatComplex		catanhs(floatComplex z) {
  floatComplex minus_i_z = FloatComplex(cimags(z), -creals(z));
  floatComplex atan_minus_i_z = catans(minus_i_z);
  return FloatComplex(-cimags(atan_minus_i_z), creals(atan_minus_i_z));
}
