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

#include <stdlib.h>
#include "testRand.h"

/*

int drandsTest() {

  double nan = 0.0 / 0.0;
  double result = nan;
  int i=0;
  printf(">> Double\n");
  for (i=0;i<10;i++){
  result = drands();
  printf("%1.20f\n",result);
  }
	
  assert(&result != &nan);

  return 0;
}
*/

/*
int zrandsTest() {

  double nan = 0.0 / 0.0;
  doubleComplex result = DoubleComplex(nan,nan);
  int i=0;
  printf(">> DoubleComplex\n");
  for (i=0;i<10;i++){
  result = zrands();
  printf("%1.20f+%1.20f\n",zreals(result),zimags(result));
  }

  assert(zreals(result) != nan);

  return 0;
}
*/


int drandaTest() {
  int i;
  double dA = 0.0;
  double nan = dA / dA;
  double *result;
  result=(double*)malloc((unsigned int)12*sizeof(double));
  printf(">> Double Array\n");
  dranda(result,12);
  for (i=0;i<12;i++){
  printf("%1.20f\n",result[i]);

  assert(result[i] != nan);}
  free(result);
  return 0;
}
  

/*
int zrandaTest() {
  int i;
  double nan = 0.0 / 0.0;
  doubleComplex *result;
  result=malloc((uint)12*sizeof(doubleComplex));
  printf(">> DoubleComplex Array\n");
  zranda(result,12);
  for (i=0;i<12;i++){
  printf("%1.20f+%1.20f\n",zreals(result[i]),zimags(result[i]));

  assert(zreals(result[i]) != nan);}
  free(result);

  return 0;
}
*/

int testRand() {
/*  int drandsTestStatus = 0;*/
  int zrandaTestStatus = 0;
  printf("\n>>>> Rand Tests\n");
 /* drandsTestStatus = drandsTest();*/
  zrandaTestStatus = drandaTest();
  return (zrandaTestStatus);
}

int main(void) {
  assert(testRand() == 0);
  return 0;
}
