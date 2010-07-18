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
#include "crossCorr.h"
#include <stdio.h>

 
static void dcrossCorraTest(void){
	int i;
	double in1[] = {1,2,3,4,5,6,7,8,9,10,11,12};
	double in2[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	double inV1[]={0.8812593049369752407074,0.459206754341721534729,0.4192749080248177051544,0.9930617930367588996887,0.1705299648456275463104,0.8114501461386680603027, 0.4106854074634611606598};
    	double inV2[]={0.5211019767448306083679, 0.2967169224284589290619, 0.4054284896701574325562, 0.6229536165483295917511};
	double out[42],outV1[10],outV[12*16];
	
	/* Test Matrice-Matrice */
	dcrossCorra(in1,4,3,in2,4,4,out);
	for (i=0;i<42;i++){
		printf("out[%d] : %f\n",i,out[i]);
	}

	/* Test VecteuLigne-VecteurColonne */	
	dcrossCorra(in1,12,1,in2,1,16,outV);
	for (i=0;i<12*16;i++){
		printf("out[%d] : %f\n",i,outV[i]);
	}

	/* Test VecteurLigne-VecteurLigne */	
    	dcrossCorra(inV1,1,7,inV2,1,4,outV1);
	for (i=0;i<10;i++){
		printf("out[%d] : %f\n",i,outV1[i]);
	}
}

/*FIXME : aucun test pour les complexes*/
static void zcrossCorraTest(void){

}


static int crossCorraTest(void){
	dcrossCorraTest();
	zcrossCorraTest();
	return 0;
}

int main (void){
	assert(crossCorraTest()==0);
	return 0;
}

