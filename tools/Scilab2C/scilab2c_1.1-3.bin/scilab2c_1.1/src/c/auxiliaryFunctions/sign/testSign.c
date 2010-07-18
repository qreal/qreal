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

/*
** FIXME : Add some array tests !
*/

#include "testSign.h"

int ssignsTest() {

  float positive = 0.123f;
  float negative = -0.123f;
  float zero = 0.0f;

  printf(">> Float \n");
  assert(ssigns(positive) == 1);
  assert(ssigns(negative) == -1);
  assert(ssigns(zero) == 0);
  return 0;
}

int dsignsTest() {

  double positive = 0.123;
  double negative = -0.123;
  double zero = 0;

  printf(">> Double\n");
  assert(dsigns(positive) == 1);
  assert(dsigns(negative) == -1);
  assert(dsigns(zero) == 0);
  return 0;
}

int csignsTest() {
  floatComplex zero = FloatComplex(0., 0.);
  floatComplex i = FloatComplex(0., 1.);
  floatComplex oneAndI = FloatComplex(1., 1.);
  floatComplex sign_i = csigns(i);
  floatComplex sign_oneAndI =  csigns(oneAndI);
  floatComplex sign_zero = csigns(zero);

  printf(">> Float Complex\n");
  /* sign(%i) = %i */
  assert(creals(sign_i) == 0);
  assert(cimags(sign_i) == 1);
  /* sign(1+%i) = (1+%i) / sqrt(2) */
  assert(creals(sign_oneAndI) == 1 / sqrtf(2));
  assert(cimags(sign_oneAndI) == 1 / sqrtf(2));
  /* sign(0) = 0 */
  assert(creals(sign_zero) == 0);
  assert(cimags(sign_zero) == 0);

  return 0;
}

int zsignsTest() {
  floatComplex zero = FloatComplex(0., 0.);
  doubleComplex i = DoubleComplex(0., 1.);
  doubleComplex oneAndI = DoubleComplex(1., 1.);
  doubleComplex sign_i = zsigns(i);
  doubleComplex sign_oneAndI =  zsigns(oneAndI);
  floatComplex sign_zero = csigns(zero);

  printf(">> Double Complex\n");
  /* sign(%i) = %i */
  assert(zreals(sign_i) == 0);
  assert(zimags(sign_i) == 1);
  /* sign(1+%i) = (1+%i) / sqrt(2) */
  assert ( fabs ( zreals(sign_oneAndI) - (sqrt(2.)/2.) ) / fabs(  zreals(sign_oneAndI)) < 1e-15) ;
  assert ( fabs ( zimags(sign_oneAndI) - 1 / sqrt(2.) ) / fabs(  zimags(sign_oneAndI)) < 1e-15) ;
  /* sign(0) = 0 */
  assert(creals(sign_zero) == 0);
  assert(cimags(sign_zero) == 0);

  return 0;
}


int testSign() {
  int ssignsTestStatus, dsignsTestStatus = 0;
  int csignsTestStatus, zsignsTestStatus = 0;
  printf("\n>>>> Sign Tests\n");
  ssignsTestStatus = ssignsTest();
  dsignsTestStatus = dsignsTest();
  csignsTestStatus = csignsTest();
  zsignsTestStatus = zsignsTest();

  return (ssignsTestStatus + dsignsTestStatus +
	  csignsTestStatus + zsignsTestStatus);
}

int main(void) {
  assert(testSign() == 0);
  return 0;
}
