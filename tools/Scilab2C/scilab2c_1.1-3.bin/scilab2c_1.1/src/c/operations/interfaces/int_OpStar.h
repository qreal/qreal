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

#ifndef __INT_OPSTAR_H__
#define __INT_OPSTAR_H__

/* Scalar * Scalar */

#define s0s0OpStars0(in1,in2)				smuls(in1,in2)

#define d0d0OpStard0(in1,in2)				dmuls(in1,in2)
	
#define c0c0OpStarc0(in1,in2)				cmuls(in1,in2)

#define z0z0OpStarz0(in1,in2)				zmuls(in1,in2)

#define s0c0OpStarc0(in1,in2)				cmuls(FloatComplex(in1,0),in2)

#define c0s0OpStarc0(in1,in2)				cmuls(in1,FloatComplex(in2,0))

#define d0z0OpStarz0(in1,in2)				zmuls(DoubleComplex(in1,0),in2)

#define z0d0OpStarz0(in1,in2)				zmuls(in1,DoubleComplex(in2,0))


/* Scalar * Matrix */


#define s0s2OpStars2(in1,in2,size,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1*in2[i];}


#define d0d2OpStard2(in1,in2,size,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1*in2[i];}


#define c0c2OpStarc2(in1,in2,size,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1,in2[i]);}


#define z0z2OpStarz2(in1,in2,size,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1,in2[i]);}

#define s0c2OpStarc2(in1,in2,size,out)			c0c2OpStarc2(FloatComplex(in1,0),in2,size,out)


#define d0z2OpStarz2(in1,in2,size,out)			z0z2OpStarz2(DoubleComplex(in1,0),in2,size,out)


#define c0s2OpStarc2(in1,in2,size,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1,FloatComplex(in2[i],0));}

#define z0d2OpStarz2(in1,in2,size,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1,DoubleComplex(in2[i],0));}


/* Matrix * Scalar */


#define s2s0OpStars2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]*in2;}
									

#define d2d0OpStard2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]*in2;}


#define c2c0OpStarc2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1[i],in2);}

#define z2z0OpStarz2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1[i],in2);}


#define s2c0OpStarc2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(FloatComplex(in1[i],0),in2);}
									

#define d2z0OpStarz2(in1,size,in2,out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(DoubleComplex(in1[i],0),in2);}


#define c2s0OpStarc2(in1,size,in2,out)			c2c0OpStarc2(in1,size,FloatComplex(in2,0),out)


#define z2d0OpStarz2(in1,size,in2,out)			z2z0OpStarz2(in1,size,DoubleComplex(in2,0),out)

/* Matrix * Matrix */

#define s2s2OpStars2(in1,size1,in2,size2,out)		smulma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define d2d2OpStard2(in1,size1,in2,size2,out)		dmulma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define c2c2OpStarc2(in1,size1,in2,size2,out)		cmulma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define z2z2OpStarz2(in1,size1,in2,size2,out)		zmulma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

/* FIXME: There is some malloc in the define below, maybe they can be remove */
#define c2s2OpStarc2(in1,size1,in2,size2,out)		{float* temp;\
									temp=malloc((uint)(size2[0]*size2[1])*sizeof(float));\
									sfilla(temp,size2[0],size2[1],0);\
									c2c2OpStarc2(in1, size1, FloatComplexMatrix(in2,temp,size2[0]*size2[1]), size2, out);}

#define s2c2OpStarc2(in1,size1,in2,size2,out)		{float* temp;\
									temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
									sfilla(temp,size1[0],size1[1],0);\
									c2c2OpStarc2(FloatComplexMatrix(in1,temp,size1[0]*size1[1]), size1, in2, size2, out);}

#define z2d2OpStarz2(in1,size1,in2,size2,out)		{double* temp;\
									temp=malloc((uint)(size2[0]*size2[1])*sizeof(double));\
									dfilla(temp,size2[0],size2[1],0);\
									z2z2OpStarz2(in1, size1, DoubleComplexMatrix(in2,temp,size2[0]*size2[1]), size2, out);}

#define d2z2OpStarz2(in1,size1,in2,size2,out)		{double* temp;\
									temp=malloc((uint)(size2[0]*size2[1])*sizeof(double));\
									dfilla(temp,size1[0],size1[1],0);\
									z2z2OpStarz2(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]), size1, in2, size2, out);}

/* Vector * Vector, so there is a scalar output */

#define MAX(a, b)	(a > b ? a : b)

#define s2s2OpStars0(in1, size1, in2, size2)		smulv(  in1, in2, MAX(MAX(size1[0], size1[1]), MAX(size2[0], size2[1])))

#define c2s2OpStarc0(in1, size1, in2, size2)		cmulcsv(in1, in2, MAX(MAX(size1[0], size1[1]), MAX(size2[0], size2[1])))

#define s2c2OpStarc0(in1, size1, in2, size2)		cmulscv(in1, in2, MAX(MAX(size1[0], size1[1]), MAX(size2[0], size2[1])))

#define c2c2OpStarc0(in1, size1, in2, size2)		cmulv(  in1, in2, MAX(MAX(size1[0], size1[1]), MAX(size2[0], size2[1])))


#define d2d2OpStard0(in1, size1, in2, size2)		dmulv(  in1, in2, MAX(MAX(size1[0], size1[1]), MAX(size2[0], size2[1])))

#define z2d2OpStarz0(in1, size1, in2, size2)		zmulzdv(in1, in2, MAX(MAX(size1[0], size1[1]), MAX(size2[0], size2[1])))

#define d2z2OpStarz0(in1, size1, in2, size2)		zmuldzv(in1, in2, MAX(MAX(size1[0], size1[1]), MAX(size2[0], size2[1])))

#define z2z2OpStarz0(in1, size1, in2, size2)		zmulv(  in1, in2, MAX(MAX(size1[0], size1[1]), MAX(size2[0], size2[1])))

#endif /* !__INT_OPSTAR_H__ */
