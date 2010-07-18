/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "infiniteNorm.h"

double dinfnorma(double* in, int _iRows, int _iCols)
{
 	int iIndex1 = 0, iIndex2 = 0;
	double dblTemp = 0;
	double dblRef = 0;


    for(iIndex1 = 0 ; iIndex1 < _iRows ; iIndex1++)
    {
        dblTemp = 0;
        for(iIndex2 = 0 ; iIndex2 < _iCols ; iIndex2++)
        {
            dblTemp += in[iIndex1 + iIndex2 * _iRows];
        }
        if(dblTemp > dblRef)
            dblRef = dblTemp;
    }

	return dblRef;   
       
}
