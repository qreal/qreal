/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Arnaud Torset
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* THIS IS AN AUTOMATICALLY GENERATED FILE : DO NOT EDIT BY HAND. */

#ifndef __INT_REAL_H__
#define __INT_REAL_H__

#define s0reals0(in)			in

#define d0reald0(in)			in

#define c0reals0(in)			creals(in)

#define z0reald0(in)			zreals(in)

#define s2reals2(in,size,out)		{int i;\
						for (i=0;i<size[0]*size[1];i++) out[i]=in[i];\
						}

#define d2reald2(in,size,out)		{int i;\
						for (i=0;i<size[0]*size[1];i++) out[i]=in[i];\
						}

#define c2reals2(in,size,out)		creala(in, size[0]*size[1],out)

#define z2reald2(in,size,out)		zreala(in, size[0]*size[1],out)

#endif /* !__INT_REAL_H__ */
