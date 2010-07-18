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

#ifndef __INT_FILL_H__
#define __INT_FILL_H__


#define fills0(in)				in;

#define filld0(in)				in;

#define fillc0(in)				in;

#define fillz0(in)				in;

#define s0fills0(in,out)			out=in;

#define d0filld0(in,out)			out=in;

#define c0fillc0(in,out)			out=in;

#define z0fillz0(in,out)			out=in;

#define s2s0fills2(in1,size,in2,out)		sfilla(out,size[0],size[1],in2);

#define d2d0filld2(in1,size,in2,out)		dfilla(out,size[0],size[1],in2);	

#define c2c0fillc2(in1,size,in2,out)		cfilla(out,size[0],size[1],in2);	

#define z2z0fillz2(in1,size,in2,out)		zfilla(out,size[0],size[1],in2);

#define s0s0s0fills2(in1,in2,in3,out)		sfilla(out,in1,in2,in3)

#define d0d0d0filld2(in1,in2,in3,out)		sfilla(out,in1,in2,in3)

#define s0s0c0fillc2(in1,in2,in3,out)		sfilla(out,in1,in2,in3)

#define d0d0z0fillz2(in1,in2,in3,out)		sfilla(out,in1,in2,in3)

#endif /* !__INT_FILL_H__ */
