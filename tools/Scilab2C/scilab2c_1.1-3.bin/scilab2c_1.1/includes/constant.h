/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#include <math.h>

#define FPI 3.1415926535897931159980f
#define DPI 3.1415926535897931159980

#define NEPER 2.7182818284590450907956

#define PI DPI
#define SCI2C_PI DPI
#define SCI2C_E NEPER
#define SCI2C_T 1
#define SCI2C_F 0
#define SCI2C_NAN nan("")
#define SCI2C_INF 1e100000
#define SCI2C_IMG_C FloatComplex(0,1)
#define SCI2C_IMG_Z DoubleComplex(0,1)

#endif /* !__CONSTANT_H__ */

