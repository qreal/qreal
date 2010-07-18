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

#ifndef __INT_LEVIN_H__
#define __INT_LEVIN_H__

/* FIXME : many malloc here */
		
#define s0s2levins2(in1,in2,size,out1,out2)		{float* out2;\
									float* out3;\
									out2 = malloc((uint)*(size[1]*size[1]*in1)*sizeof(float));\
									out3 = malloc((uint)*(size[1]*size[1]*in1*(in1+1))*sizeof(float));\
									slevina (in1, in2, size[0], size[1], out1, out2, out3);\
									free(out3);\
									free(out2);\
									}


#define s0s2levins2(in1,in2,size,out1,out2)		{double* out2;\
									double* out3;\
									out2 = malloc((uint)*(size[1]*size[1]*in1)*sizeof(double));\
									out3 = malloc((uint)*(size[1]*size[1]*in1*(in1+1))*sizeof(double));\
									slevina (in1, in2, size[0], size[1], out1, out2, out3);\
									free(out3);\
									free(out2);\
									}



#define s0s2levins2s2(in1,in2,size,out1,out2)		{float* out3;\
									out3 = malloc((uint)*(size[1]*size[1]*in1*(in1+1))*sizeof(float));\
									slevina (in1, in2, size[0], size[1], out1, out2, out3);\
									free(out3);\
									}

#define d0d2levind2d2(in1,in2,size,out1,out2)		{double* out3;\
									out3 = malloc((uint)*(size[1]*size[1]*in1*(in1+1))*sizeof(double));\
									dlevina (in1, in2, size[0], size[1], out1, out2, out3);\
									free(out3);\
									}



#define s0s2levins2s2s2(in1,in2,size,out1,out2,out3)		slevina (in1, in2, size[0], size[1], out1, out2, out3);

#define d0d2levind2d2d2(in1,in2,size,out1,out2,out3)		dlevina (in1, in2, size[0], size[1], out1, out2, out3);


#endif /* !__INT_LEVIN_H__ */
