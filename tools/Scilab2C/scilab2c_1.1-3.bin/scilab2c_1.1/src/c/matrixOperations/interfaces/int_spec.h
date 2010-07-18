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

#ifndef __INT_SPEC_H__
#define __INT_SPEC_H__

/* Only eigenvalues */ 

#define s0specc0(in)			FloatComplex  (sspecs(in)  , 0)

#define d0specz0(in)			DoubleComplex ( dspecs(in) , 0)

#define c0specc0(in)			cspecs(in)

#define z0specz0(in)			zspecs(in)

#define s2specc2(in,size,out)		{float* ZEROS;\
						ZEROS=malloc((uint)(size[0]*size[0]*sizeof(float)));\
						szerosa(ZEROS,size[0],size[0]);\
						cspeca(FloatComplexMatrix(in,ZEROS,size[0]*size[0]), size[0], out);\
						}

#define d2specz2(in,size,out)		{double* ZEROS;\
						ZEROS=malloc((uint)(size[0]*size[0]*sizeof(double)));\
						dzerosa(ZEROS,size[0],size[0]);\
						zspeca(DoubleComplexMatrix(in,ZEROS,size[0]*size[0]), size[0] , out);\
						}

#define c2specc2(in,size,out)		cspeca(in, size[0], out)

#define z2specz2(in,size,out)		zspeca(in, size[0], out)

/* Eigenvalues and eigenvectors */

#define s0specc0c0(in,out)			cspec2s(FloatComplex( in,0) ,out)

#define d0specz0z0(in,out)			zspec2s(DoubleComplex(in,0) ,out)

#define c0specc0c0(in,out)			cspec2s(in,out)

#define z0specz0z0(in,out)			zspec2s(in,out)

#define s2specc2c2(in,size,out1,out2)		{float* ZEROS;\
						ZEROS=malloc((uint)(size[0]*size[0]*sizeof(float)));\
						szerosa(ZEROS,size[0],size[0]);\
						cspec2a(FloatComplexMatrix(in,ZEROS,size[0]*size[0]), size[0] , out1 , out2 );\
						}

#define d2specz2z2(in,size,out1,out2)		{double* ZEROS;\
						ZEROS=malloc((uint)(size[0]*size[0]*sizeof(double)));\
						dzerosa(ZEROS,size[0],size[0]);\
						zspec2a(DoubleComplexMatrix(in,ZEROS,size[0]*size[0]), size[0] , out1 , out2 );\
						}


#define c2specc2c2(in,size,out1,out2)		cspec2a(in, size[0], out2, out1)

#define z2specz2z2(in,size,out1,out2)		zspec2a(in, size[0], out2, out1)

#endif /* !__INT_SPEC_H__ */
