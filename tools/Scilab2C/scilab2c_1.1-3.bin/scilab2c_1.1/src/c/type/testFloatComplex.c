/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2006-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <assert.h>
#include <float.h>
#include "floatComplex.h"

#define size 10000

static int matrixCreation(void) {
  float real[size];
  float imag[size];

  floatComplex *Z;
  float extractedReal[size];
  float extractedImag[size];

  int i = 0;

  for (i = 0; i < size; ++i)
    {
      real[i] = (float) i;
      imag[i] = (float) (size - i);
    }

  Z = FloatComplexMatrix(real, imag, size);

  for (i = 0; i < size; ++i)
    {
      printf("Partie reelle = %f\n", creals(Z[i]));
      assert(creals(Z[i]) == i);
      printf("Partie imaginaire = %f\n", cimags(Z[i]));
      assert(cimags(Z[i]) == size - i);
    }

  creala(Z, size, extractedReal);
  cimaga(Z, size, extractedImag);
  for (i = 0; i < size; ++i)
    {
      printf("Partie reelle = %f\n", extractedReal[i]);
      assert(extractedReal[i] == i);
      printf("Partie imaginaire = %f\n", extractedImag[i]);
      assert(extractedImag[i] == size - i);
   }

  return 0;
}

int main(void) {
  matrixCreation();
  return 0;
}
