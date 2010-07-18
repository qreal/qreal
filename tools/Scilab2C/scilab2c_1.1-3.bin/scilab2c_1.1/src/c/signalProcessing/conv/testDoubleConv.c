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



static void dconvaTest(void){
	int i;
	double x1[2]={0.5253563085570931434631,0.5376229803077876567841};
	double h1[1]={0.2256303490139544010162};
	double conv1[2]={0.1185363272564196462922,0.1213040606847684693692};
	double out1[2];	

	double x2[8]={0.5878720157779753208160,0.4829179299995303153992,0.2232865034602582454681,\
0.8400885667651891708374,0.1205995907075703144074,0.2855364168062806129456,\
0.8607514644972980022430,0.8494101651012897491455};
	double h2[5]={0.9931209897622466087341,0.6488562873564660549164,0.9923190940171480178833,\
0.0500419777818024158478,0.7485506581142544746399};
	double conv2[12]={0.5838280381629499249385,0.8610403862135279640100,1.1184514745692031301161,\
1.4878174016996970063786,1.3506563468112671166921,1.568122169583095804057,\
1.3689566747706436444787,2.3202981934690010135114,1.5098489496125344633271,\
1.099698103860223152850,0.6868222398318966082087,0.6358265380955081536030};
	double out2[12];
	
	dconva(x1,2,h1,1,out1);
	for (i=0;i<2;i++){
		assert( ( fabs(out1[i]-conv1[i]) / fabs(out1[i]) ) <3e-16);
	}

	dconva(h1,1,x1,2,out1);
	for (i=0;i<2;i++){
		assert( ( fabs(out1[i]-conv1[i]) / fabs(out1[i]) ) <3e-16);
	}

	dconva(x2,8,h2,5,out2);
	for (i=0;i<12;i++){
		assert( ( fabs(out2[i]-conv2[i]) / fabs(out2[i]) ) <3e-15);
	}

	dconva(h2,5,x2,8,out2);
	for (i=0;i<12;i++){
		assert( ( fabs(out2[i]-conv2[i]) / fabs(out2[i]) ) <3e-15);
	}
}


static void zconvaTest(void){
	int i;
	double rx1[2]={0.6274093082174658775330,0.7608432597480714321137};
	double ix1[2]={0.0485566202551126480103,0.6723949727602303028107};
	double rh1[1]={0.2017172696068882942200};
	double ih1[1]={0.3911573919467628002167};
	double rconv1[2]={0.1075660116388346271288,-0.1095370389478208250633};
	double iconv1[2]={0.2552104975446664592020,0.4332431431659207188289};
	doubleComplex *x1, *h1, out1[2];

	double rx2[4]={0.9262343775480985641480,0.5667211269028484821320,0.5711638936772942543030,\
0.8160110483877360820770};
	double ix2[4]={0.0568927936255931854248,0.5595936686731874942780,0.1249340316280722618103,\
0.7279222286306321620941};
	double rh2[7]={0.6783956284634768962860,0.3320095278322696685791,0.0258709923364222049713,\
0.5174468224868178367615,0.3916873238049447536469,0.2413538414984941482544,\
0.5064434898085892200470};
	double ih2[7]={0.4236102001741528511047,0.2893727528862655162811,0.0887932181358337402344,\
0.6212881752289831638336,0.3454984454438090324402,0.7064867629669606685638,\
0.5211472492665052413940};
	double rconv2[10]={0.6042529849650153384033,0.4384669630868825995762,0.3796883200627478305300,\
0.8076061278620030314812,0.3526828755641281398781,0.3863983221928849465243,\
0.3314166433494881713884,0.113095069738258141578,-0.0931668091195109737690,\
0.0339088159370524999670};
	double iconv2[10]={0.4309581525680090119401,0.9066106897516487128641,0.7602048387189073164194,\
1.7159441161321140612017,1.5157099506355884432907,1.5938830446486993430710,\
2.1768689123487110492761,1.5794692240126795290678,1.1131203493567218831828,\
0.7939133870152889072003};
	doubleComplex *x2, *h2, out2[10];	

	x1=DoubleComplexMatrix(rx1,ix1,2);
	h1=DoubleComplexMatrix(rh1,ih1,1);
	x2=DoubleComplexMatrix(rx2,ix2,4);
	h2=DoubleComplexMatrix(rh2,ih2,7);

	zconva(x1,2,h1,1,out1);
	for (i=0;i<2;i++){
		assert( ( fabs(zreals(out1[i])-rconv1[i]) / fabs(zreals(out1[i])) ) <3e-16);
		assert( ( fabs(zimags(out1[i])-iconv1[i]) / fabs(zimags(out1[i])) ) <3e-16);
	}

	zconva(h1,1,x1,2,out1);
	for (i=0;i<2;i++){
		assert( ( fabs(zreals(out1[i])-rconv1[i]) / fabs(zreals(out1[i])) ) <3e-16);
		assert( ( fabs(zimags(out1[i])-iconv1[i]) / fabs(zimags(out1[i])) ) <3e-16);
	}

	zconva(x2,4,h2,7,out2);
	for (i=0;i<7;i++){
		assert( ( fabs(zreals(out2[i])-rconv2[i]) / fabs(zreals(out2[i])) ) <3e-15);
		assert( ( fabs(zimags(out2[i])-iconv2[i]) / fabs(zimags(out2[i])) ) <3e-15);
	}

	zconva(h2,7,x2,4,out2);
	for (i=0;i<10;i++){
		assert( ( fabs(zreals(out2[i])-rconv2[i]) / fabs(zreals(out2[i])) ) <3e-14);
		assert( ( fabs(zimags(out2[i])-iconv2[i]) / fabs(zimags(out2[i])) ) <3e-15);
	}
}



static int convTest(void){
	printf("\n>>>> Conv Double Tests\n");
	dconvaTest();
	zconvaTest();
	return 0;
}

int main (void){
	assert(convTest()==0);
	return 0;
}



