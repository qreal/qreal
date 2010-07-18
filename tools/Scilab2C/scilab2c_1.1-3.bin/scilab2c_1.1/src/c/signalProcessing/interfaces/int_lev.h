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

#ifndef __INT_LEV_H__
#define __INT_LEV_H__

#define s2levs2(in,size,out)				{float sigma2;\
								sigma2=sleva2(in,size[0]*size[1],out);\
								}	
			
#define s2levs2s0(in,size,out)			sleva2(in,size[0]*size[1],out);
			
#define s2levs2s0s2(in,size,out1,out2)		sleva(in,size[0]*size[1],out1,out2);


#define d2levd2(in,size,out)				{double sigma2;\
								sigma2=dleva2(in,size[0]*size[1],out);\
								}
			
#define d2levd2d0(in,size,out)			dleva2(in,size[0]*size[1],out);
			
#define d2levd2d0d2(in,size,out1,out2)		dleva(in,size[0]*size[1],out1,out2);


#define c2levc2(in,size,out)				{floatComplex sigma2;\
								sigma2=cleva2(in,size[0]*size[1],out);\
								}
			
#define c2levc2c0(in,size,out)			cleva2(in,size[0]*size[1],out);
			
#define c2levc2c0c2(in,size,out1,out2)		cleva(in,size[0]*size[1],out1,out2);


#define z2levz2(in,size,out)				{doubleComplex sigma2;\
								sigma2=zleva2(in,size[0]*size[1],out);\
								}
			
#define z2levz2z0(in,size,out)			zleva2(in,size[0]*size[1],out);
			
#define z2levz2z0z2(in,size,out1,out2)		zleva(in,size[0]*size[1],out1,out2);

#endif /* !__INT_LEV_H__ */
