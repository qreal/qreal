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
/* ADDED BY NUTRICATO, BUT WAITING FOR THE AUTOMATIC GENERATION OF IT
   AS SHOWN BELOW MANY CASES ARE MISSING. SCALAR RANDOM GENERATOR NOT
   IMPLEMENTED YET, SEE DRANDS.C
*/

#ifndef __INT_SIZE_H__
#define __INT_SIZE_H__

#define s0sizes2(in,out)				out[0]=1.0f;out[1]=1.0f;

#define d0sized2(in,out)				out[0]=1.0;out[1]=1.0;

#define c0sizes2(in,out)				out[0]=1.0f;out[1]=1.0f;

#define z0sized2(in,out)				out[0]=1.0;out[1]=1.0;	

#define s2sizes2(in,size,out)				out[0]=size[0];out[1]=size[1];

#define d2sized2(in,size,out)				out[0]=size[0];out[1]=size[1];

#define c2sizes2(in,size,out)				out[0]=size[0];out[1]=size[1];

#define z2sized2(in,size,out)				out[0]=size[0];out[1]=size[1];

#define s0sizes0s0(in,out1,out2)			out1=1.0f;out2=1.0f;

#define d0sized0d0(in,out1,out2)			out1=1.0;out2=1.0;

#define c0sizes0s0(in,out1,out2)			out1=1.0f;out2=1.0f;

#define z0sized0d0(in,out1,out2)			out1=1.0;out2=1.0;

#define s2sizes0s0(in,size,out1,out2)		out1=size[0];out2=size[1];

#define d2sized0d0(in,size,out1,out2)		out1=size[0];out2=size[1];

#define c2sizes0s0(in,size,out1,out2)		out1=size[0];out2=size[1];

#define z2sized0d0(in,size,out1,out2)		out1=size[0];out2=size[1];

#define s0s0sizes0(in1,in2)				1.0f

#define d0d0sized0(in1,in2)				1.

#define c0s0sizes0(in1,in2)				1.0f

#define z0d0sized0(in1,in2)				1.

#define s2s0sizes0(in1,size,in2)			(in2==1) ? size[0] : size[1]

#define d2d0sized0(in1,size,in2)			(in2==1) ? size[0] : size[1]

#define c2s0sizes0(in1,size,in2)			(in2==1) ? size[0] : size[1]

#define z2d0sized0(in1,size,in2)			(in2==1) ? size[0] : size[1]


#endif /* !__INT_SIZE_H__ */


