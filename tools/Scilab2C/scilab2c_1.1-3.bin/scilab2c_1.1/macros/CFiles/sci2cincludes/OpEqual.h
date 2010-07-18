/*
**  -*- C -*-
**
** OpEqual.h
** Made by  Raffaele Nutricato
**
**
*/

#ifndef __OPEQUAL_H__
#define __OPEQUAL_H__

#include <string.h>
#include "floatComplex.h"
#include "doubleComplex.h"

/*
** Compute Opequal for different types .
*/

float sOpEquals1(float x);
double dOpEquals1(double x);
floatComplex c0OpEqualc0(floatComplex x);
doubleComplex z0OpEqualz0(doubleComplex x);
char g0OpEqualg0(char x);

void sOpEquala1(float* x, int size, float* y);
void dOpEquala1(double* x, int size, double* y);
void c2OpEqualc2(floatComplex* x, int* xSize, floatComplex* y);
void z2OpEqualz2(doubleComplex* x, int* xSize, doubleComplex* y);
void g2OpEqualg2(char* x, int* xSize, char* y);

#endif /* !__OPEQUAL_H__ */
