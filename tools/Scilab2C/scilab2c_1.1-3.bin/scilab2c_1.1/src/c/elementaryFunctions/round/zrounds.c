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
 

#include "round.h"

doubleComplex zrounds(doubleComplex x) {
	int roundReal;
	int roundImag;
	
	if (zreals(x)>=0)	roundReal = (int)(zreals(x)+0.5);
	else 			roundReal = (int)(zreals(x)-0.5);
	
	
	
	if (zimags(x)>=0)	roundImag = (int)(zimags(x)+0.5);
	else 			roundImag = (int)(zimags(x)-0.5);
	
	return DoubleComplex((double)roundReal, (double)roundImag);
}
