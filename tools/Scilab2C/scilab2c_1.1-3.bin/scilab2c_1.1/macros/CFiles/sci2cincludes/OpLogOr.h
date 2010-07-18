/*
**  -*- C -*-
**
** 
** Made by  Raffaele.Nutricato@tiscali.it
**
** Copyright Raffaele Nutricato
*/

#ifndef __OPLOGOR_H__
#define __OPLOGOR_H__

#include "floatComplex.h"
#include "doubleComplex.h"

#define s0s0OpLogOrs0(in1,in2) \
 (float)  (in1 || in2)
void s2s0OpLogOrs2(float* in1, int* in1Size, float in2, float* out);

#define d0d0OpLogOrd0(in1,in2) \
 (double)  (in1 || in2)
void d2d0OpLogOrd2(double* in1, int* in1Size, double in2, double* out);
#endif /* !__OPLOGOR_H__ */
