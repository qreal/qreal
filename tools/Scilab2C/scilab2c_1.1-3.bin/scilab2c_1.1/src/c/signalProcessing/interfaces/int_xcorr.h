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

#ifndef __INT_XCORR_H__
#define __INT_XCORR_H__

#define s0xcorrs0(in)			in*in

#define d0xcorrd0(in)			in*in

#define c0xcorrc0(in)			cmuls(in,cconjs(in))

#define z0xcorrz0(in)			zmuls(in,zconjs(in))

#define s2xcorrs2(in,size)		s2s2xcorrs2(in,size,in,size,out)

#define d2xcorrd2(in,size)		d2d2xcorrd2(in,size,in,size,out)

#define c2xcorrc2(in,size)		c2c2xcorrc2(in,size,in,size,out)

#define z2xcorrz2(in,size)		z2z2xcorrz2(in,size,in,size,out)


/* Scalar - Scalar */

#define s0s0xcorrs0(in1,in2)			in1*in2

#define d0d0xcorrd0(in1,in2)			in1*in2

#define c0c0xcorrc0(in1,in2)			cmuls(in1,cconjs(in2))

#define z0z0xcorrz0(in1,in2)			zmuls(in1,zconjs(in2))

#define s0c0xcorrs0(in1,in2)			cmuls(FloatComplex(in1,0),cconjs(in2))

#define d0z0xcorrd0(in1,in2)			zmuls(DoubleComplex(in1,0),zconjs(in2))

#define c0s0xcorrc0(in1,in2)			cmuls(in1,FloatComplex(in2,0))

#define z0d0xcorrz0(in1,in2)			zmuls(in1,DoubleComplex(in2,0))

/* Scalar - Scalar*/

#define s0s2xcorrs2(in1,in2,size,out) 		{int i;\
								int mn=size[0]*size[1];\
								for (i=1;i<=mn;i++) out[i]=in1*in2[mn-i];\
								}

#define d0d2xcorrd2(in1,in2,size,out) 		{int i;\
								int mn=size[0]*size[1];\
								for (i=1;i<=mn;i++) out[i]=in1*in2[mn-i];\
								}

#define c0c2xcorrc2(in1,in2,size,out) 		{int i;\
								int mn=size[0]*size[1];\
								for (i=1;i<=mn;i++) out[i]=cmuls(in1,cconjs(in2[mn-i]));\
								}

#define z0z2xcorrz2(in1,in2,size,out) 		{int i;\
								int mn=size[0]*size[1];\
								for (i=1;i<=mn;i++) out[i]=zmuls(in1,zconjs(in2[mn-i]));\
								}
								
#define s0c2xcorrs2(in1,in2,size,out) 		{int i;\
								int mn=size[0]*size[1];\
								for (i=1;i<=mn;i++) out[i]=cmuls(FloatComplex(in1,0),cconjs(in2[mn-i]));\
								}

#define d0z2xcorrd2(in1,in2,size,out) 		{int i;\
								int mn=size[0]*size[1];\
								for (i=1;i<=mn;i++) out[i]=zmuls(DoubleComplex(in1,0),zconjs(in2[mn-i]));\
								}

#define c0s2xcorrc2(in1,in2,size,out) 		{int i;\
								int mn=size[0]*size[1];\
								for (i=1;i<=mn;i++) out[i]=cmuls(in1,FloatComplex(in2[mn-i],0));\
								}

#define z0d2xcorrz2(in1,in2,size,out) 		{int i;\
								int mn=size[0]*size[1];\
								for (i=1;i<=mn;i++) out[i]=zmuls(in1,DoubleComplex(in2[mn-i],0)));\
								}





/* Matrix - Scalar*/

#define s2s0xcorrs2(in1,size,in2,out)		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]*in2;\
								}	

#define d2d0xcorrd2(in1,size,in2,out)		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]*in2;\
								}	

#define c2c0xcorrc2(in1,size,in2,out)		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1[i],cconjs(in2));\
								}

#define z2z0xcorrz2(in1,size,in2,out)		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1[i],zconjs(in2));\
								}

#define s2c0xcorrc2(in1,size,in2,out)		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]*in2;\
								}	

#define d2z0xcorrz2(in1,size,in2,out)		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]*in2;\
								}	

#define c2s0xcorrc2(in1,size,in2,out)		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=cmuls(in1[i],FloatComplex(in2,0));\
								}

#define z2d0xcorrz2(in1,size,in2,out)		{int i;\
								for (i=0;i<size[0]*size[1];i++) out[i]=zmuls(in1[i],DoubleComplex(in2,0));\
								}



/* Matrix - Matrix */

#define s2s2xcorrs2(in1,size1,in2,size2,out)		scrossCorra(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define d2d2xcorrd2(in1,size1,in2,size2,out)		dcrossCorra(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define c2c2xcorrc2(in1,size1,in2,size2,out)		ccrossCorra(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define z2z2xcorrz2(in1,size1,in2,size2,out)		zcrossCorra(in1, size1[0], size1[1], in2, size2[0], size2[1], out)


/*FIXME : malloc here*/
#define s2c2xcorrc2(in1,size1,in2,size2,out)		{float* temp;\
									temp=malloc((uint)(size1[0]*size1[1])*sizeof(float));\
									sfilla(temp,size1[0],size1[1],0);\
									c2c2xcorrc2(FloatComplex(in1,0,size1[0]*size1[1]),size1,in2,size2,out);\
									free(temp);\
									}

#define d2z2xcorrz2(in1,size1,in2,size2,out)		{double* temp;\
									temp=malloc((uint)(size1[0]*size1[1])*sizeof(double));\
									dfilla(temp,size[0],size[1],0);\
									z2z2xcorrz2(DoubleComplex(in1,0,size1[0]*size1[1]),size1,in2,size2,out);\
									free(temp);\
									}


#define c2s2xcorrc2(in1,size1,in2,size2,out)		{float* temp;\
									temp=malloc((uint)(size2[0]*size2[1])*sizeof(float));\
									sfilla(temp,size2[0],size2[1],0);\
									c2c2xcorrc2(in1,size1,FloatComplex(in2,0,size2[0]*size2[1]),size2,out);\
									free(temp);\
									}

#define z2d2xcorrz2(in1,size1,in2,size2,out)		{double* temp;\
									temp=malloc((uint)(size2[0]*size2[1])*sizeof(double));\
									sfilla(temp,size2[0],size2[1],0);\
									c2c2xcorrc2(in1,size1,DoubleComplex(in2,0,size2[0]*size2[1]),size2,out);\
									free(temp);\
									}

#endif /* !__INT_XCORR_H__ */
