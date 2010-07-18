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

#include "hilbert.h"

void dhilba ( double* out, int size ){
    
  	int iIndex1		= 0;
	int iIndex2		= 0;
	double dblVal	= size;
	double dblTemp	= 0;

	for(iIndex1 = 0 ; iIndex1 < size ; iIndex1++)
	{
		if(iIndex1 != 0)
			dblVal = ((size - iIndex1) * dblVal * (size + iIndex1)) / pow((double)iIndex1,2);
        
		dblTemp = dblVal * dblVal;

		out[iIndex1 * size + iIndex1]	= dblTemp / ( 2 * iIndex1 + 1);
        
		if(iIndex1 == size - 1)
			break;
        
		for(iIndex2 = iIndex1 + 1 ; iIndex2 < size ; iIndex2++)
		{
			dblTemp = -((size - iIndex2) * dblTemp * (size + iIndex2)) / pow((double)iIndex2,2);
			out[iIndex1 * size + iIndex2] = dblTemp / (iIndex1 + iIndex2 + 1);
			out[iIndex2 * size + iIndex1] = out[iIndex1 * size + iIndex2];
		}
	}  
    
    
}
