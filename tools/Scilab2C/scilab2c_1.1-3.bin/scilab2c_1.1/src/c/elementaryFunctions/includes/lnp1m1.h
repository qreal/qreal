/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LNP1M1_H__
#define __LNP1M1_H__

#include "dynlib_elementaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif
/*
** Compute v = log ( (1 + s)/(1 - s) ) for different types .
*/

/*
** \brief Float Lnp1m1 function
** \param in : input value.
*/
EXTERN_ELEMFUNCT float		slnp1m1s(float in);

/*
** \brief Double Lnp1m1 function
** \param in : input value.
*/
EXTERN_ELEMFUNCT double		dlnp1m1s(double in);


#ifdef  __cplusplus
} /* extern "C" */
#endif


#endif /* !__LNP1M1_H__ */

