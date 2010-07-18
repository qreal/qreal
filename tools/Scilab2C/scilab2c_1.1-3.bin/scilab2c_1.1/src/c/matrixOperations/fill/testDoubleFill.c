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

static void dfillaTest(){
	double in[6];
	double constant=7;
	int i;
	dfilla(in, 2,3,constant);
	for (i=0;i<6;i++)
		printf("indice : %d  --  valeur : %f\n",i,in[i]);

}

static void zfillaTest(){
	doubleComplex in[9];
	doubleComplex constant=DoubleComplex(1,1);
	int i;
	
	zfilla(in, 3, 3, constant);
	for (i=0;i<9;i++)
		printf("indice : %d  --  valeur : %f+%f *i\n",i,zreals(in[i]),zimags(in[i]));
}


static int fillTest(){
	dfillaTest();
	zfillaTest();
	return 0;
}

int main(void){
	assert(fillTest()==0);
	return 0;
}


