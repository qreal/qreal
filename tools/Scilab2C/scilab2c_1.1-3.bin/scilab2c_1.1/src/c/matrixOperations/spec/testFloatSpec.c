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
 
#include <stdlib.h>
#include "spec.h" 
#include "stdio.h"
#include "assert.h"
#include "math.h"

static void sspecaTest(void){
	float in[4]={1.0f,1.0f,1.0f,3.0f};
	float resultR[2]={0.5857864376269050765700f,3.4142135623730949234300f};

	float in1[4]={0.0f,4.0f,1.0f,0.0f};
	float result[2]={2.0f,-2.0f};
	
	float in2[4]={1.0f,1.0f,-2.0f,3.0f};
	
	float *out;
	
	int i;
	
	out=(float *)malloc((unsigned int)2*sizeof(float));
		
	sspeca(in,2,out);
	for (i=0;i<2;i++)	printf("%f\n",out[i]);
	for(i=0;i<2;i++){
		if (out[i]>1e-16)  assert( fabs(out[i]-resultR[i]) / fabs(out[i]) <1e-16);
		else assert(1);
	}	
	
			
	sspeca(in1,2,out);
	for (i=0;i<2;i++)	printf("%f\n",out[i]);
	for(i=0;i<2;i++){
		if (out[i]>1e-16)  assert( fabs(out[i]-result[i]) / fabs(out[i]) <1e-16);
		else assert(1);
	}	
	
	sspeca(in2,2,out);
	for (i=0;i<2;i++)	printf("%f\n",out[i]);

}

	
static void cspecaTest(void){
	float inR[4]={1.0f,1.0f,1.0f,3.0f};	
	float inI[4]={0.0f,0.0f,0.0f,0.0f};
	float resultR[2]={0.5857864376269050765700f,3.4142135623730949234300f};
	float resultI[2]={0,0};
	
	float in2R[4]={1.0f,1.0f,-2.0f,3.0f};
	float in2I[4]={0.0f,0.0f,0.0f,0.0f};
	float result2R[2]={1.9999999999999997779554f,1.9999999999999997779554f};
	float result2I[2]={0.9999999999999997779554f,-0.9999999999999997779554f};
	
	floatComplex *in,*in2,out[4];
	
	int i;
	
	in=FloatComplexMatrix(inR,inI,4);
	in2=FloatComplexMatrix(in2R,in2I,4);
	
	cspeca(in,2,out);
	for(i=0;i<2;i++){
		if (creals(out[i])>1e-16)  assert( fabs(creals(out[i])-resultR[i]) / fabs(creals(out[i])) <1e-15);
		else assert(1);
		if (cimags(out[i])>1e-16)  assert( fabs(cimags(out[i])-resultI[i]) / fabs(cimags(out[i])) <1e-16);
		else assert(1);
	}
	
	
	cspeca(in2,2,out);	
	for(i=0;i<2;i++){
		if (creals(out[i])>1e-16)  assert( fabs(creals(out[i])-result2R[i]) / fabs(creals(out[i])) <1e-15);
		else assert(1);
		if (cimags(out[i])>1e-16)  assert( fabs(cimags(out[i])-result2I[i]) / fabs(cimags(out[i])) <1e-15);
		else assert(1);
	}	
	
}




static int testSpec(void){
	printf(">>> Float Spec Tests <<<\n");
	printf(">>> Float <<<\n");
	sspecaTest();
	printf(">>> FloatComplex <<<\n");
	cspecaTest();
	return 0;
}


int main(void){
	assert (testSpec()==0);

	
	return 0;
}
