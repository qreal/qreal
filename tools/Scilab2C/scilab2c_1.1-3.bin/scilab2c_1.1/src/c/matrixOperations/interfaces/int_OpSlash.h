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

#ifndef __INT_OPSLASH_H__
#define __INT_OPSLASH_H__


/* Matrix / Matrix */

#define s2s2OpSlashs2(in1,size1,in2,size2,out)		srdivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define d2d2OpSlashd2(in1,size1,in2,size2,out)		drdivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define c2c2OpSlashc2(in1,size1,in2,size2,out)		crdivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define c2s2OpSlashc2(in1,size1,in2,size2,out)		crdivma(in1,size1[0],size1[1],FloatComplexMatrix(in2,0,size2[0]*size2[1]),size2[0],size2[1],out)

#define s2c2OpSlashc2(in1,size1,in2,size2,out)		crdivma(FloatComplexMatrix(in1,0,size1[0]*size1[1]),size1[0],size1[1],in2,size2[0],size2[1],out)

#define z2z2OpSlashz2(in1,size1,in2,size2,out)		zrdivma(in1,size1[0],size1[1],in2,size2[0],size2[1],out)

#define z2d2OpSlashc2(in1,size1,in2,size2,out)		crdivma(in1,size1[0],size1[1],DoubleComplexMatrix(in2,0,size2[0]*size2[1]),size2[0],size2[1],out)

#define d2z2OpSlashc2(in1,size1,in2,size2,out)		crdivma(DoubleComplexMatrix(in1,0,size1[0]*size1[1]),size1[0],size1[1],in2,size2[0],size2[1],out)


#endif /* !__INT_OPSLASH_H__ */
