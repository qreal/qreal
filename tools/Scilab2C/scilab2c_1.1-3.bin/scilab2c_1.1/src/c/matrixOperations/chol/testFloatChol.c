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
 
 #include <math.h>
 #include <assert.h>
 #include <stdio.h>
 #include "chol.h"

static void scholsTest(void){
	float in=4;
	float out;
		printf("\n >>> SCholS <<<\n");
	out=schols(in);
	printf("result : %f\n",out);

}

static void scholaTest(void){
	float in[9]= {0.722725308034569025040f,0.8976796260103583335877f,0.2427821881137788295746f,\
0.4337721113115549087524f,0.9677053210325539112091f,0.5068534435704350471497f,\
0.523297640960663557053f,0.5596947595477104187012f,0.5617307000793516635895f};
	float res[9]={0.8501325238070644996213f,0.0f,0.0f,\
0.5102405791617476982225f,0.8410468907315681308390f,0.0f,\
0.6155483131232661886401f,0.2920372626834314977451f,0.3123222878611475739064f};
	float out[9];
	int size=3;
	int i;
			printf("\n >>> SCholA <<<\n");
	schola(in,size,out);
	for (i=0;i<9;i++){ 
		if (out[i]>1e-16)  assert( (fabs(out[i]-res[i]) / fabs(out[i])) <1e-6);
	}
}


static void ccholsTest(void){
	floatComplex in;
	floatComplex out;
	in=FloatComplex(3,1);
		printf("\n >>> CCholS <<<\n");
	out=cchols(in);
	printf("result : %f\n",creals(out));

}







static void ccholaTest(void){
	float inR[9]= {9.0f,4.0f,2.0f,4.0f,5.0f,1.0f,2.0f,1.0f,4.0f};
	float inI[9]= {0.0f,-1.0f,2.0f,-1.0f,0.0f,1.0f,2.0f,1.0f,0.0f};
	float resR[9]= {3.0f,0.0f,0.0f,1.333333333333333259319f,\
	1.763834207376393736766f,0.0f,0.6666666666666666296592f,\
	0.1889822365046136265487f,1.7525491637693282331867f};
	float resI[9]= {0.0f,0.0f,0.0f,- 0.3333333333333333148296f,\
	0.0f,0.0f,0.6666666666666666296592f,- 0.0629940788348712366052f,0.0f};
	floatComplex *in, out[9];
	int i;
	
		printf("\n >>> CCholA <<<\n");	
	in=FloatComplexMatrix(inR,inI,9);
	
	cchola(in,3,out);
	for (i=0;i<9;i++) printf("indice : %d   out : %f+%f *i\n",i,creals(out[i]),cimags(out[i]));
	for (i=0;i<9;i++){
	if (creals(out[i])>1e-16)
			assert( (fabs(creals(out[i])-resR[i]) / fabs(creals(out[i]))) <1e-8);
	if (cimags(out[i])>1e-16)
			assert( (fabs(cimags(out[i])-resI[i]) / fabs(cimags(out[i]))) <1e-8);
	}
}
static int cholTest(void){
	printf("\n >>> Float Chol <<<\n");
	scholsTest();
	scholaTest();
	ccholsTest();
	ccholaTest();
	return 0;
}


int main (void){
	assert(cholTest()==0);
	return 0;
}	
