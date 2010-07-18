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

#ifndef __CONJ_H__
#define __CONJ_H__

#include "dynlib_auxiliaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

EXTERN_AUXFUNCT floatComplex cconjs( floatComplex in ) ;

EXTERN_AUXFUNCT void cconja  ( floatComplex* in , int size, floatComplex* out );

EXTERN_AUXFUNCT doubleComplex zconjs ( doubleComplex in) ;

EXTERN_AUXFUNCT void zconja ( doubleComplex* in , int size, doubleComplex* out ) ;

#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__CONJ_H__ */
