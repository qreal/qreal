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

#ifndef __INT_FFTSHIFT_H__
#define __INT_FFTSHIFT_H__

#define s0fftshifts0(in)			sfftshifts(in)

#define d0fftshiftd0(in)			dfftshifts(in)

#define c0fftshiftc0(in)			cfftshifts(in)

#define z0fftshiftz0(in)			zfftshifts(in)

#define s2fftshifts2(in,size,out)		sfftshifta(in, size[0], size[1], out)

#define d2fftshiftd2(in,size,out)		dfftshifta(in, size[0], size[1], out)

#define c2fftshiftc2(in,size,out)		cfftshifta(in, size[0], size[1], out)

#define z2fftshiftz2(in,size,out)		zfftshifta(in, size[0], size[1], out)


#define s0s0fftshifts0(in1,in2)			sfftshifts(in1)

#define d0d0fftshiftd0(in1,in2)			dfftshifts(in1)

#define c0s0fftshiftc0(in1,in2)			cfftshifts(in1)

#define z0d0fftshiftz0(in1,in2)			zfftshifts(in1)

#define s2s0fftshifts2(in,size,in2,out)		(in2==1) ? srowfftshifta(in, size[0], size[1], out) : scolumnfftshifta(in, size[0], size[1], out)

#define d2d0fftshiftd2(in,size,in2,out)		(in2==1) ? drowfftshifta(in, size[0], size[1], out) : dcolumnfftshifta(in, size[0], size[1], out)

#define c2s0fftshiftc2(in,size,in2,out)		(in2==1) ? crowfftshifta(in, size[0], size[1], out) : ccolumnfftshifta(in, size[0], size[1], out)

#define z2d0fftshiftz2(in,size,in2,out)		(in2==1) ? zrowfftshifta(in, size[0], size[1], out) : zcolumnfftshifta(in, size[0], size[1], out)

#endif /* !__INT_FFTSHIFT_H__ */
