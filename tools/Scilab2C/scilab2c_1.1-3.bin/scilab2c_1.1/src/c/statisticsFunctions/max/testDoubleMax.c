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



#include "statMax.h"
#include "assert.h"
#include "stdio.h"


static void dmaxaTest(void){
	double in[12]={4,5,7,1,2,6,4,1,2,5,4,3};
	double out;
	
	out = dmaxa(in,12);
	assert(out-7==0);
	
}


static void drowmaxaTest(void){
	double in[12]={4,5,7,1,2,6,4,1,2,5,4,3};	
	double result2[2]={7,5};
	double result3[3]={7,6,5};
	double result4[4]={7,6,4,5};
	double result6[6]={5,7,6,4,5,4};
	double out1[1],out2[2],out3[3],out4[4],out6[6],out12[12];
	int i;
	
	
	drowmaxa(in,12,1,out1);
	drowmaxa(in,6,2,out2);
	drowmaxa(in,4,3,out3);
	drowmaxa(in,3,4,out4);
	drowmaxa(in,2,6,out6);
	drowmaxa(in,1,12,out12);
	
	assert(out1[0]-7==0);
	for (i=0;i<2;i++) 	assert(out2[i]-result2[i]==0);
	for (i=0;i<3;i++) 	assert(out3[i]-result3[i]==0);
	for (i=0;i<4;i++) 	assert(out4[i]-result4[i]==0);
	for (i=0;i<6;i++) 	assert(out6[i]-result6[i]==0);
	for (i=0;i<12;i++) 	assert(out12[i]-in[i]==0);
}


static void dcolumnmaxaTest(void){
	double in[12]={4,5,7,1,2,6,4,1,2,5,4,3};
	double out1[1],out2[2],out3[3],out4[4],out6[6],out12[12];
	double result2[2]={7,6};
	double result3[3]={5,5,7};
	double result4[4]={4,6,7,3};
	double result6[6]={4,5,7,5,4,6};
	int i;
	
	
	
	dcolumnmaxa(in,12,1,out12);
	dcolumnmaxa(in,6,2,out6);
	dcolumnmaxa(in,4,3,out4);
	dcolumnmaxa(in,3,4,out3);
	dcolumnmaxa(in,2,6,out2);
	dcolumnmaxa(in,1,12,out1);
	
	assert(out1[0]-7==0);
	for (i=0;i<2;i++) 	assert(out2[i]-result2[i]==0);
	for (i=0;i<3;i++) 	assert(out3[i]-result3[i]==0);
	for (i=0;i<4;i++) 	assert(out4[i]-result4[i]==0);
	for (i=0;i<6;i++) 	assert(out6[i]-result6[i]==0);
	for (i=0;i<12;i++) 	assert(out12[i]-in[i]==0);
}

static int maxTest(void){
	dmaxaTest();
	drowmaxaTest();
	dcolumnmaxaTest();
	return 0;
}


int main(void){
	assert(maxTest()==0);
	return 0;
}
