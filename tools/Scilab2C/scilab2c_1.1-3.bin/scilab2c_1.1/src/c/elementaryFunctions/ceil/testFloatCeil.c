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
#include "ceil.h"


static void sceilsTest(void) {

	assert ((fabs( sceils(-2) - (-2))/fabs(sceils(-2)))<1e-16);	
	assert ((fabs( sceils(-1.9f) - (-1))/fabs(sceils(-1.9f)))<1e-16);		
	assert ((fabs( sceils(-1.8f) - (-1))/fabs(sceils(-1.8f)))<1e-16);	
	assert ((fabs( sceils(-1.7f) - (-1))/fabs(sceils(-1.7f)))<1e-16);	
	assert ((fabs( sceils(-1.6f) - (-1))/fabs(sceils(-1.6f)))<1e-16);	
	assert ((fabs( sceils(-1.5f) - (-1))/fabs(sceils(-1.5f)))<1e-16);	
	assert ((fabs( sceils(-1.4f) - (-1))/fabs(sceils(-1.4f)))<1e-16);	
	assert ((fabs( sceils(-1.3f) - (-1))/fabs(sceils(-1.3f)))<1e-16);	
	assert ((fabs( sceils(-1.2f) - (-1))/fabs(sceils(-1.2f)))<1e-16);	
	assert ((fabs( sceils(-1.1f) - (-1))/fabs(sceils(-1.1f)))<1e-16);	
	assert ((fabs( sceils(-1.0f) - (-1))/fabs(sceils(-1.0f)))<1e-16);		
	assert (fabs( sceils(-.9f))<1e-16);	
	assert ((fabs( sceils(1.0f) - (1))/fabs(sceils(1.0f)))<1e-16);	
	assert ((fabs( sceils(1.1f) - (2))/fabs(sceils(1.1f)))<1e-16);	
	assert ((fabs( sceils(1.2f) - (2))/fabs(sceils(1.2f)))<1e-16);	
	assert ((fabs( sceils(1.3f) - (2))/fabs(sceils(1.3f)))<1e-16);	
	assert ((fabs( sceils(1.4f) - (2))/fabs(sceils(1.4f)))<1e-16);	
	assert ((fabs( sceils(1.5f) - (2))/fabs(sceils(1.5f)))<1e-16);	
	assert ((fabs( sceils(1.6f) - (2))/fabs(sceils(1.6f)))<1e-16);	
	assert ((fabs( sceils(1.7f) - (2))/fabs(sceils(1.7f)))<1e-16);	
	assert ((fabs( sceils(1.8f) - (2))/fabs(sceils(1.8f)))<1e-16);	
	assert ((fabs( sceils(1.9f) - (2))/fabs(sceils(1.9f)))<1e-16);	
	assert ((fabs( sceils(2.0f) - (2))/fabs(sceils(2.0f)))<1e-16);
}

static void cceilsTest(void) {
	floatComplex in, out;
	
	
	/* tests allant de -2 + i a -1 + 2*i, les reels décroissants de 0.1, les imaginaires croissant de 0.1
	+ 1 test supplementaire : -0.9 + 0.9*i*/
	
	in=FloatComplex(-2,1);
	out = cceils(in);
	assert ((fabs( creals(out) - (-2))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (1))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.9f,1.1f);
	out = cceils(in);	
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);	
	in=FloatComplex(-1.8f,1.2f);
	out = cceils(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.7f,1.3f);
	out = cceils(in);		
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.6f,1.4f);
	out = cceils(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);		
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.5f,1.5f);
	out = cceils(in);	
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);		
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.4f,1.6f);
	out = cceils(in);	
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.3f,1.7f);
	out = cceils(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.2f,1.8f);
	out = cceils(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.1f,1.9f);
	out = cceils(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-1.0f,2.0f);
	out = cceils(in);
	assert ((fabs( creals(out) - (-1))/fabs(creals(out)))<1e-16);	
	assert ((fabs( cimags(out) - (2))/fabs(cimags(out)))<1e-16);
	in=FloatComplex(-.9f,.9f);
	out = cceils(in);	
	assert (fabs( creals(out))<1e-16);			
	assert ((fabs( cimags(out) - (1))/fabs(cimags(out)))<1e-16);
}

static void sceilaTest(void) {
	int i;
	float in[10]={1.2f, 2.0f,-2.0f,-3.1f,-4.5f,6.1f,4.1f,4.9f,-0.7f, 0.5f};
	float res[10]={2,2,-2,-3,-4,7,5,5,0,1};
	float out[10];
	
	sceila(in,10,out);
	
	for (i=0;i<10;i++) 
		if (out[i]!=0) assert(fabs(out[i]-res[i])/fabs(out[i])<1e-16);
		else assert(fabs(out[i]-res[i]) <1e-16);
}

static void cceilaTest(void) {
	int i;
	float inR[10]={1.2f, 2.0f,-2.0f,-3.1f,-4.5f,6.1f,4.1f,4.9f,-0.7f, 0.5f};
	float inI[10]={5.4f,1.8f,1.4f,-4.0f,-2.7f,-0.3f,1.5f,1.4f,7.3f,-4.6f};
	float resR[10]={2,2,-2,-3,-4,7,5,5,0,1};
	float resI[10]={6,2,2,-4,-2,0,2,2,8,-4};
	floatComplex out[10],*in;
	
	in=FloatComplexMatrix(inR,inI,10);
	cceila(in,10,out);
	
	for (i=0;i<10;i++){
	 	if(creals(out[i])!=0)	assert(fabs(creals(out[i])-resR[i])/fabs(creals(out[i]))<1e-16);
		else	assert(fabs(creals(out[i])-resR[i]) <1e-16);
	 	if(cimags(out[i])!=0)	assert(fabs(cimags(out[i])-resI[i])/fabs(cimags(out[i]))<1e-16);
		else assert(fabs(cimags(out[i])-resI[i]) <1e-16);
	 }
}

static int testCeil() {
  printf("\n>>>> Float Ceil Tests\n");
  sceilsTest();
  cceilsTest();
  sceilaTest();
  cceilaTest();

  return 0;
}

int main() {
  assert(testCeil() == 0);
  return 0;
}
