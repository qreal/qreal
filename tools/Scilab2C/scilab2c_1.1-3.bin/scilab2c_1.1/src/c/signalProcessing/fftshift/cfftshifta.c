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

#include <malloc.h>
#include "fftshift.h"

void cfftshifta(floatComplex* in,int rows,int columns,floatComplex* out){
	floatComplex* temp;

	/*FIXME : malloc here*/
	temp= (floatComplex *) malloc((unsigned int)(rows*columns)*sizeof(floatComplex));

	crowfftshifta(in,rows,columns,temp);
	ccolumnfftshifta(temp,rows,columns,out);
}


