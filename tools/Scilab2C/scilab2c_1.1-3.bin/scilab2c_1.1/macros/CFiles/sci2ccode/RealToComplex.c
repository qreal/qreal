/*
**  -*- C -*-
**
** 
** Made by  Raffaele.Nutricato@tiscali.it
**
** Copyright Raffaele Nutricato
*/

#include "RealToComplex.h"

floatComplex s0floatcomplexc0(float in) 
{
  floatComplex out;
  out = FloatComplex(in,0);
  return out;
}

floatComplex d0floatcomplexc0(double in) 
{
  floatComplex out;
  out = FloatComplex(in,0);
  return out;
}

floatComplex c0floatcomplexc0(floatComplex in) 
{
   return in;
}

floatComplex z0floatcomplexc0(doubleComplex in) 
{
  floatComplex out;
  out = FloatComplex((float)zreals(in),(float)zimags(in));
  return out;
}

void s2floatcomplexc2(float* in, int* inSize, floatComplex* out) 
{
   int i = 0;
   for (i=0;i<inSize[0]*inSize[1];i++)
   {
      out[i] = s0floatcomplexc0(in[i]);
   }
}

void d2floatcomplexc2(double* in, int* inSize, floatComplex* out) 
{
   int i = 0;
   for (i=0;i<inSize[0]*inSize[1];i++)
   {
      out[i] = d0floatcomplexc0(in[i]);
   }
}

void c2floatcomplexc2(floatComplex* in, int* inSize, floatComplex* out) 
{
   int i = 0;
   for (i=0;i<inSize[0]*inSize[1];i++)
   {
      out[i] = c0floatcomplexc0(in[i]);
   }
}

void z2floatcomplexc2(doubleComplex* in, int* inSize, floatComplex* out) 
{
   int i = 0;
   for (i=0;i<inSize[0]*inSize[1];i++)
   {
      out[i] = z0floatcomplexc0(in[i]);
   }
}

doubleComplex s0doublecomplexz0(float in) 
{
  doubleComplex out;
  out = DoubleComplex((double)(in),0);
  return out;
}

doubleComplex d0doublecomplexz0(double in) 
{
  doubleComplex out;
  out = DoubleComplex(in,0);
  return out;
}

doubleComplex c0doublecomplexz0(floatComplex in) 
{
  doubleComplex out;
  out = DoubleComplex((double) creals(in),(double) cimags(in));
  return out;
}

doubleComplex z0doublecomplexz0(doubleComplex in) 
{
   return in;
}

void s2doublecomplexz2(float* in, int* inSize, doubleComplex* out) 
{
   int i = 0;
   for (i=0;i<inSize[0]*inSize[1];i++)
   {
      out[i] = s0doublecomplexz0(in[i]);
   }
}

void d2doublecomplexz2(double* in, int* inSize, doubleComplex* out) 
{
   int i = 0;
   for (i=0;i<inSize[0]*inSize[1];i++)
   {
      out[i] = d0doublecomplexz0(in[i]);
   }
}

void c2doublecomplexz2(floatComplex* in, int* inSize, doubleComplex* out) 
{
   int i = 0;
   for (i=0;i<inSize[0]*inSize[1];i++)
   {
      out[i] = c0doublecomplexz0(in[i]);
   }
}

void z2doublecomplexz2(doubleComplex* in, int* inSize, doubleComplex* out) 
{
   int i = 0;
   for (i=0;i<inSize[0]*inSize[1];i++)
   {
      out[i] = z0doublecomplexz0(in[i]);
   }
}
