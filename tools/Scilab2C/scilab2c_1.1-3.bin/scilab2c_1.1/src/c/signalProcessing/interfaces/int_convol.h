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

#ifndef __INT_CONVOL_H__
#define __INT_CONVOL_H__


/* Scalar - Scalar */

#define s0s0convols0(in1,in2)			in1*in2

#define d0d0convold0(in1,in2)			in1*in2

#define c0c0convolc0(in1,in2)			cmuls(in1,in2)

#define z0z0convolz0(in1,in2)			zmuls(in1,in2)

#define s0c0convolc0(in1,in2)			cmuls(FloatComplex(in1,0),in2)

#define d0z0convolz0(in1,in2)			zmuls(DoubleComplex(in1,0),in2)

#define c0s0convolc0(in1,in2)			cmuls(in1,FloatComplex(in2,0))

#define z0d0convolz0(in1,in2)			zmuls(in1,DoubleComplex(in2,0))

/* Scalar - Matrix */

#define s0s2convols2(in1,in2,size,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1*in2[i];\
									}										

#define d0d2convold2(in1,in2,size,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1*in2[i];\
									}

#define c0c2convolc2(in1,in2,size,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1,in2[i]);\
									}

#define z0z2convolz2(in1,in2,size,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1,in2[i]);\
									}

#define s0c2convolc2(in1,in2,size,out)			c0c2convolc2(FloatComplex(in1,0),in2,size,out)

#define d0z2convolz2(in1,in2,size,out)			z0z2convolz2(DoubleComplex(in1,0),in2,size,out)

#define c0s2convolc2(in1,in2,size,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1,FloatComplex(in2[i],0));\
									}

#define z0d2convolz2(in1,in2,size,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1,DoubleComplex(in2[i],0));\
									}

/* Matrix - Scalar */

#define s2s0convols2(in1,size,in2,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]*in2;\
									}
										
#define d2d0convold2(in1,size,in2,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]*in2;\
									}	

#define c2c0convolc2(in1,size,in2,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1[i],in2);\
									}	

#define z2z0convolz2(in1,size,in2,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1[i],in2);\
									}	

#define s2c0convolc2(in1,size,in2,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(FloatComplex(in1[i],0),in2);\
									}	

#define d2z0convolz2(in1,size,in2,out)			{int i;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(DoubleComplex(in1[i],0),in2);\
									}	

#define c2s0convolc2(in1,size,in2,out)			c2c0convolc2(in1,size,FloatComplex(in2,0),out)	

#define z2d0convolz2(in1,size,in2,out)			z2z0convolz2(in1,size,DoubleComplex(in2,0),out)	


/* Matrix - Matrix */

#define s2s2convols2(in1,size1,in2,size2,out)				if(((size1[0]==1)||(size1[1]==1))&&((size2[0]==1)||(size2[1]==1))){\
												sconva(in1,size1[0]*size1[1],in2,size2[0]*size2[1],out);\
											}else{\
												sconv2da(in1,size1[0],size1[1],in2,size2[0],size2[1],out);\
											}

#define d2d2convold2(in1,size1,in2,size2,out)				if(((size1[0]==1)||(size1[1]==1))&&((size2[0]==1)||(size2[1]==1))){\
												dconva(in1,size1[0]*size1[1],in2,size2[0]*size2[1],out);\
											}else{\
												dconv2da(in1,size1[0],size1[1],in2,size2[0],size2[1],out);\
											}

#define c2c2convolc2(in1,size1,in2,size2,out)				if(((size1[0]==1)||(size1[1]==1))&&((size2[0]==1)||(size2[1]==1))){\
												cconva(in1,size1[0]*size1[1],in2,size2[0]*size2[1],out);\
											}else{\
												cconv2da(in1,size1[0],size1[1],in2,size2[0],size2[1],out);\
											}

#define z2z2convolz2(in1,size1,in2,size2,out)				if(((size1[0]==1)||(size1[1]==1))&&((size2[0]==1)||(size2[1]==1))){\
												zconva(in1,size1[0]*size1[1],in2,size2[0]*size2[1],out);\
											}else{\
												zconv2da(in1,size1[0],size1[1],in2,size2[0],size2[1],out);\
											}

#define s2c2convolc2(in1,size1,in2,size2,out)				{float* temp;\
											temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
											sfilla(temp,size1[0],size1[1],0);\
											c2c2convolc2(FloatComplexMatrix(in1,temp,size1[0]*size1[1]),size1,in2,size2,out);\
											free(temp);\
											}
											
#define d2z2convolz2(in1,size1,in2,size2,out)				{double* temp;\
											temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
											dfilla(temp,size1[0],size1[1],0);\
											z2z2convolz2(DoubleComplexMatrix(in1,temp,size1[0]*size1[1]),size1,in2,size2,out);\
											free(temp);\
											}

#define c2s2convolc2(in1,size1,in2,size2,out)				{float* temp;\
											temp=malloc((uint)(size2[0]*size2[1])*sizeof(float));\
											sfilla(temp,size2[0],size2[1],0);\
											c2c2convolc2(in1,size1,FloatComplexMatrix(in2,temp,size2[0]*size2[1]),size2,out);\
											free(temp);\
											}


#define z2d2convolz2(in1,size1,in2,size2,out)				{double* temp;\
											temp=malloc((uint)(size2[0]*size2[1])*sizeof(double));\
											dfilla(temp,size2[0],size2[1],0);\
											z2z2convolz2(in1,size1,DoubleComplexMatrix(in2,temp,size2[0]*size2[1]),size2,out);\
											free(temp);\
											}

#endif /* !__INT_CONVOL_H__ */
