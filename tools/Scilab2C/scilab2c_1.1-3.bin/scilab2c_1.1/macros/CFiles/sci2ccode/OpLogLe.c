/*
**  -*- C -*-
**
** OpDotSlash.c
** Made by  Raffaele Nutricato <raffaele.nutricato@tiscali.it>
**
**
** Copyright Raffaele Nutricato 2007
*/

#include "OpLogLe.h"

void s2s0OpLogLes2(float* in1, int* in1Size, float in2, float* out)
{
   int rows = 0;
   int cols = 0;
   for (rows = 0; rows < in1Size[0];rows++)
   {
      for (cols = 0; cols < in1Size[1];cols++)
      {
         out[rows*in1Size[1]+cols] = (float) (in1[rows*in1Size[1]+cols] <= in2);
      }
   }
}

void d2d0OpLogLed2(double* in1, int* in1Size, double in2, double* out)
{
   int rows = 0;
   int cols = 0;
   for (rows = 0; rows < in1Size[0];rows++)
   {
      for (cols = 0; cols < in1Size[1];cols++)
      {
         out[rows*in1Size[1]+cols] = (double) (in1[rows*in1Size[1]+cols] <= in2);
      }
   }
}
