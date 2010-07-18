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

#ifndef __HILBERT_H__
#define __HILBERT_H__

#include "dynlib_matrixoperations.h"
#include "floatComplex.h"
#include "doubleComplex.h"
#include <math.h>

#ifdef  __cplusplus
extern "C" {
#endif
/*
** generate an Hilbert's matrix 
** param out : the hilbert's matrix in float precision
** param size: matrix's size 
*/

EXTERN_MATOPS void shilba ( float*  out, int size) ;

/*
** generate an Hilbert's matrix 
** param out : the hilbert's matrix in double precision
** param size: matrix's size 
*/


EXTERN_MATOPS void dhilba ( double* out, int size ) ;

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* !__HILBERT_H__ */

