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

#ifndef __INT_ATANH_H__
#define __INT_ATANH_H__

#define s0atanhs0(in)			satanhs(in)

#define s0atanhc0(in)			catanhs(FloatComplex(in,0))

#define d0atanhd0(in)			datanhs(in)

#define d0atanhz0(in)			zatanhs(DoubleComplex(in,0))

#define c0atanhc0(in)			catanhs(in)

#define z0atanhz0(in)			zatanhs(in)

#define s2atanhs2(in,size,out)		satanha(in, size[0]*size[1], out)

#define s2atanhc2(in,size,out)		catanha(FloatComplexMatrix(in,0), size[0]*size[1], out)

#define d2atanhd2(in,size,out)		datanha(in, size[0]*size[1], out)

#define d2atanhz2(in,size,out)		zatanha(DoubleComplexMatrix(in,0), size[0]*size[1], out)

#define c2atanhc2(in,size,out)		catanha(in, size[0]*size[1], out)

#define z2atanhz2(in,size,out)		zatanha(in, size[0]*size[1], out)

#endif /* !__INT_ATANH_H__ */
