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
#include "dist.h"

static void ddistsTest(void){
	double in1=2,in2=1;
	printf("%f\n",ddists(in1,in2));
	
}

static void ddistaTest(void){
	double in1[4]={3,4,5,6};
	double in2[4]={0};
	double out;
	double result=9.271109059321277712229;
	
	double in3[6]={1,4,2,5,3,6};
	double in4[6]={0};
	double result2=9.5080320006957226297573;
	
	out=ddista(in1,in2,2,2);
	printf("%f\n",out);
	assert(fabs(out-result) / fabs(out) <1e-16);
	
	
	out=ddista(in3,in4,2,3);
	printf("%f\n",out);
	assert(fabs(out-result2) / fabs(out) <3e-16);
}

static void zdistsTest(void){
	doubleComplex in1,in2;
	double out;
	in1=DoubleComplex(3,4);
	in2=DoubleComplex(0,0);
	
	out=zdists(in1,in2);
	printf("%f\n",out);
	assert(fabs(out-5) / fabs(out) <1e-16);
	
}

static void zdistaTest(void){
	double in1[4]={3,4,5,6};
	double in2[4]={0};
	double out;
	double result=9.271109059321277712229;
	
	double in3[6]={1,4,2,5,3,6};
	double in4[6]={0};
	double result2=9.5080320006957226297573;
	
	
	double in5[15]={1.9379387702792882919312,    6.797837591730058193207,     5.88365742936730384827,      9.331753817386925220490 ,    5.509122880175709724426   ,
    8.0405469564720988273621 ,   1.074489727616310119629  ,   7.4039250845089554786682  ,  5.6103316973894834518433 ,   7.6611549546942114830017 , 
    7.8306589089334011077881 ,   1.4388314681127667427063 ,   1.6471924725919961929321  ,  3.1774142337962985038757  ,  5.0265956297516822814941};
	double in6[15]={6.9204960623756051064    ,   7.0065794046968221664429 ,   8.8706119870766997337341 ,   6.9797694869339466094971   , 6.7989911558106541633606  ,
    3.6159398127347230911255 ,   2.6739976881071925163269   , 0.7736870646476745605469 ,   1.494100340642035007477  ,   3.2018390577286481857300 , 
    2.0260545751079916954041 ,   4.4988587312400341033936  ,  7.7075743721798062324524  ,  2.4352242145687341690063 ,   2.1261148853227496147156};
	double in7[15]={0};
	double result3=28.509738138317576527925;
	doubleComplex *in1Cpx, *in2Cpx, *in3Cpx, *in4Cpx,*in5Cpx,*in6Cpx;
	
	in1Cpx=DoubleComplexMatrix(in1,in2,4);
	in2Cpx=DoubleComplexMatrix(in2,in2,4);
	in3Cpx=DoubleComplexMatrix(in3,in4,6);
	in4Cpx=DoubleComplexMatrix(in4,in4,6);
	in5Cpx=DoubleComplexMatrix(in5,in6,15);
	in6Cpx=DoubleComplexMatrix(in7,in7,15);
	
	
	
	out=zdista(in1Cpx,in2Cpx,2,2);
	printf("%f\n",out);
	assert(fabs(out-result) / fabs(out) <1e-16);
	
	
	out=zdista(in3Cpx,in4Cpx,2,3);
	printf("%f\n",out);
	assert(fabs(out-result2) / fabs(out) <3e-16);
	
	out=zdista(in5Cpx,in6Cpx,5,3);
	printf("%f\n",out);
	assert(fabs(out-result3) / fabs(out) <3e-16);
	
}

static int distTest(void){
	printf("\n>>> Double Dist Tests <<<\n");
	printf(">>> double <<<\n");
	ddistsTest();
	ddistaTest();
	printf(">>> doubleComplex <<<\n");
	zdistsTest();
	zdistaTest();
	return 0;
}

int main(void){
	assert(distTest()==0);
	return 0;
}
