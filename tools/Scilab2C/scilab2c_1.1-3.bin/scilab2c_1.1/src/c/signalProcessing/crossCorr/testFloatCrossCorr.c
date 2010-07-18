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

 
static void scrossCorraTest(void){
	int i;
	float in1[] = {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,10.0f,11.0f,12.0f};
	float in2[] = {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,10.0f,11.0f,12.0f,13.0f,14.0f,15.0f,16.0f};

	float out[42];
	
	scrossCorra(in1,4,3,in2,4,4,out);
	for (i=0;i<42;i++){
		printf("out[%d] : %f\n",i,out[i]);
	}
}

/*FIXME : aucun test pour les complexes*/
static void ccrossCorraTest(void){

}


static int crossCorraTest(void){
	scrossCorraTest();
	ccrossCorraTest();
	return 0;
}

int main (void){
	assert(crossCorraTest()==0);
	return 0;
}

