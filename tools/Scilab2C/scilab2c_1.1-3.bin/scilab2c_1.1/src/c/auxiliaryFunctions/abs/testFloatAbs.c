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

#include "testAbs.h"

int sabssTest() {
  float value1 = -123456.789f;
  float value2 = 987654.321f;
  printf(">> Floats \n");
  assert(sabss(value1) == -value1);
  assert(sabss(value2) == value2);
  assert(sabss(0) == 0);
  assert(sabss(-1) == 1);
  assert(sabss(1) == 1);
  return 0;
}

int cabssTest() {
  floatComplex value1 = FloatComplex(4.0f,3.0f);
  printf(">> Float Complex \n");
  assert(cabss(value1) == 5.0f);
  return 0;
}

int sabsaTest() {
  float value1 = -123456.789f;
  float value2 = 987654.321f;
  float value3 = 0.0f;
  float in[3];
  float out[3];

  in[0] = value1;
  in[1] = value2;
  in[2] = value3;

  printf(">> Floats Array\n");
  sabsa(in, 3, out);
  assert(out[0] == -value1);
  assert(out[1] == value2);
  assert(out[2] == value3);
  assert(out[2] == 0.0f);
  return 0;
}

int cabsaTest() {
  floatComplex in[4];
  float out[4];

  in[0] = FloatComplex(4.0f,3.0f);
  in[1] = FloatComplex(-4.0f,3.0f);
  in[2] = FloatComplex(4.0f,-3.0f);
  in[3] = FloatComplex(-4.0f,-3.0f);

  printf(">> Float Complex Array\n");
  cabsa(in, 4, out);
  assert(out[0] == 5.0f);
  assert(out[1] == 5.0f);
  assert(out[2] == 5.0f);
  assert(out[3] == 5.0f);
  return 0;
}

int testAbs() {
  int sabssStatus = 0;
  int cabssStatus = 0;
  int sabsaStatus = 0;
  int cabsaStatus = 0;

  printf(">>>> Float Abs Tests\n");
  sabssStatus = sabssTest();
  cabssStatus = cabssTest();
  sabsaStatus = sabsaTest();
  cabsaStatus = cabsaTest();

  return (sabssStatus +
	  cabssStatus +
	  sabsaStatus +
	  cabsaStatus);
}

int main(void) {
  assert(testAbs() == 0);
  return 0;
}
