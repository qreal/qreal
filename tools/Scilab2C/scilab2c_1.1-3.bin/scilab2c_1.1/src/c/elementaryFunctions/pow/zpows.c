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

#include "pow.h"
#include "log.h"
#include "exp.h"

doubleComplex		zpows(doubleComplex z, doubleComplex power) {
  /*Cas z=0 */
  if ( (zreals(z)==0) && (zimags(z)==0) ){
  	/* Cas 0^0 */
  	if ( (zreals(power)==0) && (zimags(power)==0) ) return DoubleComplex(1,0);
  	/* Cas 0^x, x!=0 */
  	return DoubleComplex(0,0);
  }
  
  /* Cas z!=0 */
  return zexps(zmuls(zlogs(z), power));
}
