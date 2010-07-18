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

#ifndef __INT_CONJ_H__
#define __INT_CONJ_H__

#define copy(in,size,out)		{int i;for (i=0; i<size[0]*size[1];i++) out[i]=in[i];}

#define s0conjs0(in)			in

#define d0conjd0(in)			in

#define c0conjc0(in)			cconjs(in)

#define z0conjz0(in)			zconjs(in)

#define s2conjs2(in,size,out)		copy(in,size,out)

#define d2conjd2(in,size,out)		copy(in,size,out)

#define c2conjc2(in,size,out)		cconja(in, size[0]*size[1], out)

#define z2conjz2(in,size,out)		zconja(in, size[0]*size[1], out)

#endif /* !__INT_CONJ_H__ */
