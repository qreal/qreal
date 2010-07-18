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

#ifndef __INT_ONES_H__
#define __INT_ONES_H__


#define oness0()			1.0f

#define onesd0()			1



#define s0oness0(in)			soness(in)

#define d0onesd0(in)			doness(in)

#define c0onesc0(in)			coness(in)

#define z0onesz0(in)			zoness(in)


#define s0s0oness0(in1,in2)			1.0f

#define d0d0onesd0(in,in2)			1


#define s0s0oness2(in1,in2,out)	sonesa(out, in1, in2)	

#define d0d0onesd2(in1,in2,out)	donesa(out, in1, in2)					


#define s2oness2(in,size,out)		sonesa(out, size[0], size[1])

#define d2onesd2(in,size,out)		donesa(out, size[0], size[1])

#define c2onesc2(in,size,out)		conesa(out, size[0], size[1])

#define z2onesz2(in,size,out)		zonesa(out, size[0], size[1])


#endif /* !__INT_ONES_H__ */
