/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Arnaud TORSET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "division.h" 
#include "conj.h" 
#include "multiplication.h"
/* 
 A\B = B*A^-1 = B*conj(A)/|A|^2
 |A|^2 = Re(A)^2 + Im(A)^2
 */
 
floatComplex cldivs (floatComplex in1, floatComplex in2){
	floatComplex fC1, fC2;
	fC1 = cmuls(in2,cconjs(in1));
	fC2 = FloatComplex(creals(in1)*creals(in1)+cimags(in1)*cimags(in1),0);
	return crdivs(fC1,fC2);
}
