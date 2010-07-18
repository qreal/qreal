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

#ifndef __INT_OPMINUS_H__
#define __INT_OPMINUS_H__

/* - Scalar */

#define s0OpMinuss0(in)					-in

#define d0OpMinusd0(in)					-in

#define c0OpMinusc0(in)					FloatComplex(-creals(in), -cimags(in))

#define z0OpMinusz0(in)					DoubleComplex(-zreals(in), -zimags(in))

/* - Matrix */

#define s2OpMinuss2(in, size, out)			{int i=0; \
    for (i=0;i<size[0]*size[1];i++) out[i] = -in[i];		  \
  }

#define d2OpMinusd2(in, size, out)			{int i=0; \
    for (i=0;i<size[0]*size[1];i++) out[i] = -in[i];		  \
  }

#define c2OpMinusc2(in, size, out)			{int i=0; \
    for (i=0;i<size[0]*size[1];i++) out[i] = FloatComplex(-creals(in[i]), -cimags(in[i])); \
  }

#define z2OpMinusz2(in, size, out)			{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i] = DoubleComplex(-zreals(in[i]), -zimags(in[i])); \
  }


/* Scalar - Scalar */

#define s0s0OpMinuss0(in1,in2)				sdiffs(in1,in2)

#define d0d0OpMinusd0(in1,in2)				ddiffs(in1,in2)

#define c0c0OpMinusc0(in1,in2)				cdiffs(in1,in2)

#define z0z0OpMinusz0(in1,in2)				zdiffs(in1,in2)

#define s0c0OpMinusc0(in1,in2)				cdiffs(FloatComplex(in1,0),in2)

#define c0s0OpMinusc0(in1,in2)				cdiffs(in1,FloatComplex(in2,0))

#define d0z0OpMinusz0(in1,in2)				zdiffs(DoubleComplex(in1,0),in2)

#define z0d0OpMinusz0(in1,in2)				zdiffs(in1,DoubleComplex(in2,0))

/* Matrix - Scalar */


#define s2s0OpMinuss2(in1,size,in2,out)		{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]-in2;		\
  }


#define d2d0OpMinusd2(in1,size,in2,out)		{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i]=in1[i]-in2;		\
  }


#define c2c0OpMinusc2(in1,size,in2,out)		{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i]=cdiffs(in1[i],in2);	\
  }

#define c2s0OpMinusc2(in1,size,in2,out)		c2c0OpMinusc2(in1,size,FloatComplex(in2,0),out)


#define s2c0OpMinusc2(in1,size,in2,out)		{int i=0;		\
    for (i=0;i<size[0]*size[1];i++) out[i]=cdiffs(FloatComplex(in1[i],0),in2); \
  }


#define z2z0OpMinusz2(in1,size,in2,out)		{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i]=zdiffs(in1[i],in2);	\
  }

#define z2d0OpMinusz2(in1,size,in2,out)		z2z0OpMinusz2(in1,size,DoubleComplex(in2,0),out)


#define d2z0OpMinusz2(in1,size,in2,out)		{int i=0;		\
    for (i=0;i<size[0]*size[1];i++) out[i]=zdiffs(DoubleComplex(in1[i],0),in2);	\
  }


/* Scalar - Matrix */


#define s0s2OpMinuss2(in1,in2,size,out)		{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i]=in1-in2[i];		\
  }


#define d0d2OpMinusd2(in1,in2,size,out)		{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i]=in1-in2[i];		\
  }


#define c0c2OpMinusc2(in1,in2,size,out)		{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i]=cdiffs(in1,in2[i]);	\
  }

#define s0c2OpMinusc2(in1,in2,size,out)		c0c2OpMinusc2(FloatComplex(in1,0),in2,size,out)


#define c0s2OpMinusc2(in1,in2,size,out)		{int i=0;		\
    for (i=0;i<size[0]*size[1];i++) out[i]=cdiffs(in1,FloatComplex(in2[i],0)); \
  }


#define z0z2OpMinusz2(in1,in2,size,out)		{int i=0;	\
    for (i=0;i<size[0]*size[1];i++) out[i]=zdiffs(in1,in2[i]);	\
  }

#define d0z2OpMinusz2(in1,in2,size,out)		z0z2OpMinusz2(DoubleComplex(in1,0),in2,size,out)


#define z0d2OpMinusz2(in1,in2,size,out)		{int i=0;		\
    for (i=0;i<size[0]*size[1];i++) out[i]=zdiffs(in1,DoubleComplex(in2[i],0));	\
  }



/* Matrix - Matrix */

#define s2s2OpMinuss2(in1,size1,in2,size2,out)		sdiffa(in1, size1[0]*size1[1], in2, size2[0]*size2[1], out)

#define d2d2OpMinusd2(in1,size1,in2,size2,out)		ddiffa(in1, size1[0]*size1[1], in2, size2[0]*size2[1], out)

#define c2c2OpMinusc2(in1,size1,in2,size2,out)		cdiffa(in1, size1[0]*size1[1], in2, size2[0]*size2[1], out)


#define s2c2OpMinusc2(in1,size1,in2,size2,out)		{int i;		\
    for (i=0;i<size1[0]*size2[1];i++) out[i]=cdiffs(FloatComplex(in1[i],0),in2[i]); \
  }


#define c2s2OpMinusc2(in1,size1,in2,size2,out)		{int i;		\
    for (i=0;i<size1[0]*size2[1];i++) out[i]=cdiffs(in1[i],FloatComplex(in2[i],0));\
  }

#define z2z2OpMinusz2(in1,size1,in2,size2,out)		zdiffa(in1, size1[0]*size1[1], in2, size2[0]*size2[1], out)


#define d2z2OpMinusz2(in1,size1,in2,size2,out)		{int i;		\
    for (i=0;i<size1[0]*size2[1];i++) out[i]=zdiffs(DoubleComplex(in1[i],0),in2[i]); \
  }


#define z2d2OpMinusz2(in1,size1,in2,size2,out)		{int i;		\
    for (i=0;i<size1[0]*size2[1];i++) out[i]=zdiffs(in1[i],DoubleComplex(in2[i],0)); \
  }




#endif /* !__INT_OPMINUS_H__ */
