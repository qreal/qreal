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

#ifndef __INT_OPDOTHAT_H__
#define __INT_OPDOTHAT_H__


/* Scalar - Scalar */
#define s0s0OpDotHats0(in1, in2)			spows(in1, in2)

#define d0d0OpDotHatd0(in1, in2)			dpows(in1, in2)

#define c0c0OpDotHatc0(in1, in2)			cpows(in1, in2)

#define z0z0OpDotHatz0(in1, in2)			zpows(in1, in2)

#define s0c0OpDotHatc0(in1, in2)			cpows(FloatComplex(in1,0), in2)

#define c0s0OpDotHatc0(in1, in2)			cpows(in1, FloatComplex(in2,0))

#define d0z0OpDotHatz0(in1, in2)			zpows(DoubleComplex(in1,0), in2)

#define z0d0OpDotHatz0(in1, in2)			zpows(in1, DoubleComplex(in2,0))


/* Scalar - Matrix */	
#define s0s2OpDotHats2(in1, in2, size, out)		{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=spows(in1,in2[i]);\
									}

#define d0d2OpDotHatd2(in1, in2, size, out)		{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=dpows(in1,in2[i]);\
									}


#define c0c2OpDotHatc2(in1, in2, size, out)		{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cpows(in1,in2[i]);\
									}


#define z0z2OpDotHatz2(in1, in2, size, out)		{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zpows(in1,in2[i]);\
									}



#define s0c2OpDotHatc2(in1, in2, size, out)		{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cpows(FloatComplex(in1,0),in2[i]);\
									}

#define d0z2OpDotHatz2(in1, in2, size, out)		{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zpows(DoubleComplex(in1,0),in2[i]);\
									}

#define c0s2OpDotHatc2(in1, in2, size, out)		{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=cpows(in1,FloatComplex(in2[i],0));\
									}

#define z0d2OpDotHatz2(in1, in2, size, out)		{int i=0;\
									for (i=0;i<size[0]*size[1];i++) out[i]=zpows(in1,DoubleComplex(in2[i],0));\
									}


/* Matrix - Scalar */

#define s2s0OpDotHats2(in1,size,in2,out)		{int i=0;\
								for (i=0;i<size[0]*size[1];i++) out[i]=spows(in1[i],in2);\
								}

#define d2d0OpDotHatd2(in1,size,in2,out)		{int i=0;\
								for (i=0;i<size[0]*size[1];i++) out[i]=dpows(in1[i],in2);\
								}

#define c2c0OpDotHatc2(in1,size,in2,out)		{int i=0;\
								for (i=0;i<size[0]*size[1];i++) out[i]=cpows(in1[i],in2);\
								}

#define z2z0OpDotHatz2(in1,size,in2,out)		{int i=0;\
								for (i=0;i<size[0]*size[1];i++) out[i]=zpows(in1[i],in2);\
								}

#define s2c0OpDotHatc2(in1,size,in2,out)		{int i=0;\
								for (i=0;i<size[0]*size[1];i++) out[i]=cpows(FloatComplex(in1[i],0),in2);\
								}

#define d2z0OpDotHatz2(in1,size,in2,out)		{int i=0;\
								for (i=0;i<size[0]*size[1];i++) out[i]=zpows(DoubleComplex(in1[i],0),in2);\
								}

#define c2s0OpDotHatc2(in1,size,in2,out)		{int i=0;\
								for (i=0;i<size[0]*size[1];i++) out[i]=cpows(in1[i],FloatComplex(in2,0));\
								}

#define z2d0OpDotHatz2(in1,size,in2,out)		{int i=0;\
								for (i=0;i<size[0]*size[1];i++) out[i]=zpows(in1[i],DoubleComplex(in2,0));\
								}


/* Matrix - Matrix */

#define s2s2OpDotHats2(in1, size1, in2, size2, out)	spowa(in1, size1[0]*size2[1], in2, out)

#define d2d2OpDotHatd2(in1, size1, in2, size2, out)	dpowa(in1, size1[0]*size2[1], in2, out)

#define c2c2OpDotHatc2(in1, size1, in2, size2, out)	cpowa(in1, size1[0]*size2[1], in2, out)

#define z2z2OpDotHatz2(in1, size1, in2, size2, out)	zpowa(in1, size1[0]*size2[1], in2, out)

#define s2c2OpDotHatc2(in1, size1, in2, size2, out)	{int i=0;\
									for (i=0;i<size1[0]*size2[1];i++) out[i]=cpows(FloatComplex(in1[i],0),in2[i]);\
									}

#define c2s2OpDotHatc2(in1, size1, in2, size2, out)	{int i=0;\
									for (i=0;i<size1[0]*size2[1];i++) out[i]=cpows(in1[i],FloatComplex(in2[i],0));\
									}

#define d2z2OpDotHatz2(in1, size1, in2, size2, out)	{int i=0;\
									for (i=0;i<size1[0]*size2[1];i++) out[i]=zpows(DoubleComplex(in1[i],0),in2[i]);\
									}

#define z2d2OpDotHatz2(in1, size1, in2, size2, out)	{int i=0;\
									for (i=0;i<size1[0]*size2[1];i++) out[i]=zpows(in1[i],DoubleComplex(in2[i],0));\
									}




#endif /* !__INT_OPDOTHAT_H__ */
