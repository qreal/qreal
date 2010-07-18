
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



#include "matrixSquaredMagnitude.h"
#include <assert.h>
#include <stdio.h>

static void ssquMagnsTest(void){
}


static void ssquMagnaTest(void){
}


static void csquMagnsTest(void){
}

static void csquMagnaTest(void){
}


static int squMagnTest(void){
	printf("\n >>> Squared Magnitude Float Tests<<<\n");
	ssquMagnsTest();
	ssquMagnaTest();
	csquMagnsTest();
	csquMagnaTest();
	return 0;
}

int main (void){
	assert(squMagnTest()==0);
	return 0;
}





