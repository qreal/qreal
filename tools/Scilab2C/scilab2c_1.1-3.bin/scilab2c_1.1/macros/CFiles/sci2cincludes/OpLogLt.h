/*
**  -*- C -*-
**
** 
** Made by  Raffaele.Nutricato@tiscali.it
**
** Started on  Tue Dec  5 15:49:18 2006 jofret
** Last update Mon Oct 22 10:01:54 2007 bruno
**
** Copyright INRIA 2006
*/

/*
   Update 23/02/09 by Arnaud Torset : Add matrix comparaison, remove include(floatComplex and doubleComplex)
*/
#ifndef __OPLOGLT_H__
#define __OPLOGLT_H__

#define s0s0OpLogLts0(in1,in2) \
 (float)  (in1 < in2)
void s2s0OpLogLts2(float* in1, int* in1Size, float in2, float* out);

#define d0d0OpLogLtd0(in1,in2) \
 (double)  (in1 < in2)
void d2d0OpLogLtd2(double* in1, int* in1Size, double in2, double* out);

/* we must have size1=size2 */

#define s2s2OpLogLts2(in1,size1,in2,size2,out) {int i;\
								for (i=0;i<size1[0]*size2[1]) out[i] = s0s0OpLogLts0(in1[i],in2[i]);\
								}
#define d2d2OpLogLtd2(in1,size1,in2,size2,out) {int i;\
								for (i=0;i<size1[0]*size2[1]) out[i] = d0d0OpLogLtd0(in1[i],in2[i]);\
								}	
#endif /* !__OPLOGLT_H__ */
