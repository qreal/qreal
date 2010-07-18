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

int dabssTest() {
  double value1 = -123456.789;
  double value2 = 987654.321;
  printf(">> Doubles \n");
  assert(dabss(value1) == -value1);
  assert(dabss(value2) == value2);
  assert(dabss(0) == 0);
  assert(dabss(-1) == 1);
  assert(dabss(1) == 1);
  return 0;
}

int zabssTest() {
  doubleComplex value1 = DoubleComplex(4,3);
  printf(">> Double Complex \n");
  assert(zabss(value1) == 5);
  return 0;
}

int dabsaTest() {
  double value1 = -123456.789;
  double value2 = 987654.321;
  double value3 = 0;
  double in[3];
  double out[3];

  in[0] = value1;
  in[1] = value2;
  in[2] = value3;

  printf(">> Doubles Array\n");
  dabsa(in, 3, out);
  assert(out[0] == -value1);
  assert(out[1] == value2);
  assert(out[2] == value3);
  assert(out[2] == 0);
  return 0;
}

int zabsaTest() {
  doubleComplex in[4];
  double out[4];

  in[0] = DoubleComplex(4,3);
  in[1] = DoubleComplex(-4,3);
  in[2] = DoubleComplex(4,-3);
  in[3] = DoubleComplex(-4,-3);

  printf(">> Double Complex Array\n");
  zabsa(in, 4, out);
  assert(out[0] == 5);
  assert(out[1] == 5);
  assert(out[2] == 5);
  assert(out[3] == 5);
  return 0;
}

int testAbs() {
  int dabssStatus = 0;
  int zabssStatus = 0;
  int dabsaStatus = 0;
  int zabsaStatus = 0;

  printf(">>>> Abs Tests\n");
  dabssStatus = dabssTest();
  zabssStatus = zabssTest();
  dabsaStatus = dabsaTest();
  zabsaStatus = zabsaTest();

  return (dabssStatus +
	  zabssStatus+
	  dabsaStatus +
	  zabsaStatus);
}

int main(void) {
  assert(testAbs() == 0);
  return 0;
}
