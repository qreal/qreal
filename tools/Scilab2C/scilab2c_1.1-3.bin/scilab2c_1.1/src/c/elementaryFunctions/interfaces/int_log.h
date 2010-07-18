/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* THIS IS AN AUTOMATICALLY GENERATED FILE : DO NOT EDIT BY HAND. */

#ifndef __INT_LOG_H__
#define __INT_LOG_H__

#define s0logs0(in)			slogs(in)

#define s0logc0(in)			clogs(FloatComplex(in,0))

#define d0logd0(in)			dlogs(in)

#define d0logz0(in)			zlogs(DoubleComplex(in,0))

#define c0logc0(in)			clogs(in)

#define z0logz0(in)			zlogs(in)

#define s2logs2(in,size,out)		sloga(in, size[0]*size[1], out)

#define s2logc2(in,size,out)		cloga(FloatComplexMatrix(in,0), size[0]*size[1], out)

#define d2logd2(in,size,out)		dloga(in, size[0]*size[1], out)

#define d2logz2(in,size,out)		zloga(DoubleComplexMatrix(in,0), size[0]*size[1], out)

#define c2logc2(in,size,out)		cloga(in, size[0]*size[1], out)

#define z2logz2(in,size,out)		zloga(in, size[0]*size[1], out)

#endif /* !__INT_LOG_H__ */
