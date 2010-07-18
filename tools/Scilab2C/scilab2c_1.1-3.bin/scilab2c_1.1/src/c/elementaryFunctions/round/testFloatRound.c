/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Arnaud TORSET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "round.h"

static void sroundsTest(void) {

	assert ((fabs( srounds(-2) - (-2))/fabs(srounds(-2)))<1e-16);	
	assert ((fabs( srounds(-1.9f) - (-2))/fabs(srounds(-1.9f)))<1e-16);		
	assert ((fabs( srounds(-1.8f) - (-2))/fabs(srounds(-1.8f)))<1e-16);	
	assert ((fabs( srounds(-1.7f) - (-2))/fabs(srounds(-1.7f)))<1e-16);	
	assert ((fabs( srounds(-1.6f) - (-2))/fabs(srounds(-1.6f)))<1e-16);	
	assert ((fabs( srounds(-1.5f) - (-2))/fabs(srounds(-1.5f)))<1e-16);	
	assert ((fabs( srounds(-1.4f) - (-1))/fabs(srounds(-1.4f)))<1e-16);	
	assert ((fabs( srounds(-1.3f) - (-1))/fabs(srounds(-1.3f)))<1e-16);	
	assert ((fabs( srounds(-1.2f) - (-1))/fabs(srounds(-1.2f)))<1e-16);	
	assert ((fabs( srounds(-1.1f) - (-1))/fabs(srounds(-1.1f)))<1e-16);	
	assert ((fabs( srounds(-1.0f) - (-1))/fabs(srounds(-1.0f)))<1e-16);	
	assert ((fabs( srounds(-0.9f) - (-1))/fabs(srounds(-0.9f)))<1e-16);
	assert ((fabs( srounds(1.0f) - (1))/fabs(srounds(1.0f)))<1e-16);	
	assert ((fabs( srounds(1.1f) - (1))/fabs(srounds(1.1f)))<1e-16);	
	assert ((fabs( srounds(1.2f) - (1))/fabs(srounds(1.2f)))<1e-16);	
	assert ((fabs( srounds(1.3f) - (1))/fabs(srounds(1.3f)))<1e-16);	
	assert ((fabs( srounds(1.4f) - (1))/fabs(srounds(1.4f)))<1e-16);	
	assert ((fabs( srounds(1.5f) - (2))/fabs(srounds(1.5f)))<1e-16);	
	assert ((fabs( srounds(1.6f) - (2))/fabs(srounds(1.6f)))<1e-16);	
	assert ((fabs( srounds(1.7f) - (2))/fabs(srounds(1.7f)))<1e-16);	
	assert ((fabs( srounds(1.8f) - (2))/fabs(srounds(1.8f)))<1e-16);	
	assert ((fabs( srounds(1.9f) - (2))/fabs(srounds(1.9f)))<1e-16);	
	assert ((fabs( srounds(2.0f) - (2))/fabs(srounds(2.0f)))<1e-16);
}

static void croundsTest(void) {
	floatComplex in, out;
	
	
	/* tests allant de -2 + i a -1 + 2*i, les reels décroissants de 0.1, les imaginaires croissant de 0.1
	+ 1 test supplementaire : -0.9 + 0.9*i*/
	
	in=FloatComplex(-2,1);
	out = crounds(in);
	assert ((fabs( creals(out) - (-2))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (1))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.9f,1.1f);
	out = crounds(in);	
	assert ((fabs( creals(out) - (-2))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (1))/fabs(cimags(out)))<1e-16);	
	in=FloatComplex(-1.8f,1.2f);
	out = crounds(in);
	assert ((fabs( creals(out) - (-2))/fabs(creals(out)))<1e-16);
	assert ((fabs( cimags(out) - (1))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.7f,1.3f);
	out = crounds(in);		
	assert ((fabs( creals(out) - (-2))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (1))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.6f,1.4f);
	out = crounds(in);
	assert ((fabs( creals(out) - (-2))/fabs(creals(out)))<1e-16);		
	assert ((fabs( cimags(out) - (1))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.5f,1.5f);
	out = crounds(in);	
	assert ((fabs( creals(out) - (-2))/fabs(creals(out)))<1e-16);		
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.4f,1.6f);
	out = crounds(in);	
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.3f,1.7f);
	out = crounds(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.2f,1.8f);
	out = crounds(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.1f,1.9f);
	out = crounds(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.0f,2.0f);
	out = crounds(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-.9f,.9f);
	out = crounds(in);	
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);		
	assert ((fabs( cimags(out) - (1))/fabs(cimags(out)))<1e-16);
}

static void sroundaTest(void) {
	int i;
	float in[10]={1.2f, 2.0f,-2.0f,-3.1f,-4.5f,6.1f,4.1f,4.9f,-0.7f, 0.5f};
	float res[10]={1,2,-2,-3,-5,6,4,5,-1,1};
	float out[10];
	
	srounda(in,10,out);
	
	for (i=0;i<10;i++) 
		if (out[i]!=0) assert(fabs(out[i]-res[i])/fabs(out[i]) <1e-16);
		else assert(fabs(out[i]-res[i]) <1e-16);
}

static void croundaTest(void) {
	int i;
	float inR[10]={1.2f, 2.0f,-2.0f,-3.1f,-4.5f,6.1f,4.1f,4.9f,-0.7f, 0.5f};
	float inI[10]={5.4f,1.8f,1.4f,-4.0f,-2.7f,-0.3f,1.5f,1.4f,7.3f,-4.6f};
	float resR[10]={1,2,-2,-3,-5,6,4,5,-1,1};
	float resI[10]={5,2,1,-4,-3,0,2,1,7,-5};
	floatComplex out[10],*in;
	
	in=FloatComplexMatrix(inR,inI,10);
	crounda(in,10,out);
	
	for (i=0;i<10;i++){
		if (creals(out[i])!=0) assert(fabs(creals(out[i])-resR[i])/fabs(creals(out[i])) <1e-16);
		else assert(fabs(creals(out[i])-resR[i]) <1e-16);
		if (cimags(out[i])!=0) assert(fabs(cimags(out[i])-resI[i])/fabs(cimags(out[i])) <1e-16);
	 	else assert(fabs(cimags(out[i])-resI[i]) <1e-16);
	 }
}

static int testRound() {
  printf("\n>>>> Float Round Tests\n");
  sroundsTest();
  croundsTest();
  sroundaTest();
  croundaTest();

  return 0;
}

int main() {
  assert(testRound() == 0);
  return 0;
}
