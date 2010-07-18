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

#include "testLength.h"

int slengthaTest() {
  float goodArray[5] = {0.,2.,3.,5.,10.};
  float badArray[5] = {0.,0.,0.,0.,0.};

  /* Only to avoid gss unused variable warning */
  goodArray[4] = 10.0f;
  badArray[4] = 0.0f;

  printf(">> Floats \n");
  assert(slengtha(goodArray, 5) == 5);
  assert(slengtha(badArray, 5) == 5);
  return 0;
}

int dlengthaTest() {
  double goodArray[5] = {0.,2.,3.,5.,10.};
  double badArray[5] = {0.,0.,0.,0.,0.};

  /* Only to avoid gss unused variable warning */
  goodArray[4] = 10.0;
  badArray[4] = 0.0;

  printf(">> Doubles \n");
  assert(dlengtha(goodArray, 5) == 5);
  assert(dlengtha(badArray, 5) == 5);
  return 0;
}

int clengthaTest() {
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
  assert(clengtha(goodArray, 5) == 5);
  assert(clengtha(badArray, 5) == 5);
  return 0;
}

int zlengthaTest() {
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
  assert(zlengtha(goodArray, 5) == 5);
  assert(zlengtha(goodArray, 5) == 5);
  return 0;
}

int testLength() {
  int slengthaStatus, dlengthaStatus = 0;
  int clengthaStatus, zlengthaStatus = 0;

  printf(">>>> Length Tests\n");
  slengthaStatus = slengthaTest();
  dlengthaStatus = dlengthaTest();
  clengthaStatus = clengthaTest();
  zlengthaStatus = zlengthaTest();

  return (slengthaStatus + dlengthaStatus +
	  clengthaStatus + zlengthaStatus);
}

int main(void) {
  assert(testLength() == 0);
  return 0;
}
