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

#ifndef __INT_OPEQUAL_H__
#define __INT_OPEQUAL_H__


#define s0OpEquals0(in)					in

#define d0OpEquald0(in)					in

#define c0OpEqualc0(in)					in

#define z0OpEqualz0(in)					in

#define s2OpEquals2(in1,size,out)			{int i=0;	\
    for (i = 0 ; i<size[0] * size[1] ; i++) out[i]=in1[i];		\
  }

#define d2OpEquald2(in1,size,out)			{int i=0;	\
    for (i = 0 ; i<size[0] * size[1] ; i++) out[i]=in1[i];		\
  }

#define g2OpEqualg2(in1,size,out)			{int i=0;	\
    for (i = 0 ; i<size[0] * size[1] ; i++) out[i]=in1[i];		\
  }

#endif /* !__INT_OPEQUAL_H__ */
