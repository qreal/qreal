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

#ifndef __INT_VARIANCE_H__
#define __INT_VARIANCE_H__

#define s0variances0(in)				svariances(in)

#define d0varianced0(in)				dvariances(in)

#define c0variancec0(in)				cvariances(in)
	
#define z0variancez0(in)				zvariances(in)

#define s2variances0(in,size)				svariancea(in, size[0]*size[1])

#define d2varianced0(in,size)				dvariancea(in, size[0]*size[1])

#define c2variancec0(in,size)				cvariancea(in, size[0]*size[1])

#define z2variancez0(in,size)				zvariancea(in, size[0]*size[1])

	
#define s0s0variances0(in1,in2)			(in2==1) ? srowvariances(in1) : scolumnvariances(in1)	

#define d0d0varianced0(in1,in2)			(in2==1) ? drowvariances(in1) : dcolumnvariances(in1)	

#define c0s0variancec0(in1,in2)			(in2==1) ? crowvariances(in1) : ccolumnvariances(in1)	

#define z0d0variancez0(in1,in2)			(in2==1) ? zrowvariances(in1) : zcolumnvariances(in1)	

#define s2s0variances2(in1,size,in2,out)		(in2==1) ? srowvariancea(in1,size[0],size[1],out) : scolumnvariancea(in1,size[0],size[1],out)	

#define d2d0varianced2(in1,size,in2,out)		(in2==1) ? drowvariancea(in1,size[0],size[1],out) : dcolumnvariancea(in1,size[0],size[1],out)

#define c2s0variancec2(in1,size,in2,out)		(in2==1) ? crowvariancea(in1,size[0],size[1],out) : ccolumnvariancea(in1,size[0],size[1],out)

#define z2d0variancez2(in1,size,in2,out)		(in2==1) ? zrowvariancea(in1,size[0],size[1],out) : zcolumnvariancea(in1,size[0],size[1],out)	
		
		
	/*'r'  and 'c' case  */	
#define s0g2variances0(in1,in2,size2)			(in2[0]=='r') ? srowvariances(in1) : scolumnvariances(in1)	

#define d0g2varianced0(in1,in2,size2)			(in2[0]=='r') ? drowvariances(in1) : dcolumnvariances(in1)	

#define c0g2variancec0(in1,in2,size2)			(in2[0]=='r') ? crowvariances(in1) : ccolumnvariances(in1)	

#define z0g2variancez0(in1,in2,size2)			(in2[0]=='r') ? zrowvariances(in1) : zcolumnvariances(in1)	

#define s2g2variances2(in1,size,in2,size2,out)		(in2[0]=='r') ? srowvariancea(in1,size[0],size[1],out) : scolumnvariancea(in1,size[0],size[1],out)	

#define d2g2varianced2(in1,size,in2,size2,out)		(in2[0]=='r') ? drowvariancea(in1,size[0],size[1],out) : dcolumnvariancea(in1,size[0],size[1],out)

#define c2g2variancec2(in1,size,in2,size2,out)		(in2[0]=='r') ? crowvariancea(in1,size[0],size[1],out) : ccolumnvariancea(in1,size[0],size[1],out)

#define z2g2variancez2(in1,size,in2,size2,out)		(in2[0]=='r') ? zrowvariancea(in1,size[0],size[1],out) : zcolumnvariancea(in1,size[0],size[1],out)	
	
	

#endif /* !__INT_VARIANCE_H__ */
