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

#ifndef __INT_OPCC_H__
#define __INT_OPCC_H__

/*
	/!\	
     / ! \	We used rowcat because disp isn't right. In the real case, it must be columncat for Cc.
    /__!_ \
    
*/


/* Column Cat */

/* Same input elements */

#define s0s0OpCcs2(in1,in2,out)					srowcats(in1,in2,out)
		
#define d0d0OpCcd2(in1,in2,out)					drowcats(in1,in2,out)

#define c0c0OpCcc2(in1,in2,out)					crowcats(in1,in2,out)

#define z0z0OpCcz2(in1,in2,out)					zrowcats(in1,in2,out)

#define s2s2OpCcs2(in1,size1,in2,size2,out)			srowcata(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define d2d2OpCcd2(in1,size1,in2,size2,out)			drowcata(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define c2c2OpCcc2(in1,size1,in2,size2,out)			crowcata(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define z2z2OpCcz2(in1,size1,in2,size2,out)			zrowcata(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

/* Differents input elements */
#define s0c0OpCcc2(in1,in2,out)					c0c0OpCcc2(FloatComplex(in1,0),in2,out)
		
#define d0z0OpCcz2(in1,in2,out)					z0z0OpCcz2(DoubleComplex(in1,0),in2,out)

#define c0s0OpCcc2(in1,in2,out)					c0c0OpCcc2(in1,FloatComplex(in2,0),out)

#define z0d0OpCcz2(in1,in2,out)					z0z0OpCcz2(in1,DoubleComplex(in2,0),out)

#define s2c2OpCcc2(in1,size1,in2,size2,out)			sfilla((float*)out,size1[0],size1[1],0); \
										c2c2OpCcc2(FloatComplexMatrix(in1,(float*)out,size1[0]*size1[1]), size1, in2, size2, out)

#define d2z2OpCcz2(in1,size1,in2,size2,out)			dfilla((double*)out,size1[0],size1[1],0);  \
										z2z2OpCcz2(DoubleComplexMatrix(in1,(double*)out,size1[0]*size1[1]), size1, in2, size2, out)

#define c2s2OpCcc2(in1,size1,in2,size2,out)			sfilla((float*)out,size2[0],size2[1],0);  \
										c2c2OpCcc2(in1, size1, FloatComplexMatrix(in2,(float*)out,size2[0]*size2[1]), size2, out)

#define z2d2OpCcz2(in1,size1,in2,size2,out)			dfilla((double*)out,size2[0],size2[1],0); \
										z2z2OpCcz2(in1, size1, DoubleComplexMatrix(in2,(double*)out,size2[0]*size2[1]), size2, out)

/* Matrix-Scalar */

/* Same type */

#define s2s0OpCcs2(in1,size,in2,out)				{ float temp=in2; \
										  srowcata(in1, size[0], size[1],&temp, 1, 1, out); \
										}

#define d2d0OpCcd2(in1,size,in2,out)				{ double temp=in2; \
										  drowcata(in1, size[0], size[1],&temp, 1, 1, out); \
										}

#define c2c0OpCcc2(in1,size,in2,out)				{ floatComplex temp = in2;   \
										  crowcata(in1, size[0], size[1],&temp, 1, 1, out);\
										}

#define z2z0OpCcz2(in1,size,in2,out)				{ doubleComplex temp =in2;   \
										  zrowcata(in1, size[0], size[1],&temp, 1, 1, out);\
								}


/* Different type */
#define s2c0OpCcc2(in1,size,in2,out)				sfilla((float *)out,size[0],size[1],0); \
										c2c0OpCcc2(FloatComplexMatrix(in1,(float*)out,size[0]*size[1]),size,in2,out)

#define d2z0OpCcz2(in1,size,in2,out)				dfilla((double *)out,size[0],size[1],0); \
										z2z0OpCcz2(DoubleComplexMatrix(in1,(double *)out,size[0]*size[1]),size,in2,out)

#define c2s0OpCcc2(in1,size,in2,out)				{ floatComplex temp = FloatComplex(in2, 0); 	c2c0OpCcc2(in1, size, temp, out)}

#define z2d0OpCcz2(in1,size,in2,out)				{ doubleComplex temp = DoubleComplex(in2, 0); 	z2z0OpCcz2(in1, size, temp, out)}
					

/* Scalar-Matrix */

/* Same type */
#define s0s2OpCcs2(in1,in2,size,out)				{ \
 float __tmp1 = in1 ;\
 srowcata(&__tmp1, 1, 1, in2, size[0], size[1], out);\
}

#define d0d2OpCcd2(in1,in2,size,out)				{ \
 double __tmp1 = in1 ;\
 drowcata(&__tmp1, 1, 1, in2, size[0], size[1], out);\
}

#define c0c2OpCcc2(in1,in2,size,out)				{ \
 floatComplex __tmp1 = in1 ;\
 crowcata(&__tmp1, 1, 1, in2, size[0], size[1], out);\
}

#define z0z2OpCcz2(in1,in2,size,out)				{ \
 doubleComplex __tmp1 = in1 ;\
 zrowcata(&__tmp1, 1, 1, in2, size[0], size[1], out);\
}

/* Different type */
#define s0c2OpCcc2(in1,in2,size,out)				c0c2OpCcc2(FloatComplex(in1,0),in2,size,out)

#define d0z2OpCcz2(in1,in2,size,out)				z0z2OpCcz2(DoubleComplex(in1,0),in2,size,out)

#define c0s2OpCcc2(in1,in2,size,out)				sfilla((float*)out,size[0],size[1],0); \
										c0c2OpCcc2(in1,FloatComplexMatrix(in2,(float*)out,size[0]*size[1]),size,out)

#define z0d2OpCcz2(in1,in2,size,out)				dfilla((double*)out,size[0],size[1],0);  \
										z0z2OpCcz2(in1,DoubleComplexMatrix(in2,(double*)out,size[0]*size[1]),size,out)

#endif /* !__INT_OPCC_H__ */
