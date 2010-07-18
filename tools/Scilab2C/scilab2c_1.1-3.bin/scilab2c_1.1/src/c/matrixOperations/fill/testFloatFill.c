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

#include <stdio.h>
#include <assert.h>
#include "fill.h"

static void sfillaTest(){
	float in[8];
	float constant=2;
	int i;
	sfilla(in, 4, 2, constant);
	for (i=0;i<8;i++)
		printf("indice : %d  --  valeur : %f\n",i,in[i]);
}

static void cfillaTest(){
	floatComplex in[15];
	floatComplex constant=FloatComplex(4,0);
	int i;
	cfilla(in, 3, 5, constant);
	for (i=0;i<15;i++)
		printf("indice : %d  --  valeur : %f+%f *i\n",i,creals(in[i]),cimags(in[i]));
}


static int fillTest(){
	sfillaTest();
	cfillaTest();
	return 0;
}

int main(void){
	assert(fillTest()==0);
	return 0;
}


