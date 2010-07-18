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

#include "testType.h"
#include "type.h"

int stypesTest() {
  float in = 3.14f;

  /* Only to avoid gcc unused variable */
  ++in;

  assert(stypes(in) == 1);
  return 0;
}

int dtypesTest() {
  double in = 3.14;

  /* Only to avoid gcc unused variable */
  ++in;

  assert(dtypes(in) == 1);
  return 0;
}

int ctypesTest() {
  floatComplex in = FloatComplex(3.14f, 3.14f);

  /* Only to avoid gcc unused variable */
  creals(in);

  assert(ctypes(in) == 1);
  return 0;
}

int ztypesTest() {
  doubleComplex in = DoubleComplex(3.14, 3.14);

  /* Only to avoid gcc unused variable */
  zreals(in);

  assert(ztypes(in) == 1);
  return 0;
}

int stypeaTest() {
  float goodArray[5] = {0.0f,2.0f,3.0f,5.0f,10.0f};
  float badArray[5] = {0.0f,0.0f,0.0f,0.0f,0.0f};

   /* Only to avoid gcc unused variable */
  goodArray[4] = 11.0f;
  badArray[4] = 0.0f;

 printf(">> Floats \n");
  assert(stypea(goodArray, 5) == 1);
  assert(stypea(badArray, 5) == 1);
  return 0;
}

int dtypeaTest() {
  double goodArray[5] = {0.,2.,3.,5.,10.};
  double badArray[5] = {0.,0.,0.,0.,0.};

   /* Only to avoid gcc unused variable */
  goodArray[4] = 11.0;
  badArray[4] = 0.0;

  printf(">> Doubles \n");
  assert(dtypea(goodArray, 5) == 1);
  assert(dtypea(badArray, 5) == 1);
  return 0;
}

int ctypeaTest() {
  floatComplex goodArray[5];
  floatComplex badArray[5];

  /* Good values in goodArray */
  goodArray[0] = FloatComplex(0.0f, 0.0f);
  goodArray[1] = FloatComplex(0.0f, 2.0f);
  goodArray[2] = FloatComplex(3.0f, 50.0f);
  goodArray[3] = FloatComplex(5.0f, 10.0f);
  goodArray[4] = FloatComplex(10.0f, -10.0f);
  /* Bad values in badArray */
  badArray[5] = FloatComplex(0.0f, 0.0f);
  badArray[5] = FloatComplex(0.0f, 0.0f);
  badArray[5] = FloatComplex(0.0f, 0.0f);
  badArray[5] = FloatComplex(0.0f, 0.0f);
  badArray[5] = FloatComplex(0.0f, 0.0f);

  printf(">> Float Complex \n");
  assert(ctypea(goodArray, 5) == 1.0f);
  assert(ctypea(badArray, 5) == 1.0f);
  return 0;
}

int ztypeaTest() {
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
  assert(ztypea(goodArray, 5) == 1);
  assert(ztypea(goodArray, 5) == 1);
  return 0;
}

int testType() {
  int stypesStatus, dtypesStatus = 0;
  int ctypesStatus, ztypesStatus = 0;
  int stypeaStatus, dtypeaStatus = 0;
  int ctypeaStatus, ztypeaStatus = 0;

  printf(">>>> Type Tests\n");
  stypesStatus = stypesTest();
  dtypesStatus = dtypesTest();
  ctypesStatus = ctypesTest();
  ztypesStatus = ztypesTest();
  stypeaStatus = stypeaTest();
  dtypeaStatus = dtypeaTest();
  ctypeaStatus = ctypeaTest();
  ztypeaStatus = ztypeaTest();

  return (stypesStatus + dtypesStatus +
	  ctypesStatus + ztypesStatus +
	  stypeaStatus + dtypeaStatus +
	  ctypeaStatus + ztypeaStatus);
}

int main(void) {
  assert(testType() == 0);
  return 0;
}
