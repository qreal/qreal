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

#ifndef __INT_FFT_H__
#define __INT_FFT_H__

#define s0ffts0(in)			sffts(in)

#define d0fftd0(in)			dffts(in)

#define c0fftc0(in)			cffts(in)

#define z0fftz0(in)			zffts(in)


#define s2ffts2(in,size,out)		sfftma(in, size[0], size[1], out)

#define d2fftd2(in,size,out)		dfftma(in, size[0], size[1], out)

#define c2fftc2(in,size,out)		cfftma(in, size[0], size[1], out)

#define z2fftz2(in,size,out)		zfftma(in, size[0], size[1], out)

/* FIXME : malloc here */
#define s2fftc2(in,size,out)		{float* ZEROS;\
						ZEROS=malloc((uint)(size[0]*size[1]*sizeof(float));\
						szerosa(ZEROS,size[0],size[1]);\
						cfftma(FloatComplexMatrix(in,ZEROS,size[0]*size[1]), size[0], size[1], out);\
						}
/* FIXME : malloc here */
#define d2fftz2(in,size,out)		{double* ZEROS;\
						ZEROS=malloc((uint)(size[0]*size[1]*sizeof(double));\
						dzerosa(ZEROS,size[0],size[1]);\
						zfftma(DoubleComplexMatrix(in,ZEROS,size[0]*size[1]), size[0], size[1], out);\
						}


	
#define s0s0ffts0(in1,in2)			(in2==-1.0f) ? s0ffts0(in1) : s0iffts0(in1)

#define d0d0fftd0(in1,in2)			(in2==-1.0) ? d0fftd0(in1) : d0ifftd0(in1)

#define c0s0fftc0(in1,in2)			(in2==-1.0f) ? c0fftc0(in1) : c0ifftc0(in1)

#define z0d0fftz0(in1,in2)			(in2==-1.0) ? z0fftz0(in1) : z0ifftz0(in1)

#define s2s0ffts2(in1,size,in2,out)		(in2==-1.0f) ? s2ffts2(in1,size,out) : s2iffts2(in1,size,out)

#define d2d0fftd2(in1,size,in2,out)		(in2==-1.0) ? d2fftd2(in1,size,out) : d2ifftd2(in1,size,out)

#define c2s0fftc2(in1,size,in2,out)		(in2==-1.0f) ? c2fftc2(in1,size,out) : c2ifftc2(in1,size,out)

#define z2d0fftz2(in1,size,in2,out)		(in2==-1.0) ? z2fftz2(in1,size,out) : z2ifftz2(in1,size,out)

#define s2s0fftc2(in1,size,in2,out)		(in2==-1.0f) ? s2fftc2(in1,size,out) : s2ifftc2(in1,size,out)

#define d2d0fftz2(in1,size,in2,out)		(in2==-1.0) ? d2fftz2(in1,size,out) : d2ifftz2(in1,size,out)


#endif /* !__INT_FFT_H__ */
