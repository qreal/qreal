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

#include "sign.h"
#include "abs.h"

floatComplex csigns(floatComplex in) {
  if ( (creals(in)==0) && (cimags(in)==0) ) return FloatComplex(0,0);
  return FloatComplex(creals(in) / cabss(in), cimags(in) / cabss(in));
}
