/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "testSize.h"

int ssizeaTest() {
  float goodArray[5] = {0.,2.,3.,5.,10.};
  float badArray[5] = {0.,0.,0.,0.,0.};

  /* Only to avoid gss unused variable warning */
  goodArray[4] = 10.0f;
  badArray[4] = 0.0f;

  printf(">> Floats \n");
  assert(ssizea(goodArray, 5) == 5);
  assert(ssizea(badArray, 5) == 5);
  return 0;
}

int dsizeaTest() {
  double goodArray[5] = {0.,2.,3.,5.,10.};
  double badArray[5] = {0.,0.,0.,0.,0.};

  /* Only to avoid gss unused variable warning */
  goodArray[4] = 10.0;
  badArray[4] = 0.0;

  printf(">> Doubles \n");
  assert(dsizea(goodArray, 5) == 5);
  assert(dsizea(badArray, 5) == 5);
  return 0;
}

int csizeaTest() {
  floatComplex goodArray[5];
  floatComplex badArray[5];

  /* Good values in goodArray */
  goodArray[0] = FloatComplex(0., 0.);
  goodArray[1] = FloatComplex(0., 2.);
  goodArray[2] = FloatComplex(3., 50.);
  goodArray[3] = FloatComplex(5., 10.);
  goodArray[4] = FloatComplex(10., -10.);
  /* Bad values in badArray */
  badArray[5] = FloatComplex(0., 0.);
  badArray[5] = FloatComplex(0., 0.);
  badArray[5] = FloatComplex(0., 0.);
  badArray[5] = FloatComplex(0., 0.);
  badArray[5] = FloatComplex(0., 0.);

  printf(">> Float Complex \n");
  assert(csizea(goodArray, 5) == 5);
  assert(csizea(badArray, 5) == 5);
  return 0;
}

int zsizeaTest() {
  doubleComplex goodArray[5];
  doubleComplex badArray[5];

  /* Good values in goodArray. */
  goodArray[0] = DoubleComplex(0., 0.);
  goodArray[1] = DoubleComplex(0., 2.);
  goodArray[2] = DoubleComplex(3., 50.);
  goodArray[3] = DoubleComplex(5., 10.);
  goodArray[4] = DoubleComplex(10., -10.);
  /* Bad values in badArray */
  badArray[0] = DoubleComplex(0., 0.);
  badArray[1] = DoubleComplex(0., 0.);
  badArray[2] = DoubleComplex(0., 0.);
  badArray[3] = DoubleComplex(0., 0.);
  badArray[4] = DoubleComplex(0., 0.);

  printf(">> Double Complex \n");
  assert(zsizea(goodArray, 5) == 5);
  assert(zsizea(goodArray, 5) == 5);
  return 0;
}

int testSize() {
  int ssizeaStatus, dsizeaStatus = 0;
  int csizeaStatus, zsizeaStatus = 0;

  printf(">>>> Size Tests\n");
  ssizeaStatus = ssizeaTest();
  dsizeaStatus = dsizeaTest();
  csizeaStatus = csizeaTest();
  zsizeaStatus = zsizeaTest();

  return (ssizeaStatus + dsizeaStatus +
	  csizeaStatus + zsizeaStatus);
}

int main(void) {
  assert(testSize() == 0);
  return 0;
}
