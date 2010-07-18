/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "string.h" 

void cstrings (floatComplex in, char* out) {
	
	float RealIn = creals(in);
    float ImgIn = cimags(in);
	
	sprintf ( out, "%5.20e + %5.20ei" ,RealIn ,ImgIn );	

}
