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

static void sdistsTest(void){
	float in1=2,in2=1;
	printf("%f\n",sdists(in1,in2));
	
}

static void sdistaTest(void){
	float in1[4]={3,4,5,6};
	float in2[4]={0};
	float out;
	float result=9.271109059321277712229f;
	
	float in3[6]={1,4,2,5,3,6};
	float in4[6]={0};
	float result2=9.5080320006957226297573f;
	
	out=sdista(in1,in2,2,2);
	printf("%f\n",out);
	assert(fabs(out-result) / fabs(out) <1e-16);
	
	
	out=sdista(in3,in4,2,3);
	printf("%f\n",out);
	assert(fabs(out-result2) / fabs(out) <1e-15);
}

static void cdistsTest(void){
	floatComplex in1,in2;
	float out;
	in1=FloatComplex(3,4);
	in2=FloatComplex(0,0);
	
	out=cdists(in1,in2);
	printf("%f\n",out);
	assert(fabs(out-5) / fabs(out) <1e-16);
	
}

static void cdistaTest(void){
	float in1[4]={3,4,5,6};
	float in2[4]={0};
	float out;
	float result=9.271109059321277712229f;
	
	float in3[6]={1,4,2,5,3,6};
	float in4[6]={0};
	float result2=9.5080320006957226297573f;
	
	
	float in5[15]={1.9379387702792882919312f,    6.797837591730058193207f,     5.88365742936730384827f,      9.331753817386925220490f ,    5.509122880175709724426f   ,
    8.0405469564720988273621f ,   1.074489727616310119629f  ,   7.4039250845089554786682f  ,  5.6103316973894834518433f ,   7.6611549546942114830017f , 
    7.8306589089334011077881f ,   1.4388314681127667427063f ,   1.6471924725919961929321f  ,  3.1774142337962985038757f  ,  5.0265956297516822814941f};
	float in6[15]={6.9204960623756051064f    ,   7.0065794046968221664429f ,   8.8706119870766997337341f ,   6.9797694869339466094971f   , 6.7989911558106541633606f  ,
    3.6159398127347230911255f ,   2.6739976881071925163269f   , 0.7736870646476745605469f ,   1.494100340642035007477f  ,   3.2018390577286481857300f , 
    2.0260545751079916954041f ,   4.4988587312400341033936f  ,  7.7075743721798062324524f  ,  2.4352242145687341690063f ,   2.1261148853227496147156f};
	float in7[15]={0};
	float result3=28.509738138317576527925f;
	floatComplex *in1Cpx, *in2Cpx, *in3Cpx, *in4Cpx,*in5Cpx,*in6Cpx;
	
	in1Cpx=FloatComplexMatrix(in1,in2,4);
	in2Cpx=FloatComplexMatrix(in2,in2,4);
	in3Cpx=FloatComplexMatrix(in3,in4,6);
	in4Cpx=FloatComplexMatrix(in4,in4,6);
	in5Cpx=FloatComplexMatrix(in5,in6,15);
	in6Cpx=FloatComplexMatrix(in7,in7,15);
	
	
	
	out=cdista(in1Cpx,in2Cpx,2,2);
	printf("%f\n",out);
	assert(fabs(out-result) / fabs(out) <1e-16);
	
	
	out=cdista(in3Cpx,in4Cpx,2,3);
	printf("%f\n",out);
	assert(fabs(out-result2) / fabs(out) <1e-15);
	
	out=cdista(in5Cpx,in6Cpx,5,3);
	printf("%f\n",out);
	assert(fabs(out-result3) / fabs(out) <1e-15);
	
}

static int distTest(void){
	printf("\n>>> Float Dist Tests <<<\n");
	printf(">>> float <<<\n");
	sdistsTest();
	sdistaTest();
	printf(">>> floatComplex <<<\n");
	cdistsTest();
	cdistaTest();
	return 0;
}

int main(void){
	assert(distTest()==0);
	return 0;
}
