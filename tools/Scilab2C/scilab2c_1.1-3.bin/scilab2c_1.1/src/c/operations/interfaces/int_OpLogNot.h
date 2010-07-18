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

#ifndef __INT_OPLOGNOT_H__
#define __INT_OPLOGNOT_H__

#define s0OpLogNots0(in)			(in==0) ? (float)1 : (float)0

#define d0OpLogNotd0(in)			(in==0) ? (double)1 : (double)0

#define c0OpLogNotc0(in)			(creals(in)==0) ? FloatComplex(1,0) : FloatComplex(0,0)

#define z0OpLogNotz0(in)			(zreals(in)==0) ? DoubleComplex(1,0) : DoubleComplex(0,0)

#define s2OpLogNots2(in,size,out)		{int i;\
							for (i=0;i<size[0]*size[1];i++)	out[i]=s0OpLogNots0(in[i]);\
							}

#define d2OpLogNotd2(in,size,out)		{int i;\
							for (i=0;i<size[0]*size[1];i++)	out[i]=d0OpLogNotd0(in[i]);\
							}

#define c2OpLogNotc2(in,size,out)		{int i;\
							for (i=0;i<size[0]*size[1];i++)	out[i]=c0OpLogNotc0(in[i]);\
							}

#define z2OpLogNotz2(in,size,out)		{int i;\
							for (i=0;i<size[0]*size[1];i++)	out[i]=z0OpLogNotz0(in[i]);\
							}

#endif /* !__INT_OPLOGNOT_H__ */
