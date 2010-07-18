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
#include <stdio.h>

#define SOURCE {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,\
		    21,22,23,24,25,26,27,28,29,30}


static void drowfftshiftaTest(void){
	int i=0;	
	
	double in[]=SOURCE;
	double result5_6[30]={4,5,1,2,3,9,10,6,7,8,14,15,11,12,13,19,20,16,17,18,24,25,21,22,23,29,30,26,27,28};
	double result6_5[30]={4,5,6,1,2,3,10,11,12,7,8,9,16,17,18,13,14,15,22,23,24,19,20,21,28,29,30,25,26,27};
	double out[30];
	
	drowfftshifta(in,5,6,out);
	for (i=0;i<30;i++)	assert( out[i]-result5_6[i]==0);
	
	drowfftshifta(in,6,5,out);	
	for (i=0;i<30;i++)	assert( out[i]-result6_5[i]==0);
	
}

static void zrowfftshiftaTest(void){
	int i=0;
		
	double inR[]=SOURCE;
	double result5_6[30]={4,5,1,2,3,9,10,6,7,8,14,15,11,12,13,19,20,16,17,18,24,25,21,22,23,29,30,26,27,28};
	double result6_5[30]={4,5,6,1,2,3,10,11,12,7,8,9,16,17,18,13,14,15,22,23,24,19,20,21,28,29,30,25,26,27};
	doubleComplex out[30];
	double ZEROS[30]={0};
	doubleComplex *in;
	
	in=DoubleComplexMatrix(inR,ZEROS,30);
	
	zrowfftshifta(in,5,6,out);
	for (i=0;i<30;i++)	{
		assert( zreals(out[i])-result5_6[i]==0);		
		assert( zimags(out[i])==0);
	}
	
	zrowfftshifta(in,6,5,out);	
	for (i=0;i<30;i++)	{
		assert( zreals(out[i])-result6_5[i]==0);		
		assert( zimags(out[i])==0);
	}
}


static void dcolumnfftshiftaTest(void){
	int i=0;	
	
	double in[]=SOURCE;
	double result5_6[30]={16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	double result6_5[30]={19,20,21,22,23,24,25,26,27,28,29,30,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
	double out[30];
	
	dcolumnfftshifta(in,5,6,out);
	for (i=0;i<30;i++)	assert( out[i]-result5_6[i]==0);
	
	dcolumnfftshifta(in,6,5,out);	
	for (i=0;i<30;i++)	assert( out[i]-result6_5[i]==0);
	
}

static void zcolumnfftshiftaTest(void){
	int i=0;
		
	double inR[]=SOURCE;
	double result5_6[30]={16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	double result6_5[30]={19,20,21,22,23,24,25,26,27,28,29,30,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
	doubleComplex out[30];
	double ZEROS[30]={0};
	doubleComplex *in;
	
	in=DoubleComplexMatrix(inR,ZEROS,30);
	
	zcolumnfftshifta(in,5,6,out);
	for (i=0;i<30;i++)	{
		assert( zreals(out[i])-result5_6[i]==0);		
		assert( zimags(out[i])==0);
	}
	
	zcolumnfftshifta(in,6,5,out);	
	for (i=0;i<30;i++)	{
		assert( zreals(out[i])-result6_5[i]==0);		
		assert( zimags(out[i])==0);
	}
	
}

static void dfttshiftaTest(void){
	int i=0;	
	
	double in[]=SOURCE;
	double result5_6[30]={19,20,16,17,18,24,25,21,22,23,29,30,26,27,28,4,5,1,2,3,9,10,6,7,8,14,15,11,12,13};
	double result6_5[30]={22,23,24,19,20,21,28,29,30,25,26,27,4,5,6,1,2,3,10,11,12,7,8,9,16,17,18,13,14,15};
	double out[30];
	
	dfftshifta(in,5,6,out);
	for (i=0;i<30;i++)	assert( out[i]-result5_6[i]==0);
	
	dfftshifta(in,6,5,out);	
	for (i=0;i<30;i++)	assert( out[i]-result6_5[i]==0);
	
}

static void zfftshiftaTest(void){
	int i=0;
		
	double inR[]=SOURCE;
 	double result5_6[30]={19,20,16,17,18,24,25,21,22,23,29,30,26,27,28,4,5,1,2,3,9,10,6,7,8,14,15,11,12,13};
	double result6_5[30]={22,23,24,19,20,21,28,29,30,25,26,27,4,5,6,1,2,3,10,11,12,7,8,9,16,17,18,13,14,15};
	doubleComplex out[30];
	double ZEROS[30]={0};
	doubleComplex *in;
	
	in=DoubleComplexMatrix(inR,ZEROS,30);
	
	zfftshifta(in,5,6,out);
	for (i=0;i<30;i++)	{
		assert( zreals(out[i])-result5_6[i]==0);		
		assert( zimags(out[i])==0);
	}
	
	zfftshifta(in,6,5,out);	
	for (i=0;i<30;i++)	{
		assert( zreals(out[i])-result6_5[i]==0);		
		assert( zimags(out[i])==0);
	}
	
}

static int FftShiftTest(void){
	drowfftshiftaTest();
	zrowfftshiftaTest();
	dcolumnfftshiftaTest();
	zcolumnfftshiftaTest();
	dfttshiftaTest();
	zfftshiftaTest();
	return 0;
}


int main(void){
	assert (FftShiftTest()==0);
	return 0;
}
