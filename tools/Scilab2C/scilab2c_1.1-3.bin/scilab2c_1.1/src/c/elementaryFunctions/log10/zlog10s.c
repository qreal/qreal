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

#include "log10.h"
#include "log.h"

doubleComplex	zlog10s(doubleComplex in) {
  doubleComplex log_z = zlogs(in);
  return DoubleComplex(zreals(log_z) / dlogs(10.0), zimags(log_z) / dlogs(10.0));
}
