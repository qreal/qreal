/*
**  -*- C -*-
**
** 
** Made by  Raffaele.Nutricato@tiscali.it
**
** Copyright Raffaele Nutricato
*/

#ifndef __OPLOGAND_H__
#define __OPLOGAND_H__

#include "floatComplex.h"
#include "doubleComplex.h"

#define s0s0OpLogAnds0(in1,in2) \
 (float)  (in1 && in2)
void s2s0OpLogAnds2(float* in1, int* in1Size, float in2, float* out);

#define d0d0OpLogAndd0(in1,in2) \
 (double)  (in1 && in2)
void d2d0OpLogAndd2(double* in1, int* in1Size, double in2, double* out);
#endif /* !__OPLOGAND_H__ */
