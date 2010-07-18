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

#ifndef __INT_LOG10_H__
#define __INT_LOG10_H__

#define s0log10s0(in)			slog10s(in)

#define s0log10c0(in)			clog10s(FloatComplex(in,0))

#define d0log10d0(in)			dlog10s(in)

#define d0log10z0(in)			zlog10s(DoubleComplex(in,0))

#define c0log10c0(in)			clog10s(in)

#define z0log10z0(in)			zlog10s(in)

#define s2log10s2(in,size,out)		slog10a(in, size[0]*size[1], out)

#define s2log10c2(in,size,out)		clog10a(FloatComplexMatrix(in,0), size[0]*size[1], out)

#define d2log10d2(in,size,out)		dlog10a(in, size[0]*size[1], out)

#define d2log10z2(in,size,out)		zlog10a(DoubleComplexMatrix(in,0), size[0]*size[1], out)

#define c2log10c2(in,size,out)		clog10a(in, size[0]*size[1], out)

#define z2log10z2(in,size,out)		zlog10a(in, size[0]*size[1], out)

#endif /* !__INT_LOG10_H__ */
