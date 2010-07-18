/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Arnaud Torset
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
#include "fix.h"

floatComplex cfixs(floatComplex x) {
	float fixReal = 0, fixImag = 0;
	
	if (creals(x)>=0)	fixReal = floorf(creals(x));
	else fixReal = ceilf(creals(x));
	
	if (cimags(x)>=0)	fixImag = floorf(cimags(x));
	else fixImag = ceilf(cimags(x));
	
	return FloatComplex(fixReal, fixImag);
}
