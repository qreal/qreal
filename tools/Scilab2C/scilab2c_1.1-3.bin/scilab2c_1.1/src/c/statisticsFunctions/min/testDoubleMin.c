/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Arnaud Torset
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */



#include "statMin.h"
#include "assert.h"
#include "stdio.h"


static void dminaTest(void){
	double in[12]={4,5,7,1,2,6,4,1,2,5,4,3};
	double out;
	
	out = dmina(in,12);
	assert(out-1==0);
	
}


static void drowminaTest(void){
	double in[12]={4,5,7,1,2,6,4,1,2,5,4,3};	
	double result2[2]={1,1};
	double result3[3]={1,1,2};
	double result4[4]={4,1,1,3};
	double result6[6]={4,1,2,1,2,3};
	double out1[1],out2[2],out3[3],out4[4],out6[6],out12[12];
	int i;
	
	
	drowmina(in,12,1,out1);
	drowmina(in,6,2,out2);
	drowmina(in,4,3,out3);
	drowmina(in,3,4,out4);
	drowmina(in,2,6,out6);
	drowmina(in,1,12,out12);
	
	assert(out1[0]-1==0);
	for (i=0;i<2;i++) 	assert(out2[i]-result2[i]==0);
	for (i=0;i<3;i++) 	assert(out3[i]-result3[i]==0);
	for (i=0;i<4;i++) 	assert(out4[i]-result4[i]==0);
	for (i=0;i<6;i++) 	assert(out6[i]-result6[i]==0);
	for (i=0;i<12;i++) 	assert(out12[i]-in[i]==0);
}


static void dcolumnminaTest(void){
	double in[12]={4,5,7,1,2,6,4,1,2,5,4,3};
	double out1[1],out2[2],out3[3],out4[4],out6[6],out12[12];
	double result2[2]={2,1};
	double result3[3]={1,1,2};
	double result4[4]={2,5,4,1};
	double result6[6]={4,1,2,1,2,3};
	int i;
	
	
	
	dcolumnmina(in,12,1,out12);
	dcolumnmina(in,6,2,out6);
	dcolumnmina(in,4,3,out4);
	dcolumnmina(in,3,4,out3);
	dcolumnmina(in,2,6,out2);
	dcolumnmina(in,1,12,out1);
	
	assert(out1[0]-1==0);
	for (i=0;i<2;i++) 	assert(out2[i]-result2[i]==0);
	for (i=0;i<3;i++) 	assert(out3[i]-result3[i]==0);
	for (i=0;i<4;i++) 	assert(out4[i]-result4[i]==0);
	for (i=0;i<6;i++) 	assert(out6[i]-result6[i]==0);
	for (i=0;i<12;i++) 	assert(out12[i]-in[i]==0);
}

static int minTest(void){
	dminaTest();
	drowminaTest();
	dcolumnminaTest();
	return 0;
}


int main(void){
	assert(minTest()==0);
	return 0;
}
