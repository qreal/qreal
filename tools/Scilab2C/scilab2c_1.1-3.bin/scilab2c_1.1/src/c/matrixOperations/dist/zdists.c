/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Arnaud TORSET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "dist.h"
#include "pow.h"
#include "sqrt.h"

double zdists(doubleComplex in1, doubleComplex in2){
	double out=0;
	
	out+=dpows(zreals(in1)-zreals(in2),2);
	out+=dpows(zimags(in1)-zimags(in2),2);
	return dsqrts(out);
}


