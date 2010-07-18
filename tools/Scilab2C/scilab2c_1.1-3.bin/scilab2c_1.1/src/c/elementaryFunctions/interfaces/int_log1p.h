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

#ifndef __INT_LOG1p1P_H__
#define __INT_LOG1p1P_H__

#define s0log1ps0(in)			slog1ps(in)

#define s0log1pc0(in)			clog1ps(FloatComplex(in,0))

#define d0log1pd0(in)			dlog1ps(in)

#define d0log1pz0(in)			zlog1ps(DoubleComplex(in,0))

#define c0log1pc0(in)			clog1ps(in)

#define z0log1pz0(in)			zlog1ps(in)

#define s2log1ps2(in,size,out)		slog1pa(in, size[0]*size[1], out)

#define s2log1pc2(in,size,out)		clog1pa(FloatComplexMatrix(in,0), size[0]*size[1], out)

#define d2log1pd2(in,size,out)		dlog1pa(in, size[0]*size[1], out)

#define d2log1pz2(in,size,out)		zlog1pa(DoubleComplexMatrix(in,0), size[0]*size[1], out)

#define c2log1pc2(in,size,out)		clog1pa(in, size[0]*size[1], out)

#define z2log1pz2(in,size,out)		zlog1pa(in, size[0]*size[1], out)
#endif /* !__INT_LOG1p1P_H__ */
