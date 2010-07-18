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

#ifndef __INT_OPRC_H__
#define __INT_OPRC_H__

/*
	/!\	
     / ! \	We used columncat because disp isn't right. In the real case, it must be rowcat for Rc.
    /__!_ \
    
*/


/* Column Cat */

/* Same input elements */

#define s0s0OpRcs2(in1,in2,out)					scolumncats(in1,in2,out)
		
#define d0d0OpRcd2(in1,in2,out)					dcolumncats(in1,in2,out)

#define c0c0OpRcc2(in1,in2,out)					ccolumncats(in1,in2,out)

#define z0z0OpRcz2(in1,in2,out)					zcolumncats(in1,in2,out)

#define s2s2OpRcs2(in1,size1,in2,size2,out)			scolumncata(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define d2d2OpRcd2(in1,size1,in2,size2,out)			dcolumncata(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define c2c2OpRcc2(in1,size1,in2,size2,out)			ccolumncata(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

#define z2z2OpRcz2(in1,size1,in2,size2,out)			zcolumncata(in1, size1[0], size1[1], in2, size2[0], size2[1], out)

/* Differents input elements */
#define s0c0OpRcc2(in1,in2,out)					c0c0OpRcc2(FloatComplex(in1,0),in2,out)
		
#define d0z0OpRcz2(in1,in2,out)					z0z0OpRcz2(DoubleComplex(in1,0),in2,out)

#define c0s0OpRcc2(in1,in2,out)					c0c0OpRcc2(in1,FloatComplex(in2,0),out)

#define z0d0OpRcz2(in1,in2,out)					z0z0OpRcz2(in1,DoubleComplex(in2,0),out)

#define s2c2OpRcc2(in1,size1,in2,size2,out)			sfilla((float*)out,size1[0],size1[1],0); \
										c2c2OpRcc2(FloatComplexMatrix(in1,(float*)out,size1[0]*size1[1]), size1, in2, size2, out)

#define d2z2OpRcz2(in1,size1,in2,size2,out)			dfilla((double*)out,size1[0],size1[1],0);  \
										z2z2OpRcz2(DoubleComplexMatrix(in1,(double*)out,size1[0]*size1[1]), size1, in2, size2, out)

#define c2s2OpRcc2(in1,size1,in2,size2,out)			sfilla((float*)out,size2[0],size2[1],0);  \
										c2c2OpRcc2(in1, size1, FloatComplexMatrix(in2,(float*)out,size2[0]*size2[1]), size2, out)

#define z2d2OpRcz2(in1,size1,in2,size2,out)			dfilla((double*)out,size2[0],size2[1],0);  \
										z2z2OpRcz2(in1, size1, DoubleComplexMatrix(in2,(double*)out,size2[0]*size2[1]), size2, out)

/* Matrix-Scalar */

/* Same type */

#define s2s0OpRcs2(in1,size,in2,out)				{ float temp=in2; \
										  scolumncata(in1, size[0], size[1],&temp, 1, 1, out); \
										}

#define d2d0OpRcd2(in1,size,in2,out)				{ double temp=in2; \
										  dcolumncata(in1, size[0], size[1],&temp, 1, 1, out); \
										}
										
#define c2c0OpRcc2(in1,size,in2,out)				{ floatComplex temp=in2; \
										 ccolumncata(in1, size[0], size[1], &temp, 1, 1, out); \
										}

#define z2z0OpRcz2(in1,size,in2,out)				{ doubleComplex temp=in2; \
										 zcolumncata(in1, size[0], size[1], &temp, 1, 1, out); \
										}

/* Different type */
#define s2c0OpRcc2(in1,size,in2,out)				{ floatComplex temp=in2; \
										sfilla((float *)out,size[0],size[1],0);  \
										scolumncata(FloatComplexMatrix(in1,(float*)out,size[0]*size[1]), size[0], size[1], &temp , 1, 1, out); \
										}

#define d2z0OpRcz2(in1,size,in2,out)				{ doubleComplex temp=in2; \
										dfilla((double *)out,size[0],size[1],0);  \
										zcolumncata(DoubleComplexMatrix(in1,(double *)out,size[0]*size[1]), size[0], size[1], &temp , 1, 1, out); \
										}

#define c2s0OpRcc2(in1,size,in2,out)				{ floatComplex temp = FloatComplex(in2,0);   \
										  ccolumncata(in1, size[0], size[1], &temp, 1, 1, out); \
										}

#define z2d0OpRcz2(in1,size,in2,out)				{ doubleComplex temp = DoubleComplex(in2,0);   \
										   zcolumncata(in1, size[0], size[1], &temp, 1, 1, out); \
										}

/* Scalar-Matrix */

/* Same type */
#define s0s2OpRcs2(in1,in2,size,out)				{ \
 float __tmp1 = in1 ;\
 scolumncata(&__tmp1 , 1, 1, in2, size[0], size[1], out);\
}
#define d0d2OpRcd2(in1,in2,size,out)				{\
 double __tmp1 = in1 ;\
 dcolumncata(&__tmp1 , 1, 1, in2, size[0], size[1], out); \
}

#define c0c2OpRcc2(in1,in2,size,out)				{ \
 floatComplex __tmp1 = in1 ;\
 ccolumncata(&__tmp1, 1, 1, in2, size[0], size[1], out);\
}

#define z0z2OpRcz2(in1,in2,size,out)				{ \
 doubleComplex  __tmp1 = in1 ;\
 zcolumncata(&__tmp1, 1, 1, in2, size[0], size[1], out);\
}

/* Different type */
#define s0c2OpRcc2(in1,in2,size,out)				c0c2OpRcc2(FloatComplex(in1,0),in2,size,out)

#define d0z2OpRcz2(in1,in2,size,out)				z0z2OpRcz2(DoubleComplex(in1,0),in2,size,out)

#define c0s2OpRcc2(in1,in2,size,out)				sfilla((float*)out,size[0],size[1],0); \
										c0c2OpRcc2(in1,FloatComplexMatrix(in2,(float*)out,size[0]*size[1]),size,out)

#define z0d2OpRcz2(in1,in2,size,out)				dfilla((double*)out,size[0],size[1],0);  \
										z0z2OpRcz2(in1,DoubleComplexMatrix(in2,(double*)out,size[0]*size[1]),size,out)

#endif /* !__INT_OPRC_H__ */
