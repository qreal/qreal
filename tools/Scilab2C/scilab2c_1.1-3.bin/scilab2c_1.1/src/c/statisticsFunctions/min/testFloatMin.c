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

static void sminaTest(void){
	float in[12]={4,5,7,1,2,6,4,1,2,5,4,3};
	float out;
	
	out = smina(in,12);
	assert(out-1==0);
	
}


static void srowminaTest(void){
	float in[12]={4,5,7,1,2,6,4,1,2,5,4,3};
	float out1[1],out2[2],out3[3],out4[4],out6[6],out12[12];
	float result2[2]={1,1};
	float result3[3]={1,1,2};
	float result4[4]={4,1,1,3};
	float result6[6]={4,1,2,1,2,3};
	int i;
	
	
	srowmina(in,12,1,out1);
	srowmina(in,6,2,out2);
	srowmina(in,4,3,out3);
	srowmina(in,3,4,out4);
	srowmina(in,2,6,out6);
	srowmina(in,1,12,out12);
	
	assert(out1[0]-1==0);
	for (i=0;i<2;i++) 	assert(out2[i]-result2[i]==0);
	for (i=0;i<3;i++) 	assert(out3[i]-result3[i]==0);
	for (i=0;i<4;i++) 	assert(out4[i]-result4[i]==0);
	for (i=0;i<6;i++) 	assert(out6[i]-result6[i]==0);
	for (i=0;i<12;i++) 	assert(out12[i]-in[i]==0);
}


static void scolumnminaTest(void){
	float in[12]={4,5,7,1,2,6,4,1,2,5,4,3};
	float out1[1],out2[2],out3[3],out4[4],out6[6],out12[12];
	float result2[2]={2,1};
	float result3[3]={1,1,2};
	float result4[4]={2,5,4,1};
	float result6[6]={4,1,2,1,2,3};
	int i;
	
	
	
	scolumnmina(in,12,1,out12);
	scolumnmina(in,6,2,out6);
	scolumnmina(in,4,3,out4);
	scolumnmina(in,3,4,out3);
	scolumnmina(in,2,6,out2);
	scolumnmina(in,1,12,out1);
	
	assert(out1[0]-1==0);
	for (i=0;i<2;i++) 	assert(out2[i]-result2[i]==0);
	for (i=0;i<3;i++) 	assert(out3[i]-result3[i]==0);
	for (i=0;i<4;i++) 	assert(out4[i]-result4[i]==0);
	for (i=0;i<6;i++) 	assert(out6[i]-result6[i]==0);
	for (i=0;i<12;i++) 	assert(out12[i]-in[i]==0);
}

static int minTest(void){
	sminaTest();
	srowminaTest();
	scolumnminaTest();
	return 0;
}


int main(void){
	assert(minTest()==0);
	return 0;
}
