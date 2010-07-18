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

#ifndef __INT_SUM_H__
#define __INT_SUM_H__

#define s0sums0(in)				ssums(in)

#define d0sumd0(in)				dsums(in)

#define c0sumc0(in)				csums(in)

#define z0sumz0(in)				zsums(in)

#define s2sums0(in,size)			ssuma(in, size[0]*size[1])

#define d2sumd0(in,size)			dsuma(in, size[0]*size[1])

#define c2sumc0(in,size)			csuma(in, size[0]*size[1])

#define z2sumz0(in,size)			zsuma(in, size[0]*size[1])


#define s0s0sums0(in1,in2)			(in2==1) ? srowsums(in1) : scolumnsums(in1)	

#define d0d0sumd0(in1,in2)			(in2==1) ? drowsums(in1) : dcolumnsums(in1)	

#define c0s0sumc0(in1,in2)			(in2==1) ? crowsums(in1) : ccolumnsums(in1)	

#define z0d0sumz0(in1,in2)			(in2==1) ? zrowsums(in1) : zcolumnsums(in1)	

#define s2s0sums2(in1,size,in2,out)		(in2==1) ? srowsuma(in1,size[0],size[1],out) : scolumnsuma(in1,size[0],size[1],out)	

#define d2d0sumd2(in1,size,in2,out)		(in2==1) ? drowsuma(in1,size[0],size[1],out) : dcolumnsuma(in1,size[0],size[1],out)

#define c2s0sumc2(in1,size,in2,out)		(in2==1) ? crowsuma(in1,size[0],size[1],out) : ccolumnsuma(in1,size[0],size[1],out)

#define z2d0sumz2(in1,size,in2,out)		(in2==1) ? zrowsuma(in1,size[0],size[1],out) : zcolumnsuma(in1,size[0],size[1],out)	
		
		
/*'r'  and 'c' case  */	
#define s0g2sums0(in1,in2,size2)			(in2[0]=='r') ? srowsums(in1) : scolumnsums(in1)	

#define d0g2sumd0(in1,in2,size2)			(in2[0]=='r') ? drowsums(in1) : dcolumnsums(in1)	

#define c0g2sumc0(in1,in2,size2)			(in2[0]=='r') ? crowsums(in1) : ccolumnsums(in1)	

#define z0g2sumz0(in1,in2,size2)			(in2[0]=='r') ? zrowsums(in1) : zcolumnsums(in1)	

#define s2g2sums2(in1,size,in2,size2,out)		(in2[0]=='r') ? srowsuma(in1,size[0],size[1],out) : scolumnsuma(in1,size[0],size[1],out)	

#define d2g2sumd2(in1,size,in2,size2,out)		(in2[0]=='r') ? drowsuma(in1,size[0],size[1],out) : dcolumnsuma(in1,size[0],size[1],out)

#define c2g2sumc2(in1,size,in2,size2,out)		(in2[0]=='r') ? crowsuma(in1,size[0],size[1],out) : ccolumnsuma(in1,size[0],size[1],out)

#define z2g2sumz2(in1,size,in2,size2,out)		(in2[0]=='r') ? zrowsuma(in1,size[0],size[1],out) : zcolumnsuma(in1,size[0],size[1],out)	
	

#endif /* !__INT_SUM_H__ */
