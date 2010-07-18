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

#ifndef __INT_MAX_H__
#define __INT_MAX_H__

#define s0maxs0(in)	in

#define d0maxd0(in)	in

#define s2maxs0(in,size)	smaxa(in,size[0]*size[1]);

#define d2maxd0(in,size)	dmaxa(in,size[0]*size[1]);



#define s0s0maxs0(in1,in2)			max(in1,in2)

#define d0d0maxd0(in1,in2)			max(in1,in2)

#define s2s2maxs2(in1,size1,in2,size2,out)		maxa(in1, size1, in2, size2, out)

#define d2d2maxd2(in1,size1,in2,size2,out)		maxa(in1, size1, in2, size2, out)


#define s2s0maxs2(in1,size,in2,out)		{int i;\
							for (i=0;i<size[0]*size[1];i++) out[i]=max(in1[i],in2);\
							}							

#define d2d0maxd2(in1,size,in2,out)		{int i;\
							for (i=0;i<size[0]*size[1];i++) out[i]=max(in1[i],in2);\
							}
							
#define s0s2maxs2(in1,in2,size,out)		{int i;\
							for (i=0;i<size[0]*size[1];i++) out[i]=max(in1,in2[i]);\
							}
												
#define d0d2maxd2(in1,in2,size,out)		{int i;\
							for (i=0;i<size[0]*size[1];i++) out[i]=max(in1,in2[i]);\
							}
		
#endif /* !__INT_MAX_H__ */
