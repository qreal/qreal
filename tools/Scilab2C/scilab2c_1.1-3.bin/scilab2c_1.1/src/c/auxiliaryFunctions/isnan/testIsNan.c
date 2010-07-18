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

/* FIXME : Must improve isnan rules and remove define. */
#define NDEBUG

#include "testIsNan.h"

int sisnansTest() {
  int result = 0;
  float fA=0.0f;
  float fB=0.0f;
  float nan = fA / fB;
  float full = 1.23456789f;

  printf(">> Float \n");
  if (sisnans(nan) == false) {
    printf("ERROR ! : Test Failed (Must be nan)\n");
    result = ERROR;
  }
  assert (sisnans(nan) == true);

  if (sisnans(full) == true) {
    printf("ERROR ! : Test Failed (Must not be nan)\n");
    result = ERROR;
  }
  assert(sisnans(full) == false);

  return result;
}

int disnansTest() {
  int result = 0;
  double dA = 0.;
  double dB = 0.;
  double nan = dA / dB;
  double full = 1.456789321861;

  printf(">> Double\n");
  if (disnans(nan) == false) {
    printf("ERROR ! : Test Failed (Must be nan)\n");
    result = ERROR;
  }
  assert(disnans(nan) == true);

  if (disnans(full) == true) {
    printf("ERROR ! : Test Failed (non nan array)\n");
    result = ERROR;
  }
  assert(disnans(full) == false);

  return result;
}

int cisnansTest() {
  int result = 0;
  float fA = 0.;
  float fB = 0.;
  floatComplex nan_nan = FloatComplex(fA/fB, fA/fB);
  floatComplex nan_real = FloatComplex(fA/fB, 1.123456789f);
  floatComplex real_nan = FloatComplex(1.123456789f, fA/fB);
  floatComplex full = FloatComplex(0.0f,1.0f);

  printf(">> Float Complex\n");
  if (cisnans(nan_nan) == false) {
    printf("ERROR ! : Test Failed (Must be nan)\n");
    result = ERROR;
  }
  assert (cisnans(nan_nan) == true);

 if (cisnans(nan_real) == true) {
    printf("ERROR ! : Test Failed (Must not be nan)\n");
    result = ERROR;
  }
  assert(cisnans(nan_real) == false);

 if (cisnans(real_nan) == true) {
    printf("ERROR ! : Test Failed (Must not be nan)\n");
    result = ERROR;
  }
  assert(cisnans(real_nan) == false);

  if (cisnans(full) == true) {
    printf("ERROR ! : Test Failed (Must not be nan)\n");
    result = ERROR;
  }
  assert(cisnans(full) == false);


  return result;
}

int zisnansTest() {
  int result = 0;
  float fA = 0.;
  float fB = 0.;
  floatComplex nan_nan = FloatComplex(fA/fB, fA/fB);
  floatComplex nan_real = FloatComplex(fA/fB, 1.123456789f);
  floatComplex real_nan = FloatComplex(1.123456789f , fA/fB);
  floatComplex full = FloatComplex(0.0f,1.0f);

  printf(">> Double Complex\n");
  if (cisnans(nan_nan) == false) {
    printf("ERROR ! : Test Failed (Must be nan)\n");
    result = ERROR;
  }
  assert (cisnans(nan_nan) == true);

 if (cisnans(nan_real) == true) {
    printf("ERROR ! : Test Failed (Must not be nan)\n");
    result = ERROR;
  }
  assert(cisnans(nan_real) == false);

 if (cisnans(real_nan) == true) {
    printf("ERROR ! : Test Failed (Must not be nan)\n");
    result = ERROR;
  }
  assert(cisnans(real_nan) == false);

  if (cisnans(full) == true) {
    printf("ERROR ! : Test Failed (Must not be nan)\n");
    result = ERROR;
  }
  assert(cisnans(full) == false);


  return result;
}


int testIsNan() {
  int sisnansTestStatus, disnansTestStatus = 0;
  int cisnansTestStatus, zisnansTestStatus = 0;
  printf("\n>>>> IsNan Tests\n");
  sisnansTestStatus = sisnansTest();
  disnansTestStatus = disnansTest();
  cisnansTestStatus = cisnansTest();
  zisnansTestStatus = zisnansTest();

  return (sisnansTestStatus + disnansTestStatus +
	  cisnansTestStatus + zisnansTestStatus);
}

int main(void) {
  assert(testIsNan() == 0);
  return 0;
}
