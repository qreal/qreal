/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Arnaud Torset
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* THIS IS AN AUTOMATICALLY GENERATED FILE : DO NOT EDIT BY HAND. */

#ifndef __INT_IMAG_H__
#define __INT_IMAG_H__

#define s0imags0(in)			0

#define d0imagd0(in)			0

#define c0imags0(in)			cimags(in)

#define z0imagd0(in)			zimags(in)

#define s2imags2(in,size,out)		szerosa(out,size[0],size[1])

#define d2imagd2(in,size,out)		dzerosa(out,size[0],size[1])

#define c2imags2(in,size,out)		cimaga(in, size[0]*size[1], out)

#define z2imagd2(in,size,out)		zimaga(in, size[0]*size[1], out)

#endif /* !__INT_IMAG_H__ */
