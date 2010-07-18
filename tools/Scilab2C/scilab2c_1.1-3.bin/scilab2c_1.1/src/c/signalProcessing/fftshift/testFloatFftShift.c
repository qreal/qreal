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


#include "fftshift.h"
#include <assert.h>
#define SOURCE {1,2,3,4,5,6,7,8,9,10,11,12}


static void srowfftshiftaTest(void){
	int i=0;	
	
	float in[]=SOURCE;
	float result3_4[12]={3,1,2,6,4,5,9,7,8,12,10,11};
	float result4_3[12]={3,4,1,2,7,8,5,6,11,12,9,10};
	float out[12];
	
	srowfftshifta(in,3,4,out);
	for (i=0;i<12;i++)	assert( out[i]-result3_4[i]==0);
	
	srowfftshifta(in,4,3,out);	
	for (i=0;i<12;i++)	assert( out[i]-result4_3[i]==0);
	
}

static void crowfftshiftaTest(void){
	int i=0;
		
	float inR[]=SOURCE;
	float result3_4[12]={3,1,2,6,4,5,9,7,8,12,10,11};
	float result4_3[12]={3,4,1,2,7,8,5,6,11,12,9,10};
	floatComplex out[12];
	float ZEROS[12]={0};
	floatComplex *in;
	
	in=FloatComplexMatrix(inR,ZEROS,12);
	
	crowfftshifta(in,3,4,out);
	for (i=0;i<12;i++)	{
		assert( creals(out[i])-result3_4[i]==0);		
		assert( cimags(out[i])==0);
	}
	
	crowfftshifta(in,4,3,out);	
	for (i=0;i<12;i++)	{
		assert( creals(out[i])-result4_3[i]==0);		
		assert( cimags(out[i])==0);
	}
}


static void scolumnfftshiftaTest(void){
	int i=0;	
	
	float in[]=SOURCE;
	float result3_4[12]={7,8,9,10,11,12,1,2,3,4,5,6};
	float result4_3[12]={9,10,11,12,1,2,3,4,5,6,7,8};
	float out[12];
	
	scolumnfftshifta(in,3,4,out);
	for (i=0;i<12;i++)	assert( out[i]-result3_4[i]==0);
	
	scolumnfftshifta(in,4,3,out);	
	for (i=0;i<12;i++)	assert( out[i]-result4_3[i]==0);
	
}

static void ccolumnfftshiftaTest(void){
	int i=0;
		
	float inR[]=SOURCE;
	float result3_4[12]={7,8,9,10,11,12,1,2,3,4,5,6};
	float result4_3[12]={9,10,11,12,1,2,3,4,5,6,7,8};
	floatComplex out[12];
	float ZEROS[12]={0};
	floatComplex *in;
	
	in=FloatComplexMatrix(inR,ZEROS,12);
	
	ccolumnfftshifta(in,3,4,out);
	for (i=0;i<12;i++)	{
		assert( creals(out[i])-result3_4[i]==0);		
		assert( cimags(out[i])==0);
	}
	
	ccolumnfftshifta(in,4,3,out);	
	for (i=0;i<12;i++)	{
		assert( creals(out[i])-result4_3[i]==0);		
		assert( cimags(out[i])==0);
	}
	
}

static void sfftshiftaTest(void){
	int i=0;	
	
	float in[]=SOURCE;
	float result3_4[12]={9,7,8,12,10,11,3,1,2,6,4,5};
	float result4_3[12]={11,12,9,10,3,4,1,2,7,8,5,6};
	float out[12];
	
	sfftshifta(in,3,4,out);
	for (i=0;i<12;i++)	assert( out[i]-result3_4[i]==0);
	
	sfftshifta(in,4,3,out);	
	for (i=0;i<12;i++)	assert( out[i]-result4_3[i]==0);
	
}

static void cfftshiftaTest(void){
	int i=0;
		
	float inR[]=SOURCE;
	float result3_4[12]={9,7,8,12,10,11,3,1,2,6,4,5};
	float result4_3[12]={11,12,9,10,3,4,1,2,7,8,5,6};
	floatComplex out[12];
	float ZEROS[12]={0};
	floatComplex *in;
	
	in=FloatComplexMatrix(inR,ZEROS,12);
	
	cfftshifta(in,3,4,out);
	for (i=0;i<12;i++)	{
		assert( creals(out[i])-result3_4[i]==0);		
		assert( cimags(out[i])==0);
	}
	
	cfftshifta(in,4,3,out);	
	for (i=0;i<12;i++)	{
		assert( creals(out[i])-result4_3[i]==0);		
		assert( cimags(out[i])==0);
	}
	
}

static int FftShiftTest(void){
	srowfftshiftaTest();
	crowfftshiftaTest();
	scolumnfftshiftaTest();
	ccolumnfftshiftaTest();
	sfftshiftaTest();
	cfftshiftaTest();
	return 0;
}


int main(void){
	assert (FftShiftTest()==0);
	return 0;
}
