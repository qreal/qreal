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

#ifndef __IS_NAN_H__
#define __IS_NAN_H__

#include <math.h>
#include "dynlib_auxiliaryfunctions.h"
#include "floatComplex.h"
#include "doubleComplex.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*
** \brief Float Is Nan function
*/
EXTERN_AUXFUNCT float	sisnans(float x);

/*
** \brief Double Is Nan function
*/
EXTERN_AUXFUNCT double disnans(double x);

/*
** \brief Float Complex Is Nan function
*/
EXTERN_AUXFUNCT float	cisnans(floatComplex z);

/*
** \brief Double Complex Is Nan function
*/
EXTERN_AUXFUNCT double zisnans(doubleComplex z);

/*
** \brief Float Is Nan function
*/
EXTERN_AUXFUNCT void	sisnana(float* x, int size, float* out);

/*
** \brief Double Is Nan function
*/
EXTERN_AUXFUNCT void disnana(double* x, int size, double* out);

/*
** \brief Float Complex Is Nan function
*/
EXTERN_AUXFUNCT void	cisnana(floatComplex* z, int size, float* out);

/*
** \brief Double Complex Is Nan function
*/
EXTERN_AUXFUNCT void zisnana(doubleComplex* z, int size, double* out);

#ifdef  __cplusplus
} /* extern "C" */
#endif



#endif /* !__IS_NAN_H__ */
