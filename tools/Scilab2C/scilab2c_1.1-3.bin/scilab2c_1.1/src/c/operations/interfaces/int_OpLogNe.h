/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Aranud Torset
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* Idem that OpLogEq with a !(negation) behind*/

#ifndef __OPLOGNE_H__
#define __OPLOGNE_H__

#include "floatComplex.h"
#include "doubleComplex.h"

#define s0s0OpLogNes0(in1,in2)	 (float)  !(in1 == in2)
#define d0d0OpLogNed0(in1,in2)	 (double)  !(in1 == in2)
#define c0c0OpLogNes0(in1,in2) 	 (float)  !((creals(in1) == creals(in2)) && (cimags(in1) == cimags(in2)))
#define z0z0OpLogNed0(in1,in2) 	 (double)  !((zreals(in1) == zreals(in2)) && (zimags(in1) == zimags(in2)))

#define s0c0OpLogNes0(in1,in2)	(float) !((in1==creals(in2)) && (0==cimags(in2)))
#define d0z0OpLogNed0(in1,in2)	(double) !((in1==zreals(in2)) && (0==zimags(in2)))
#define c0s0OpLogNes0(in1,in2)	s0c0OpLogNes0(in2,in1)
#define z0d0OpLogNed0(in1,in2)	d0z0OpLogNed0(in2,in1)



#define s2s0OpLogNes2(in1,size,in2,out) 	{int i;\
							for (i=0;i<size[0]*size[1];i++) out[i]=(float)!(in1[i]==in2);\
							}
#define d2d0OpLogNed2(in1,size,in2,out) 	{int i;\
							for (i=0;i<size[0]*size[1];i++) out[i]=(double)!(in1[i]==in2);\
							}								
#define c2c0OpLogNes2(in1,size,in2,out) 	{int i;\
							for (i=0;i<size[0]*size[1];i++) out[i]=(float)!((creals(in1[i])==creals(in2))&&(cimags(in1[i])==cimags(in2)));\
							}		
#define z2z0OpLogNed2(in1,size,in2,out) 	{int i;\
							for (i=0;i<size[0]*size[1];i++) out[i]=(double)!((zreals(in1[i])==zreals(in2))&&(zimags(in1[i])==zimags(in2)));\
							}

		
#define c2s0OpLogNes2(in1,size,in2,out)		c2c0OpLogNes2(in1,size,FloatComplex(in2,0),out)		
#define z2d0OpLogNed2(in1,size,in2,out)		z2z0OpLogNed2(in1,size,DoubleComplex(in2,0),out)

#define s2c0OpLogNes2(in1,size,in2,out) 		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=(float)!((in1[i]==creals(in2))&&(cimags(in2)==0));\
								}
	
#define d2z0OpLogNed2(in1,size,in2,out) 		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=(double)!((in1[i]==zreals(in2))&&(zimags(in2)==0));\
								}



#define  s0s2OpLogNes2(in1,in2,inSize,out) 	s2s0OpLogNes2(in2,inSize,in1,out)
#define  c0s2OpLogNes2(in1,in2,inSize,out)  	s2c0OpLogNes2(in2,inSize,in1,out)
#define  d0d2OpLogNed2(in1,in2,inSize,out)  	d2d0OpLogNed2(in2,inSize,in1,out)
#define  z0d2OpLogNed2(in1,in2,inSize,out)  	d2z0OpLogNed2(in2,inSize,in1,out)
#define  s0c2OpLogNes2(in1,in2,inSize,out)  	c2s0OpLogNes2(in2,inSize,in1,out)
#define  c0c2OpLogNes2(in1,in2,inSize,out)  	c2c0OpLogNes2(in2,inSize,in1,out)
#define  d0z2OpLogNed2(in1,in2,inSize,out)  	z2d0OpLogNed2(in2,inSize,in1,out)
#define  z0z2OpLogNed2(in1,in2,inSize,out) 	z2z0OpLogNed2(in2,inSize,in1,out)

/* we must have size1=size2 */

#define s2s2OpLogNes2(in1,size1,in2,size2,out)	{int i;\
								for (i=0;i<size1[0]*size2[1];i++)	out[i]=(float)!(in1[i]==in2[i]);\
								}
#define d2d2OpLogNed2(in1,size1,in2,size2,out)	{int i;\
								for (i=0;i<size1[0]*size2[1];i++)	out[i]=(double)!(in1[i]==in2[i]);\
								}
#define c2c2OpLogNes2(in1,size1,in2,size2,out) 	{int i;\
								for (i=0;i<size1[0]*size2[1];i++) \
									out[i]=(float)!((creals(in1[i])==creals(in2[i]))&&(cimags(in1[i])==cimags(in2[i])));\
								}
#define z2z2OpLogNed2(in1,size1,in2,size2,out) 	{int i;\
								for (i=0;i<size1[0]*size2[1];i++) \
									out[i]=(double)!((zreals(in1[i])==zreals(in2[i]))&&(zimags(in1[i])==zimags(in2[i])));\
								}
					
#define s2c2OpLogNes2(in1,size1,in2,size2,out) 	{int i;\
								for (i=0;i<size1[0]*size2[1];i++) \
									out[i]=(float)!((in1[i]==creals(in2[i]))&&(0==cimags(in2[i])));\
								}
#define d2z2OpLogNed2(in1,size1,in2,size2,out) 	{int i;\
								for (i=0;i<size1[0]*size2[1];i++) \
									out[i]=(double)!((in1[i]==zreals(in2[i]))&&(0==zimags(in2[i])));\
								}
					
#define  c2s2OpLogNes2(in1,size1,in2,size2,out)		s2c2OpLogNes2(in2,size2,in1,size1,out)	
#define  z2d2OpLogNed2(in1,size1,in2,size2,out)		d2z2OpLogNed2(in2,size2,in1,size1,out)

#endif /* !__OPLOGNE_H__ */
