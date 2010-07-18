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

#include "testLnp1m1.h"

void slnp1m1sTest(void) {
  float value = 0.0f;
  float maxValue = 1.0f;
  float increment = 1e-3f;
  printf(">> Float scalar\n");
  while (value <= maxValue)
    {
      printf("slnp1m1s(%e) = %e\n", value, slnp1m1s(value));
      value += increment;
    }
}

int testLnp1m1(void) {
  printf("\n>>>> Float Lnp1m1 Tests\n");
  slnp1m1sTest();
  return 0;
}

int main(void) {
  assert(testLnp1m1() == 0);
  return 0;
}
