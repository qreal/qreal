/*
**  -*- C -*-
**
** 
** Made by  Raffaele.Nutricato@tiscali.it
**
** Copyright Raffaele Nutricato
*/

#ifndef __REALTOCOMPLEX_H__
#define __REALTOCOMPLEX_H__

#include "floatComplex.h"
#include "doubleComplex.h"

floatComplex s0floatcomplexc0(float in);
floatComplex d0floatcomplexc0(double in); 
floatComplex c0floatcomplexc0(floatComplex in);
floatComplex z0floatcomplexc0(doubleComplex in);
void s2floatcomplexc2(float* in, int* inSize, floatComplex* out);
void d2floatcomplexc2(double* in, int* inSize, floatComplex* out);
void c2floatcomplexc2(floatComplex* in, int* inSize, floatComplex* out);
void z2floatcomplexc2(doubleComplex* in, int* inSize, floatComplex* out);

doubleComplex s0doublecomplexz0(float in);
doubleComplex d0doublecomplexz0(double in);
doubleComplex c0doublecomplexz0(floatComplex in);
doubleComplex z0doublecomplexz0(doubleComplex in);
void s2doublecomplexz2(float* in, int* inSize, doubleComplex* out);
void d2doublecomplexz2(double* in, int* inSize, doubleComplex* out);
void c2doublecomplexz2(floatComplex* in, int* inSize, doubleComplex* out);
void z2doublecomplexz2(doubleComplex* in, int* inSize, doubleComplex* out);
#endif /* !__REALTOCOMPLEX_H__ */
