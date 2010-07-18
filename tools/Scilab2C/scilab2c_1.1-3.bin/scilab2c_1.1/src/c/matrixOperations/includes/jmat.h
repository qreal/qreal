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

#ifndef __JMAT_H__
#define __JMAT_H__

#include "dynlib_matrixoperations.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_MATOPS void sjmata(float in1, float in2, float* out);
EXTERN_MATOPS void djmata(double in1, double in2, double* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__JMAT_H__ */
