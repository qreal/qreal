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

#ifndef __INT_OPBACKSLASH_H__
#define __INT_OPBACKSLASH_H__

/* Matrix \ Matrix */

#define s2s2OpBackSlashs2(in1,size1,in2,size2,out)		sldivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define d2d2OpBackSlashd2(in1,size1,in2,size2,out)		dldivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define c2c2OpBackSlashc2(in1,size1,in2,size2,out)		cldivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define c2s2OpBackSlashc2(in1,size1,in2,size2,out)		cldivma(in1,size1[0],size1[1],FloatComplexMatrix(in2,0,size2[0]*size2[1]),size2[0],size2[1],out)

#define s2c2OpBackSlashc2(in1,size1,in2,size2,out)		cldivma(FloatComplexMatrix(in1,0,size1[0]*size1[1]),size1[0],size1[1],in2,size2[0],size2[1],out)

#define z2z2OpBackSlashz2(in1,size1,in2,size2,out)		zldivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define z2d2OpBackSlashc2(in1,size1,in2,size2,out)		cldivma(in1,size1[0],size1[1],DoubleComplexMatrix(in2,0,size2[0]*size2[1]),size2[0],size2[1],out)

#define d2z2OpBackSlashc2(in1,size1,in2,size2,out)		cldivma(DoubleComplexMatrix(in1,0,size1[0]*size1[1]),size1[0],size1[1],in2,size2[0],size2[1],out)


#endif /* !__INT_OPBACKSLASH_H__ */
