/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "testIsEmpty.h"

int sisemptyaTest() {
  int result = 0;
  float *empty;
  float full[5] = {1., 2., 3., 0., 0.};
  float out;
  
  empty = NULL;
  full[4]=0;
  
  printf(">> Float array\n");
  
  out=sisemptya(empty, 0);
  if (out == 0) {
    printf("ERROR1 ! : Test Failed (empty array)\n");
    result = ERROR;
  }
  assert (out == 1);
  
  
  out = sisemptya(full, 5);
  if ( out == 1) {
    printf("ERROR2 ! : Test Failed (non empty array)\n");
    result = ERROR;
  }
  assert(out == 0);

  return result;
}

int disemptyaTest() {
  int result = 0;
  double *empty;
  double full[5] = {1., 2., 3., 0., 0.};
  double out;
  
  empty = NULL;
  full[4]=0;
  
  
  printf(">> Double array\n");
  out=disemptya(empty, 0);
  if (out == 0) {
    printf("ERROR1 ! : Test Failed (empty array)\n");
    result = ERROR;
  }
  
  assert(out == 1);
  
  
  out=disemptya(full, 5);
  if (out == 1) {
    printf("ERROR2 ! : Test Failed (non empty array)\n");
    result = ERROR;
  }
  assert(out == 0);

  return result;
}

int cisemptyaTest() {
  int result = 0;
  floatComplex *empty;
  floatComplex full[5];
  float out;

  empty=NULL;
  
  full[0] = FloatComplex(0.,1.);
  full[1] = FloatComplex(0., 2.);
  full[2] = FloatComplex(0., 3.);
  full[3] = FloatComplex(0., 0.);
  full[4] = FloatComplex(0., 0.);

  printf(">> Float Complex array\n");
  
  out = cisemptya(empty, 0);
  if (out == 0) {
    printf("ERROR1 ! : Test Failed (empty array)\n");
    result = ERROR;
  }
  assert (out == 1);


  out = cisemptya(full, 5);
  if (out == 1) {
    printf("ERROR2 ! : Test Failed (non empty array)\n");
    result = ERROR;
  }
  assert(out == 0);


  return result;
}

int zisemptyaTest() {
  int result = 0;
  doubleComplex *empty;
  doubleComplex full[5];
  double out;

  empty=NULL;

  full[0] = DoubleComplex(0.,1.);
  full[1] = DoubleComplex(0., 2.);
  full[2] = DoubleComplex(0., 3.);
  full[3] = DoubleComplex(0., 0.);
  full[4] = DoubleComplex(0., 0.);;

  printf(">> Double Complex array\n");
  
  out = zisemptya(empty, 0);
  if (out == 0) {
    printf("ERROR1 ! : Test Failed (empty array)\n");
    result = ERROR;
  }
  assert (out == 1);

  out = zisemptya(full, 5);
  if (out == 1) {
    printf("ERROR2 ! : Test Failed (non empty array)\n");
    result = ERROR;
  }
  assert(out == 0);


  return result;
}


int testIsEmpty() {
  int sisemptyaTestStatus, disemptyaTestStatus = 0;
  int cisemptyaTestStatus, zisemptyaTestStatus = 0;
  printf("\n>>>> IsEmpty Tests\n");
  sisemptyaTestStatus = sisemptyaTest();
  disemptyaTestStatus = disemptyaTest();
  cisemptyaTestStatus = cisemptyaTest();
  zisemptyaTestStatus = zisemptyaTest();
  return (sisemptyaTestStatus + disemptyaTestStatus +
	  cisemptyaTestStatus + zisemptyaTestStatus);
}

int main(void) {
  assert(testIsEmpty() == 0);
  return 0;
}
