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
#ifndef __OPLOGGE_H__
#define __OPLOGGE_H__


#define s0s0OpLogGes0(in1,in2) \
 (float)  (in1 >= in2)
void s2s0OpLogGes2(float* in1, int* in1Size, float in2, float* out);

#define d0d0OpLogGed0(in1,in2) \
 (double)  (in1 >= in2)
void d2d0OpLogGed2(double* in1, int* in1Size, double in2, double* out);

/* we must have size1=size2 */

#define s2s2OpLogGes2(in1,size1,in2,size2,out) {int i;\
								for (i=0;i<size1[0]*size2[1]) out[i] = s0s0OpLogGes0(in1[i],in2[i]);\
								}
#define d2d2OpLogGed2(in1,size1,in2,size2,out) {int i;\
								for (i=0;i<size1[0]*size2[1]) out[i] = d0d0OpLogGed0(in1[i],in2[i]);\
								}	
#endif /* !__OPLOGLE_H__ */
