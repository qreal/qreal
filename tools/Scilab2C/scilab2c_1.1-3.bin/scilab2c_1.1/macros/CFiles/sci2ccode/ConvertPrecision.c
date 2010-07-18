/*
**  -*- C -*-
**
** ConvertPrecision.c
** Made by  Raffaele Nutricato <raffaele.nutricato@tiscali.it>
**
** Copyright Raffaele Nutricato 2008
*/


double s0doubled0(float in)
{
   double out;
   out = (double) in;
   return (out);
}

void s2doubled2(float* in, int* inSize, double* out)
{
   int i;
   for (i=0; i<inSize[0]*inSize[1]; i++)
   {
      out[i] = (double) in[i];
   }
}

float d0floats0(double in)
{
   float out;
   out = (float) in;
   return (out);
}

void d2floats2(double* in, int* inSize, float* out)
{
   int i;
   for (i=0; i<inSize[0]*inSize[1]; i++)
   {
      out[i] = (float) in[i];
   }
}
