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

#ifndef __INT_MEAN_H__
#define __INT_MEAN_H__

#define s0means0(in)					smeans(in)

#define d0meand0(in)					dmeans(in)

#define c0meanc0(in)					cmeans(in)

#define z0meanz0(in)					zmeans(in)

#define s2means0(in,size)				smeana(in, size[0]*size[1])

#define d2meand0(in,size)				dmeana(in, size[0]*size[1])

#define c2meanc0(in,size)				cmeana(in, size[0]*size[1])

#define z2meanz0(in,size)				zmeana(in, size[0]*size[1])


#define s0s0means0(in1,in2)				(in2==1) ? srowmeans(in1) : scolumnmeans(in1)	

#define d0d0meand0(in1,in2)				(in2==1) ? drowmeans(in1) : dcolumnmeans(in1)	

#define c0s0meanc0(in1,in2)				(in2==1) ? crowmeans(in1) : ccolumnmeans(in1)	

#define z0d0meanz0(in1,in2)				(in2==1) ? zrowmeans(in1) : zcolumnmeans(in1)	

#define s2s0means2(in1,size,in2,out)		(in2==1) ? srowmeana(in1,size[0],size[1],out) : scolumnmeana(in1,size[0],size[1],out)	

#define d2d0meand2(in1,size,in2,out)		(in2==1) ? drowmeana(in1,size[0],size[1],out) : dcolumnmeana(in1,size[0],size[1],out)

#define c2s0meanc2(in1,size,in2,out)		(in2==1) ? crowmeana(in1,size[0],size[1],out) : ccolumnmeana(in1,size[0],size[1],out)

#define z2d0meanz2(in1,size,in2,out)		(in2==1) ? zrowmeana(in1,size[0],size[1],out) : zcolumnmeana(in1,size[0],size[1],out)	
		
	/*'r'  and 'c' case  */	
#define s0g2means0(in1,in2,size2)			(in2[0]=='r') ? srowmeans(in1) : scolumnmeans(in1)	

#define d0g2meand0(in1,in2,size2)			(in2[0]=='r') ? drowmeans(in1) : dcolumnmeans(in1)	

#define c0g2meanc0(in1,in2,size2)			(in2[0]=='r') ? crowmeans(in1) : ccolumnmeans(in1)	

#define z0g2meanz0(in1,in2,size2)			(in2[0]=='r') ? zrowmeans(in1) : zcolumnmeans(in1)	

#define s2g2means2(in1,size,in2,size2,out)		(in2[0]=='r') ? srowmeana(in1,size[0],size[1],out) : scolumnmeana(in1,size[0],size[1],out)	

#define d2g2meand2(in1,size,in2,size2,out)		(in2[0]=='r') ? drowmeana(in1,size[0],size[1],out) : dcolumnmeana(in1,size[0],size[1],out)

#define c2g2meanc2(in1,size,in2,size2,out)		(in2[0]=='r') ? crowmeana(in1,size[0],size[1],out) : ccolumnmeana(in1,size[0],size[1],out)

#define z2g2meanz2(in1,size,in2,size2,out)		(in2[0]=='r') ? zrowmeana(in1,size[0],size[1],out) : zcolumnmeana(in1,size[0],size[1],out)	
		
	
	

#endif /* !__INT_MEAN_H__ */
