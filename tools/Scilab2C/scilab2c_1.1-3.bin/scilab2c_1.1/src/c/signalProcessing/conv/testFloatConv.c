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


#include "conv.h"
#include <stdio.h>
#include <assert.h>

static void sconvaTest(void){
	int i;
	float x1[2]={0.5253563085570931434631f,0.5376229803077876567841f};
	float h1[1]={0.2256303490139544010162f};
	float conv1[2]={0.1185363272564196462922f,0.1213040606847684693692f};	
	float out1[2]; 

	float x2[8]={0.5878720157779753208160f,0.4829179299995303153992f,0.2232865034602582454681f,\
0.8400885667651891708374f,0.1205995907075703144074f,0.2855364168062806129456f,\
0.8607514644972980022430f,0.8494101651012897491455f};
	float h2[5]={0.9931209897622466087341f,0.6488562873564660549164f,0.9923190940171480178833f,\
0.0500419777818024158478f,0.7485506581142544746399f};
	float conv2[12]={0.5838280381629499249385f,0.8610403862135279640100f,1.1184514745692031301161f,\
1.4878174016996970063786f,1.3506563468112671166921f,1.568122169583095804057f,\
1.3689566747706436444787f,2.3202981934690010135114f,1.5098489496125344633271f,\
1.099698103860223152850f,0.6868222398318966082087f,0.6358265380955081536030f};	
	float out2[12]; 

	sconva(x1,2,h1,1,out1);
	for (i=0;i<2;i++){
		assert( ( fabs(out1[i]-conv1[i]) / fabs(out1[i]) ) <1e-7);
	}

	sconva(h1,1,x1,2,out1);
	for (i=0;i<2;i++){
		assert( ( fabs(out1[i]-conv1[i]) / fabs(out1[i]) ) <1e-7);
	}


	sconva(x2,8,h2,5,out2);
	for (i=0;i<12;i++){
		assert( ( fabs(out2[i]-conv2[i]) / fabs(out2[i]) ) <1e-6);
	}

	sconva(h2,5,x2,8,out2);
	for (i=0;i<12;i++){
		assert( ( fabs(out2[i]-conv2[i]) / fabs(out2[i]) ) <1e-6);
	}
}


static void cconvaTest(void){
	int i;
	float rx1[2]={0.6274093082174658775330f,0.7608432597480714321137f};
	float ix1[2]={0.0485566202551126480103f,0.6723949727602303028107f};
	float rh1[1]={0.2017172696068882942200f};
	float ih1[1]={0.3911573919467628002167f};
	float rconv1[2]={0.1075660116388346271288f,-0.1095370389478208250633f};
	float iconv1[2]={0.2552104975446664592020f,0.4332431431659207188289f};
	floatComplex *x1, *h1, out1[2];

	float rx2[4]={0.9262343775480985641480f,0.5667211269028484821320f,0.5711638936772942543030f,\
0.8160110483877360820770f};
	float ix2[4]={0.0568927936255931854248f,0.5595936686731874942780f,0.1249340316280722618103f,\
0.7279222286306321620941f};
	float rh2[7]={0.6783956284634768962860f,0.3320095278322696685791f,0.0258709923364222049713f,\
0.5174468224868178367615f,0.3916873238049447536469f,0.2413538414984941482544f,\
0.5064434898085892200470f};
	float ih2[7]={0.4236102001741528511047f,0.2893727528862655162811f,0.0887932181358337402344f,\
0.6212881752289831638336f,0.3454984454438090324402f,0.7064867629669606685638f,\
0.5211472492665052413940f};
	float rconv2[10]={0.6042529849650153384033f,0.4384669630868825995762f,0.3796883200627478305300f,\
0.8076061278620030314812f,0.3526828755641281398781f,0.3863983221928849465243f,\
0.3314166433494881713884f,0.113095069738258141578f,-0.0931668091195109737690f,\
0.0339088159370524999670f};
	float iconv2[10]={0.4309581525680090119401f,0.9066106897516487128641f,0.7602048387189073164194f,\
1.7159441161321140612017f,1.5157099506355884432907f,1.5938830446486993430710f,\
2.1768689123487110492761f,1.5794692240126795290678f,1.1131203493567218831828f,\
0.7939133870152889072003f};
	floatComplex *x2, *h2, out2[10];
	

	x1=FloatComplexMatrix(rx1,ix1,2);
	h1=FloatComplexMatrix(rh1,ih1,1);
	x2=FloatComplexMatrix(rx2,ix2,4);
	h2=FloatComplexMatrix(rh2,ih2,7);


	cconva(x1,2,h1,1,out1);
	for (i=0;i<2;i++){
		assert( ( fabs(creals(out1[i])-rconv1[i]) / fabs(creals(out1[i])) ) <1e-6);
		assert( ( fabs(cimags(out1[i])-iconv1[i]) / fabs(cimags(out1[i])) ) <1e-6);
	}

	cconva(h1,1,x1,2,out1);
	for (i=0;i<2;i++){
		assert( ( fabs(creals(out1[i])-rconv1[i]) / fabs(creals(out1[i])) ) <1e-6);
		assert( ( fabs(cimags(out1[i])-iconv1[i]) / fabs(cimags(out1[i])) ) <1e-6);
	}

	cconva(x2,4,h2,7,out2);
	for (i=0;i<10;i++){
		assert( ( fabs(creals(out2[i])-rconv2[i]) / fabs(creals(out2[i])) ) <3e-6);
		assert( ( fabs(cimags(out2[i])-iconv2[i]) / fabs(cimags(out2[i])) ) <1e-6);
	}

	cconva(h2,7,x2,4,out2);
	for (i=0;i<10;i++){
		assert( ( fabs(creals(out2[i])-rconv2[i]) / fabs(creals(out2[i])) ) <3e-6);
		assert( ( fabs(cimags(out2[i])-iconv2[i]) / fabs(cimags(out2[i])) ) <1e-6);
	}

}



static int convTest(void){
	printf("\n>>>> Conv Float Tests\n");
	sconvaTest();
	cconvaTest();
	return 0;
}

int main (void){
	assert(convTest()==0);
	return 0;
}
