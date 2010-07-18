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

#ifndef __INT_ACOS_H__
#define __INT_ACOS_H__

#define s0acoss0(in)			sacoss(in)

#define s0acosc0(in)			cacoss(FloatComplex(in,0))

#define d0acosd0(in)			dacoss(in)

#define d0acosz0(in)			zacoss(DoubleComplex(in,0))

#define c0acosc0(in)			cacoss(in)

#define z0acosz0(in)			zacoss(in)

#define s2acoss2(in,size,out)		sacosa(in, size[0]*size[1], out)

#define s2acosc2(in,size,out)		cacosa(FloatComplexMatrix(in,0), size[0]*size[1], out)

#define d2acosd2(in,size,out)		dacosa(in, size[0]*size[1], out)

#define d2acosz2(in,size,out)		zacosa(DoubleComplexMatrix(in,0), size[0]*size[1], out)

#define c2acosc2(in,size,out)		cacosa(in, size[0]*size[1], out)

#define z2acosz2(in,size,out)		zacosa(in, size[0]*size[1], out)

#endif /* !__INT_ACOS_H__ */
