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

#include "frexp.h"

float sfrexps(float _fltVal, float *_pfltExp)
{
	int iExp = 0;
	float fltCoef = 0;
	fltCoef = (float) frexp (_fltVal, &iExp);
	*_pfltExp = (float) iExp;
	return fltCoef;
}
