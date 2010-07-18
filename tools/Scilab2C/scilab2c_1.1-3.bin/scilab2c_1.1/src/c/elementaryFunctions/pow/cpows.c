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

#include "pow.h"
#include "exp.h"
#include "log.h"

floatComplex		cpows(floatComplex z, floatComplex power) {
      /*Cas z=0 */
  if ( (creals(z)==0) && (cimags(z)==0) ){
  	/* Cas 0^0 */
  	if ( (creals(power)==0) && (cimags(power)==0) ) return FloatComplex(1,0);
  	/* Cas 0^x, x!=0 */
  	return FloatComplex(0,0);
  }
  
  /* Cas z!=0 */
  return cexps(cmuls(clogs(z), power));
}
