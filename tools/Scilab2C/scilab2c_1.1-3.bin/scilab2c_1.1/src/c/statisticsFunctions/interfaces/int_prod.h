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

#ifndef __INT_PROD_H__
#define __INT_PROD_H__

#define s0prods0(in)					sprods(in)

#define d0prodd0(in)					dprods(in)

#define c0prodc0(in)					cprods(in)
	
#define z0prodz0(in)					zprods(in)

#define s2prods0(in,size)				sproda(in, size[0]*size[1])

#define d2prodd0(in,size)				dproda(in, size[0]*size[1])

#define c2prodc0(in,size)				cproda(in, size[0]*size[1])

#define z2prodz0(in,size)				zproda(in, size[0]*size[1])

	
#define s0s0prods0(in1,in2)				(in2==1) ? srowprods(in1) : scolumnprods(in1)	

#define d0d0prodd0(in1,in2)				(in2==1) ? drowprods(in1) : dcolumnprods(in1)	

#define c0s0prodc0(in1,in2)				(in2==1) ? crowprods(in1) : ccolumnprods(in1)	

#define z0d0prodz0(in1,in2)				(in2==1) ? zrowprods(in1) : zcolumnprods(in1)	

#define s2s0prods2(in1,size,in2,out)		(in2==1) ? srowproda(in1,size[0],size[1],out) : scolumnproda(in1,size[0],size[1],out)	

#define d2d0prodd2(in1,size,in2,out)		(in2==1) ? drowproda(in1,size[0],size[1],out) : dcolumnproda(in1,size[0],size[1],out)

#define c2s0prodc2(in1,size,in2,out)		(in2==1) ? crowproda(in1,size[0],size[1],out) : ccolumnproda(in1,size[0],size[1],out)

#define z2d0prodz2(in1,size,in2,out)		(in2==1) ? zrowproda(in1,size[0],size[1],out) : zcolumnproda(in1,size[0],size[1],out)	
		
	/*'r'  and 'c' case  */	
#define s0g2prods0(in1,in2,size2)			(in2[0]=='r') ? srowprods(in1) : scolumnprods(in1)	

#define d0g2prodd0(in1,in2,size2)			(in2[0]=='r') ? drowprods(in1) : dcolumnprods(in1)	

#define c0g2prodc0(in1,in2,size2)			(in2[0]=='r') ? crowprods(in1) : ccolumnprods(in1)	

#define z0g2prodz0(in1,in2,size2)			(in2[0]=='r') ? zrowprods(in1) : zcolumnprods(in1)	

#define s2g2prods2(in1,size,in2,size2,out)		(in2[0]=='r') ? srowproda(in1,size[0],size[1],out) : scolumnproda(in1,size[0],size[1],out)	

#define d2g2prodd2(in1,size,in2,size2,out)		(in2[0]=='r') ? drowproda(in1,size[0],size[1],out) : dcolumnproda(in1,size[0],size[1],out)

#define c2g2prodc2(in1,size,in2,size2,out)		(in2[0]=='r') ? crowproda(in1,size[0],size[1],out) : ccolumnproda(in1,size[0],size[1],out)

#define z2g2prodz2(in1,size,in2,size2,out)		(in2[0]=='r') ? zrowproda(in1,size[0],size[1],out) : zcolumnproda(in1,size[0],size[1],out)	
		
	
	

#endif /* !__INT_PROD_H__ */
