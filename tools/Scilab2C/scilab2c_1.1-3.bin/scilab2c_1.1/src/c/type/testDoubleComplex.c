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
#include "doubleComplex.h"

#define size 10000

static int matrixCreation(void) {
  double real[size];
  double imag[size];

  doubleComplex *Z;
  double extractedReal[size];
  double extractedImag[size];

  int i = 0;

   for (i = 0; i < size; ++i)
    {
      real[i] = i;
      imag[i] = size - i;
    }

  Z = DoubleComplexMatrix(real, imag, size);

  for (i = 0; i < size; ++i)
    {
      printf("Partie reelle = %f\n", zreals(Z[i]));
      assert(zreals(Z[i]) == i);
      printf("Partie imaginaire = %f\n", zimags(Z[i]));
      assert(zimags(Z[i]) == size - i);
    }

  zreala(Z, size, extractedReal);
  zimaga(Z, size, extractedImag);
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
