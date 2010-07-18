/*
**  -*- C -*-
**
** OpEqual.c
** Made by  Raffaele Nutricato <raffaele.nutricato@tiscali.it>
**
**
** Copyright Raffaele Nutricato 2007
*/

#include "OpEqual.h"

float sOpEquals1(float x) 
{
   return (x);
}

double dOpEquals1(double x) 
{
   return x;
}

floatComplex c0OpEqualc0(floatComplex x) 
{
   return x;
}

doubleComplex z0OpEqualz0(doubleComplex x) 
{
   return x;
}

char g0OpEqualg0(char x) 
{
   return x;
}

void sOpEquala1(float* x, int size, float* y) 
{
   int i = 0;
   for (i = 0; i < size; ++i) 
   {
      y[i] = x[i];
   }
}

void dOpEquala1(double* x, int size, double* y) 
{
   int i = 0;
   for (i = 0; i < size; ++i) 
   {
      y[i] = x[i];
   }
}

void c2OpEqualc2(floatComplex* x, int* xSize, floatComplex* y) 
{
   int i = 0;
   int size;
   size = xSize[0]*xSize[1];
   
   for (i = 0; i < size; ++i) 
   {
      y[i] = x[i];
   }
}

void z2OpEqualz2(doubleComplex* x, int* xSize, doubleComplex* y) 
{
   int i = 0;
   int size;
   size = xSize[0]*xSize[1];
   
   for (i = 0; i < size; ++i) 
   {
      y[i] = x[i];
   }
}

void g2OpEqualg2(char* x, int* xSize, char* y) 
{
   int i = 0;
   int size;
   size = xSize[0]*xSize[1];
   
   for (i = 0; i < size; ++i) 
   {
      y[i] = x[i];
   }
}
