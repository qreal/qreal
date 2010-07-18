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

#ifndef __INT_OPSLASH_H__
#define __INT_OPSLASH_H__

/* Scalar / Scalar */

#define s0s0OpSlashs0(in1,in2)				srdivs(in1,in2)

#define d0d0OpSlashd0(in1,in2)				drdivs(in1,in2)
	
#define c0c0OpSlashc0(in1,in2)				crdivs(in1,in2)

#define z0z0OpSlashz0(in1,in2)				zrdivs(in1,in2)

#define s0c0OpSlashc0(in1,in2)				crdivs(FloatComplex(in1,0),in2)

#define c0s0OpSlashc0(in1,in2)				crdivs(in1,FloatComplex(in2,0))

#define d0z0OpSlashz0(in1,in2)				zrdivs(DoubleComplex(in1,0),in2)

#define z0d0OpSlashz0(in1,in2)				zrdivs(in1,DoubleComplex(in2,0))


/* Scalar / Matrix */


#define s0s2OpSlashs2(in1,in2,size,out)			{int i=0;\
									sinverma(in2,out,size[0]);\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1*out[i];}


#define d0d2OpSlashd2(in1,in2,size,out)			{int i=0;\
									dinverma(in2,out,size[0]);\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1*out[i];}
									
									
#define c0c2OpSlashc2(in1,in2,size,out)			{int i=0;\
									cinverma(in2,out,size[0]);\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1,out[i]);}


#define z0z2OpSlashz2(in1,in2,size,out)			{int i=0;\
									zinverma(in2,out,size[0]);\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1,out[i]);}



#define s0c2OpSlashc2(in1,in2,size,out)			c0c2OpSlashc2(FloatComplex(in1,0),in2,size,out)


#define d0z2OpSlashz2(in1,in2,size,out)			z0z2OpSlashz2(DoubleComplex(in1,0),in2,size,out)


/* FIXME: malloc */
#define c0s2OpSlashc2(in1,in2,size,out)			{int i=0;\
									float* temp = malloc((uint)(size[0]*size[1])*sizeof(float));\
									sinverma(in2,temp,size[0]);\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1,FloatComplex(temp[i],0));}

/* FIXME: malloc */
#define z0d2OpSlashz2(in1,in2,size,out)			{int i=0;\
									double* temp = malloc((uint)(size[0]*size[1])*sizeof(double));\
									dinverma(in2,temp,size[0]);\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1,DoubleComplex(temp[i],0));}


/* Matrix / Scalar */


#define s2s0OpSlashs2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]/in2;}


#define d2d0OpSlashd2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]/in2;}
									

#define c2c0OpSlashc2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=crdivs(in1[i],in2);}


#define z2z0OpSlashz2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zrdivs(in1[i],in2);}


#define s2c0OpSlashc2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=crdivs(FloatComplex(in1[i],0),in2);}


#define d2z0OpSlashz2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zrdivs(DoubleComplex(in1[i],0),in2);}


#define c2s0OpSlashc2(in1,size,in2,out)			c2c0OpSlashc2(in1,size,FloatComplex(in2,0),out)
			

#define z2d0OpSlashz2(in1,size,in2,out)			z2z0OpSlashz2(in1,size,DoubleComplex(in2,0),out)
			




/* Matrix / Matrix */

#define s2s2OpSlashs2(in1,size1,in2,size2,out)		srdivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define d2d2OpSlashd2(in1,size1,in2,size2,out)		drdivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define c2c2OpSlashc2(in1,size1,in2,size2,out)		crdivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define z2z2OpSlashz2(in1,size1,in2,size2,out)		zrdivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

/* FIXME: There is some malloc in the define below, maybe they can be remove */
#define c2s2OpSlashc2(in1,size1,in2,size2,out)		{float* temp;\
									temp=malloc((uint)(size2[0]*size2[1])*sizeof(float));\
									sfilla(temp,size2[0],size2[1],0);\
									c2c2OpSlashc2(in1, size1, FloatComplexMatrix(in2,temp,size2[0]*size2[1]), size2, out);}

#define s2c2OpSlashc2(in1,size1,in2,size2,out)		{float* temp;\
									temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
									sfilla(temp,size1[0],size1[1],0);\
									c2c2OpSlashc2(FloatComplexMatrix(in2,temp,size1[0]*size1[1]), size1, in2, size2, out);}

#define z2d2OpSlashz2(in1,size1,in2,size2,out)		{double* temp;\
									temp=malloc((uint)(size2[0]*size2[1])*sizeof(double));\
									dfilla(temp,size2[0],size2[1],0);\
									z2z2OpSlashz2(in1, size1, DoubleComplexMatrix(in2,temp,size2[0]*size2[1]), size2, out);}

#define d2z2OpSlashz2(in1,size1,in2,size2,out)		{double* temp;\
									temp=malloc((uint)(size2[0]*size2[1])*sizeof(double));\
									dfilla(temp,size1[0],size1[1],0);\
									z2z2OpSlashz2(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]), size1, in2, size2, out);}


/* Vector * Vector, so there is a scalar output */

#define s2s2OpSlashs0(in1, size1, in2, size2)		srdivv(  in1, in2, size1[1])

#define c2s2OpSlashc0(in1, size1, in2, size2)		crdivcsv(in1, in2, size1[1])

#define s2c2OpSlashc0(in1, size1, in2, size2)		crdivscv(in1, in2, size1[1])

#define c2c2OpSlashc0(in1, size1, in2, size2)		crdivv(  in1, in2, size1[1])


#define d2d2OpSlashd0(in1, size1, in2, size2)		drdivv(  in1, in2, size1[1])

#define z2d2OpSlashz0(in1, size1, in2, size2)		zrdivzdv(in1, in2, size1[1])

#define d2z2OpSlashz0(in1, size1, in2, size2)		zrdivdzv(in1, in2, size1[1])

#define z2z2OpSlashz0(in1, size1, in2, size2)		zrdivv(  in1, in2, size1[1])


#endif /* !__INT_OPSLASH_H__ */
