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

#ifndef __FREXP_H__
#define __FREXP_H__

#include <math.h>
#include "dynlib_auxiliaryfunctions.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_AUXFUNCT float  sfrexps(float _fltVal, float *_pfltExp);

EXTERN_AUXFUNCT double dfrexps(double _dblVal, double *_pdblExp);

#ifdef  __cplusplus
} /* extern "C" */
#endif



#endif /* !__FREXP_H__ */
