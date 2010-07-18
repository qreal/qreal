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

#ifndef __INT_OPHAT_H__
#define __INT_OPHAT_H__


/* Scalar - Scalar */
#define s0s0OpHats0(in1, in2)			spows(in1, in2)

#define d0d0OpHatd0(in1, in2)			dpows(in1, in2)

#define c0c0OpHatc0(in1, in2)			cpows(in1, in2)

#define s0c0OpHatc0(in1, in2)			cpows(FloatComplex(in1,0), in2)

#define c0s0OpHatc0(in1, in2)			cpows(in1, FloatComplex(in2,0))

#define z0z0OpHatz0(in1, in2)			zpows(in1, in2)

#define d0z0OpHatz0(in1, in2)			zpows(DoubleComplex(in1,0), in2)

#define z0d0OpHatz0(in1, in2)			zpows(in1, DoubleComplex(in2,0))



/* Scalar - Matrix */
#define s0s2OpHats2(in1, in2, size, out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=spows(in1,in2[i]);\
									}

#define d0d2OpHatd2(in1, in2, size, out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=dpows(in1,in2[i]);\
									}


#define c0c2OpHatc2(in1, in2, size, out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cpows(in1,in2[i]);\
									}


#define z0z2OpHatz2(in1, in2, size, out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zpows(in1,in2[i]);\
									}



#define s0c2OpHatc2(in1, in2, size, out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cpows(FloatComplex(in1,0),in2[i]);\
									}

#define d0z2OpHatz2(in1, in2, size, out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zpows(DoubleComplex(in1,0),in2[i]);\
									}

#define c0s2OpHatc2(in1, in2, size, out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cpows(in1,FloatComplex(in2[i],0));\
									}

#define z0d2OpHatz2(in1, in2, size, out)			{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zpows(in1,DoubleComplex(in2[i],0));\
									}


/* Matrix - Scalar */
/* Matrices must be square */

#define s2s0OpHats2(in1,size,in2,out)		spowma(in1,size[0],in2,out);

#define d2d0OpHatd2(in1,size,in2,out)		dpowma(in1,size[0],in2,out);

#define c2c0OpHatc2(in1,size,in2,out)		cpowma(in1,size[0],in2,out);

#define z2z0OpHatz2(in1,size,in2,out)		zpowma(in1,size[0],in2,out);

/* FIXME : malloc here */
#define s2c0OpHatc2(in1,size,in2,out)		{float* tmp;\
								tmp = malloc((uint)(size[0]*size[1])*sizeof(float));\
								szerosa(tmp,size[0],size[1]);\
								cpowma(FloatComplexMatrix(in1,tmp,size[0]*size[1]),size[0],in2,out);\
								free(tmp);\
								}

#define d2z0OpHatz2(in1,size,in2,out)		{double* tmp;\
								tmp = malloc((uint)(size[0]*size[1])*sizeof(double));\
								dzerosa(tmp,size[0],size[1]);\
								zpowma(DoubleComplexMatrix(in1,tmp,size[0]*size[1]),size[0],in2,out);\
								free(tmp);\
								}

#define c2s0OpHatc2(in1,size,in2,out)		cpowma(in1,size[0],FloatComplex(in2,0),out);

#define z2d0OpHatz2(in1,size,in2,out)		zpowma(in1,size[0],DoubleComplex(in2,0),out);





#endif /* !__INT_OPHAT_H__ */
