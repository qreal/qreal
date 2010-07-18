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

#ifndef __INT_VARIANCEF_H__
#define __INT_VARIANCEF_H__

#define s0s0variancefs0(in1,in2)					svariancefs(in1,in2)

#define d0d0variancefd0(in1,in2)					dvariancefs(in1,in2)

#define c0s0variancefc0(in1,in2)					cvariancefs(in1,in2)

#define z0d0variancefz0(in1,in2)					zvariancefs(in1,in2)

#define s2s2variancefs0(in1, size1, in2, size2)				svariancefa(in1, size1[0]*size1[1], in2)

#define d2d2variancefd0(in1, size1, in2, size2)				dvariancefa(in1, size1[0]*size1[1], in2)

#define c2s2variancefc0(in1, size1, in2, size2)				cvariancefv(in1, size1[0], size1[1], in2)

#define z2d2variancefz0(in1, size1, in2, size2)				zvariancefv(in1, size1[0], size1[1], in2)


/*by rows or columns */

#define s0s0s0variancefs0(in1,in2,in3)				(in3==1) ? srowvariancefs(in1,in2) : scolumnvariancefs(in1,in2)	

#define d0d0d0variancefd0(in1,in2,in3)				(in3==1) ? drowvariancefs(in1,in2) : dcolumnvariancefs(in1,in2)	

#define c0s0s0variancefc0(in1,in2,in3)				(in3==1) ? crowvariancefs(in1,in2) : ccolumnvariancefs(in1,in2)	

#define z0d0d0variancefz0(in1,in2,in3)				(in3==1) ? zrowvariancefs(in1,in2) : zcolumnvariancefs(in1,in2)	

#define s2s2s0variancefs2(in1, size1, in2, size2,in3,out)		(in3==1) ? srowvariancefa(in1,size1[0],size1[1],in2,out) : scolumnvariancefa(in1,size1[0],size1[1],in2,out)	

#define d2d2d0variancefd2(in1, size1, in2, size2,in3,out)		(in3==1) ? drowvariancefa(in1,size1[0],size1[1],in2,out) : dcolumnvariancefa(in1,size1[0],size1[1],in2,out)

#define c2s2s0variancefc2(in1, size1, in2, size2,in3,out)		(in3==1) ? crowvariancefa(in1,size1[0],size1[1],in2,out) : ccolumnvariancefa(in1,size1[0],size1[1],in2,out)

#define z2d2d0variancefz2(in1, size1, in2, size2,in3,out)		(in3==1) ? zrowvariancefa(in1,size1[0],size1[1],in2,out) : zcolumnvariancefa(in1,size1[0],size1[1],in2,out)	
		
/*
#define d2d2d0variancefd2(in1,size1,in2,size2 ,in3 ,out)		(in3==1) ? drowvariancefa(in1,size1[0],size1[1],in2 ,out) : dcolumnvariancefa(in1,size1[0],size1[1], in2,out)		
*/	
	

#endif /* !__INT_VARIANCEF_H__ */
