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
 
 
#include "spec.h" 
#include "stdio.h"
#include "assert.h"
#include "math.h"

static void sspec2aTest(void){
	float in[4]={1.0f,1.0f,1.0f,3.0f};
	float resultValuesR[4]={0.5857864376269050765700f,0,0,3.4142135623730949234300f};
	float resultVectorsR[4]={- 0.9238795325112867384831f,0.3826834323650897817792f,
	0.3826834323650897817792f,0.9238795325112867384831f};
	
	float in2[4]={1.0f,1.0f,-2.0f,3.0f};
	float resultValues2R[4]={1.9999999999999997779554f,0,0,1.9999999999999997779554f};
	float resultVectors2R[4]={0.8164965809277261454824f,- 0.4082482904638631282523f,
	0.8164965809277261454824f,- 0.4082482904638631282523f};
	float eigenvalues[4],eigenvectors[4];

	int i;
		
	sspec2a(in,2,eigenvalues,eigenvectors);

	for(i=0;i<4;i++){
		if (eigenvalues[i]>1e-6)  assert( fabs(eigenvalues[i]-resultValuesR[i]) / fabs(eigenvalues[i]) <1e-16);
		else assert(1);
	}
	for(i=0;i<4;i++){
		if (eigenvectors[i]>1e-6)  assert( fabs(eigenvectors[i]-resultVectorsR[i]) / fabs(eigenvectors[i]) <1e-16);
		else assert(1);
	}
	
	
	sspec2a(in2,2,eigenvalues,eigenvectors);
	

	
	for(i=0;i<4;i++){
		if (eigenvalues[i]>1e-6)  assert( fabs(eigenvalues[i]-resultValues2R[i]) / fabs(eigenvalues[i]) <1e-16);
		else assert(1);
	}
	for(i=0;i<4;i++){
		if (eigenvectors[i]>1e-6)  assert( fabs(eigenvectors[i]-resultVectors2R[i]) / fabs(eigenvectors[i]) <1e-16);
		else assert(1);
	}
}



	
static void cspec2aTest(void){
	float inR[4]={1.0f,1.0f,1.0f,3.0f};	
	float inI[4]={0.0f,0.0f,0.0f,0.0f};
	float resultValuesR[4]={0.5857864376269050765700f,0,0,3.4142135623730949234300f};
	float resultValuesI[4]={0,0,0,0};
	float resultVectorsR[4]={- 0.9238795325112867384831f,0.3826834323650897817792f,
	0.3826834323650897817792f,0.9238795325112867384831f};
	float resultVectorsI[4]={0,0,0,0};
	
	float in2R[4]={1.0f,1.0f,-2.0f,3.0f};
	float in2I[4]={0.0f,0.0f,0.0f,0.0f};
	float resultValues2R[4]={1.9999999999999997779554f,0,0,1.9999999999999997779554f};
	float resultValues2I[4]={0.9999999999999997779554f,0,0,-0.9999999999999997779554f};
	float resultVectors2R[4]={0.8164965809277261454824f,- 0.4082482904638631282523f,
	0.8164965809277261454824f,- 0.4082482904638631282523f};
	float resultVectors2I[4]={0,- 0.4082482904638629062077f,0,0.4082482904638629062077f};
	
	floatComplex *in,*in2,out1[4],out2[4];
	
	int i;
	
	in=FloatComplexMatrix(inR,inI,4);
	in2=FloatComplexMatrix(in2R,in2I,4);
	
	cspec2a(in,2,out1,out2);
	for(i=0;i<4;i++){
		if (creals(out1[i])>1e-16)  assert( fabs(creals(out1[i])-resultValuesR[i]) / fabs(creals(out1[i])) <1e-15);
		else assert(1);
		if (cimags(out1[i])>1e-16)  assert( fabs(cimags(out1[i])-resultValuesI[i]) / fabs(cimags(out1[i])) <1e-16);
		else assert(1);
	}
	for(i=0;i<4;i++){
		if (creals(out2[i])>1e-16)  assert( fabs(creals(out2[i])-resultVectorsR[i]) / fabs(creals(out2[i])) <1e-15);
		else assert(1);
		if (cimags(out2[i])>1e-16)  assert( fabs(cimags(out2[i])-resultVectorsI[i]) / fabs(cimags(out2[i])) <1e-16);
		else assert(1);
	}
	
	
	cspec2a(in2,2,out1,out2);	
	for(i=0;i<4;i++){
		if (creals(out1[i])>1e-16)  assert( fabs(creals(out1[i])-resultValues2R[i]) / fabs(creals(out1[i])) <1e-15);
		else assert(1);
		if (cimags(out1[i])>1e-16)  assert( fabs(cimags(out1[i])-resultValues2I[i]) / fabs(cimags(out1[i])) <1e-15);
		else assert(1);
	}	
	for(i=0;i<4;i++){
		if (creals(out2[i])>1e-16)  assert( fabs(creals(out2[i])-resultVectors2R[i]) / fabs(creals(out2[i])) <1e-15);
		else assert(1);
		if (cimags(out2[i])>1e-16)  assert( fabs(cimags(out2[i])-resultVectors2I[i]) / fabs(cimags(out2[i])) <1e-16);
		else assert(1);
	}
}




static int testSpec2(void){
	printf(">>> Float Spec2 Tests <<<\n");
	printf(">>> Float <<<\n");
	sspec2aTest();
	printf(">>> FloatComplex <<<\n");
	cspec2aTest();
	return 0;
}


int main(void){
	assert (testSpec2()==0);

	
	return 0;
}
