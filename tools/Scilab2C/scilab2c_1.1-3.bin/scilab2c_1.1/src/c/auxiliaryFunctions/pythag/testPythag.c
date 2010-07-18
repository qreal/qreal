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

#include "testPythag.h"

int spythagsTest() {
  float value1 = -3;
  float value2 = 4;

  printf(">> Floats \n");
  assert(spythags(value1, value2) == 5);
  assert(spythags(0, 0) == 0);
  assert(spythags(-3, 0) == 3);
  assert(spythags(3, 0) == 3);
  return 0;
}

int dpythagsTest() {
  double value1 = -3;
  double value2 = 4;

  printf(">> Doubles \n");
  assert(dpythags(value1, value2) == 5);
  assert(dpythags(0, 0) == 0);
  assert(dpythags(-3, 0) == 3);
  assert(dpythags(3, 0) == 3);
  return 0;
}

int testPythag() {
  int spythagsStatus, dpythagsStatus = 0;

  printf(">>>> Pythag Tests\n");
  spythagsStatus = spythagsTest();
  dpythagsStatus = dpythagsTest();

  return (spythagsStatus + dpythagsStatus);
}

int main(void) {
  assert(testPythag() == 0);
  return 0;
}
