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

#ifndef __INT_OPDOTBACKSLASH_H__
#define __INT_OPDOTBACKSLASH_H__

/* Scalar .\ Scalar */

#define s0s0OpDotBackSlashs0(in1,in2)				sldivs(in1,in2)

#define d0d0OpDotBackSlashd0(in1,in2)				dldivs(in1,in2)
	
#define c0c0OpDotBackSlashc0(in1,in2)				cldivs(in1,in2)

#define z0z0OpDotBackSlashz0(in1,in2)				zldivs(in1,in2)

#define s0c0OpDotBackSlashc0(in1,in2)				cldivs(FloatComplex(in1,0),in2)

#define c0s0OpDotBackSlashc0(in1,in2)				cldivs(in1,FloatComplex(in2,0))

#define d0z0OpDotBackSlashz0(in1,in2)				zldivs(DoubleComplex(in1,0),in2)

#define z0d0OpDotBackSlashz0(in1,in2)				zldivs(in1,DoubleComplex(in2,0))


/* Scalar \ Matrix */

#define s0s2OpDotBackSlashs2(in1,in2,size,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= sldivs(in1,in2[i]);}

#define d0d2OpDotBackSlashd2(in1,in2,size,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= dldivs(in1,in2[i]);}
									
#define c0c2OpDotBackSlashc2(in1,in2,size,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= cldivs(in1,in2[i]);}

#define z0z2OpDotBackSlashz2(in1,in2,size,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= zldivs(in1,in2[i]);}

#define s0c2OpDotBackSlashc2(in1,in2,size,out)			c0c2OpDotBackSlashc2(FloatComplex(in1,0),in2,size,out)

#define d0z2OpDotBackSlashz2(in1,in2,size,out)			z0z2OpDotBackSlashz2(DoubleComplex(in1,0),in2,size,out)

#define c0s2OpDotBackSlashc2(in1,in2,size,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= cldivs(in1,FloatComplex(in2[i],0));}

#define z0d2OpDotBackSlashz2(in1,in2,size,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= zldivs(in1,DoubleComplex(in2[i],0));}


/* Matrix \ Scalar */

#define s2s0OpDotBackSlashs2(in1,size,in2,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= sldivs(in1[i],in2);}

#define d2d0OpDotBackSlashd2(in1,size,in2,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= dldivs(in1[i],in2);}

#define c2c0OpDotBackSlashc2(in1,size,in2,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= cldivs(in1[i],in2);}

#define z2z0OpDotBackSlashz2(in1,size,in2,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= zldivs(in1[i],in2);}

#define s2c0OpDotBackSlashc2(in1,size,in2,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= cldivs(FloatComplex(in1[i],0),in2);}

#define d2z0OpDotBackSlashz2(in1,size,in2,out)			{int i;\
										for(i=0;i<size[0]*size[1];i++) out[i]= zldivs(DoubleComplex(in1[i],0),in2);}

#define c2s0OpDotBackSlashc2(in1,size,in2,out)			c2c0OpDotBackSlashc2(in1,size,FloatComplex(in2,0),out)

#define z2d0OpDotBackSlashz2(in1,size,in2,out)			z2z0OpDotBackSlashz2(in1,size,DoubleComplex(in2,0),out)

/* Matrix .\ Matrix */

#define s2s2OpDotBackSlashs2(in1,size1,in2,size2,out)		sldiva(in1,in2,size2[0]*size2[1],out)

#define d2d2OpDotBackSlashd2(in1,size1,in2,size2,out)		dldiva(in1,in2,size2[0]*size2[1],out)

#define c2c2OpDotBackSlashc2(in1,size1,in2,size2,out)		cldiva(in1,in2,size2[0]*size2[1],out)

#define c2s2OpDotBackSlashc2(in1,size1,in2,size2,out)		{int i;\
										for(i=0;i<size1[0]*size2[1];i++) out[i]= cldivs(in1[i],FloatComplex(in2[i], 0));}

#define s2c2OpDotBackSlashc2(in1,size1,in2,size2,out)		{int i;	\
										for(i=0;i<size1[0]*size2[1];i++) out[i]= cldivs(FloatComplex(in1[i], 0),in2[i]);}

#define z2z2OpDotBackSlashz2(in1,size1,in2,size2,out)		zldiva(in1,in2,size2[0]*size2[1],out)

#define z2d2OpDotBackSlashz2(in1,size1,in2,size2,out)		{int i;\
										for(i=0;i<size1[0]*size2[1];i++) out[i]= zldivs(in1[i],DoubleComplex(in2[i], 0));}

#define d2z2OpDotBackSlashz2(in1,size1,in2,size2,out)		{int i;\
										for(i=0;i<size1[0]*size2[1];i++) out[i]= zldivs(DoubleComplex(in1[i], 0),in2[i]);}


#endif /* !__INT_OPDOTBACKSLASH_H__ */
