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

#ifndef __INT_FIND_H__
#define __INT_FIND_H__

/* 1 input, 1 output */

#define s0finds0(in)				 		(in == 0) ? -1 : 1
			
#define d0findd0(in)						(in == 0) ? -1 : 1
	
#define c0finds0(in)						((creals(in) == 0) && (cimags(in) == 0)) ? -1 : 1

#define z0findd0(in)						((zreals(in) == 0) && (zimags(in) == 0)) ? -1 : 1

#define s2finds2(in,size,out,sizeOut)			sfinda(in, size[0]*size[1], out, sizeOut,-1)

#define d2findd2(in,size,out,sizeOut)			dfinda(in, size[0]*size[1], out, sizeOut,-1)

#define c2finds2(in,size,out,sizeOut)			cfinda(in, size[0]*size[1], out, sizeOut,-1)

#define z2findd2(in,size,out,sizeOut)			zfinda(in, size[0]*size[1], out, sizeOut,-1)

/* 1 input, 2 outputs */

#define s0finds0s0(in,out2)					*out2 = s0finds0(in);s0finds0(in)
	
#define d0findd0d0(in,out2)					*out2 = d0findd0(in);d0findd0(in)

#define c0finds0s0(in,out2)					if ((creals(in)==0) && (cimags(in)==0)) {out2=0;} else {out2=1;}

#define z0findd0d0(in,out2)					if ((zreals(in)==0) && (zimags(in)==0)) {out2=0;} else {out2=1;}


#define s2finds2s2(in,size,out1,sizeOut1,out2,sizeOut2)			sfind2da(in,size[0],size[1],out1,sizeOut1,out2,sizeOut2,-1)

#define d2findd2d2(in,size,out1,sizeOut1,out2,sizeOut2)			dfind2da(in,size[0],size[1],out1,sizeOut1,out2,sizeOut2,-1)

#define c2finds2s2(in,size,out1,sizeOut1,out2,sizeOut2)			cfind2da(in,size[0],size[1],out1,sizeOut1,out2,sizeOut2,-1)

#define z2finds2s2(in,size,out1,sizeOut1,out2,sizeOut2)			zfind2da(in,size[0],size[1],out1,sizeOut1,out2,sizeOut2,-1)

/* 2 inputs, 1 output */

#define s0s0finds0(in1,in2)					s0finds0(in1)

#define d0d0findd0(in1,in2)					d0findd0(in1)

#define c0s0finds0(in1,in2)					c0finds0(in1)

#define z0d0findd0(in1,in2)					z0findd0(in1)
	
#define s2s0finds2(in1,size,in2,out)			{\
 int temp_out_indice[2] = {0} ;\
 sfinda(in1,size[0]*size[1],out,temp_out_indice,in2);\
 }

#define d2d0findd2(in1,size,in2,out)			{\
 int temp_out_indice[2] = {0} ;\
 dfinda(in1,size[0]*size[1],out,temp_out_indice,in2);\
 }

#define c2s0finds2(in1,size,in2,out)			{\
 int temp_out_indice[2] = {0} ;\
 cfinda(in1,size[0]*size[1],out,temp_out_indice,in2);\
 }

#define z2d0findd2(in1,size,in2,out)			{\
 int temp_out_indice[2] = {0} ;\
 zfinda(in1,size[0]*size[1],out,temp_out_indice,in2);\
 }

/* 2 inputs, 2 outputs */			

#define s0s0finds0s0(in1,in2,out2)				s0finds0s0(in1,out2)			

#define d0d0findd0d0(in1,in2,out2)				d0findd0d0(in1,out2)			

#define c0s0finds0s0(in1,in2,out2)				c0finds0s0(in1,out2)			

#define z0d0findd0d0(in1,in2,out2)				z0findd0d0(in1,out2)


/*FIXME : prototypes are wrong*/
#define s2s0finds2s2(in1,size,in2,out1,out2)		{\
 int temp_out_indice1[2] = {0} ;\
 int temp_out_indice2[2] = {0} ;\
 sfind2da(in1,size[0],size[1],out1,temp_out_indice1,out2,temp_out_indice2,in2);\
 }

#define d2d0findd2d2(in1,size,in2,out1,out2)		{\
 int temp_out_indice1[2] = {0} ;\
 int temp_out_indice2[2] = {0} ;\
 dfind2da(in1,size[0],size[1],out1,temp_out_indice1,out2,temp_out_indice2,in2);\
 }
		

#define c2s0finds2s2(in1,size,in2,out1,out2)		{\
 int temp_out_indice1[2] = {0} ;\
 int temp_out_indice2[2] = {0} ;\
 cfind2da(in1,size[0],size[1],out1,temp_out_indice1,out2,temp_out_indice2,in2);\
 }		

#define z2d0findd2d2(in1,size,in2,out1,out2)		{\
 int temp_out_indice1[2] = {0} ;\
 int temp_out_indice2[2] = {0} ;\
 zfind2da(in1,size[0],size[1],out1,temp_out_indice1,out2,temp_out_indice2,in2);\
 }		



#endif /* !__INT_FIND_H__ */

