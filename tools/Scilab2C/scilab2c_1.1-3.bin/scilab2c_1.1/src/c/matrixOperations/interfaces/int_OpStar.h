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

/* Matrix * Matrix */

#define s2s2OpStars2(in1,size1,in2,size2,out)		smulma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define d2d2OpStard2(in1,size1,in2,size2,out)		dmulma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define c2c2OpStarc2(in1,size1,in2,size2,out)		cmulma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define c2s2OpStarc2(in1,size1,in2,size2,out)		cmulma(in1,size1[0],size1[1],FloatComplexMatrix(in2,0,size2[0]*size2[1]),size2[0],size2[1],out)

#define s2c2OpStarc2(in1,size1,in2,size2,out)		cmulma(FloatComplexMatrix(in1,0,size1[0]*size1[1]),size1[0],size1[1],in2,size2[0],size2[1],out)

#define z2z2OpStarz2(in1,size1,in2,size2,out)		zmulma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define z2d2OpStarc2(in1,size1,in2,size2,out)		cmulma(in1,size1[0],size1[1],DoubleComplexMatrix(in2,0,size2[0],size2[1]),size2[0],size2[1],out)

#define d2z2OpStarc2(in1,size1,in2,size2,out)		cmulma(DoubleComplexMatrix(in1,0,size1[0],size1[1]),size1[0],size1[1],in2,size2[0],size2[1],out)


#endif /* !__INT_OPSTAR_H__ */
