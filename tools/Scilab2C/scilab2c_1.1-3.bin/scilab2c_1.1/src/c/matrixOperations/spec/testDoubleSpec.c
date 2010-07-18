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
 
#include <stdlib.h>
#include "spec.h" 
#include "stdio.h"
#include "assert.h"
#include "math.h"

static void dspecaTest(void){
	double in[4]={1,1,1,3};
	double resultR[2]={0.5857864376269050765700,3.4142135623730949234300};


	double *out;
	
	int i;
	
	out=(double*)malloc((unsigned int)2*sizeof(double));
		
	dspeca(in,2,out);
	for(i=0;i<2;i++){
		if (out[i]>1e-16)  assert( fabs(out[i]-resultR[i]) / fabs(out[i]) <3e-16);
		else assert(1);
	}
	
}



	
static void zspecaTest(void){
	double inR[4]={1,1,1,3};	
	double inI[4]={0,0,0,0};
	double resultR[2]={0.5857864376269050765700,3.4142135623730949234300};
	double resultI[2]={0,0};
	
	double in2R[4]={1,1,-2,3};
	double in2I[4]={0,0,0,0};
	double result2R[2]={1.9999999999999997779554,1.9999999999999997779554};
	double result2I[2]={0.9999999999999997779554,-0.9999999999999997779554};
	
	double in3R[16]={0.0603054538369178771973,0.631347181741148233414 ,0.0241030259057879447937,0.1527438252232968807221,
	0.9481177683919668197632,0.2744265575893223285675,0.4794727200642228126526,0.485576837789267301559,
	0.676400367170572280884,0.8321249918080866336823,0.0125875836238265037537,0.5453779897652566432953,
	0.8426716942340135574341,0.9029165101237595081329,0.4409482078626751899719,0.8332359003834426403046 };
	double in3I[16]={0.7233976423740386962891,0.4377150186337530612946,0.3080607382580637931824,0.8749813153408467769623,
	0.5355882328003644943237 ,0.3085998897440731525421,0.3354632416740059852600,0.2342486302368342876434,
	0.2589411698281764984131,0.8521509231068193912506,0.4821739485487341880798,0.6095217890106141567230,
	0.9872822705656290054321,0.9811426815576851367950,0.3303113700821995735169,0.3589145573787391185761};
	double result3R[4]={1.9486046375762748894545,0.2315060459861970343365,- 0.7694743345806510648188,- 0.2300808535483104266817};
 	double result3I[4]={2.1242015857341254303492,- 0.4115101262891645017561,- 0.1980716835789462781925,0.3584662621795701720195};
 	
	doubleComplex *in,*in2,*in3,out[2],out2[4];
	
	int i;
	
	in=DoubleComplexMatrix(inR,inI,4);
	in2=DoubleComplexMatrix(in2R,in2I,4);
	in3=DoubleComplexMatrix(in3R,in3I,16);
	
	zspeca(in,2,out);
	for(i=0;i<2;i++){
		if (zreals(out[i])>1e-16)  assert( fabs(zreals(out[i])-resultR[i]) / fabs(zreals(out[i])) <3e-16);
		else assert(1);
		if (zimags(out[i])>1e-16)  assert( fabs(zimags(out[i])-resultI[i]) / fabs(zimags(out[i])) <1e-16);
		else assert(1);
	}
	
	
	zspeca(in2,2,out);	
	for(i=0;i<2;i++){
		if (zreals(out[i])>1e-16)  assert( fabs(zreals(out[i])-result2R[i]) / fabs(zreals(out[i])) <3e-16);
		else assert(1);
		if (zimags(out[i])>1e-16)  assert( fabs(zimags(out[i])-result2I[i]) / fabs(zimags(out[i])) <3e-15);
		else assert(1);
	}	
	
	
	zspeca(in3,4,out2);	
	for(i=0;i<4;i++){
		if (zreals(out2[i])>1e-16)  assert( fabs(zreals(out2[i])-result3R[i]) / fabs(zreals(out2[i])) <3e-15);
		else assert(1);
		if (zimags(out2[i])>1e-16)  assert( fabs(zimags(out2[i])-result3I[i]) / fabs(zimags(out2[i])) <3e-15);
		else assert(1);
	}	
	
	
}


static int testSpec(void){
	printf(">>> Double Spec Tests <<<\n");
	printf(">>> Double <<<\n");
	dspecaTest();
	printf(">>> DoubleComplex <<<\n");
	zspecaTest();
	return 0;
}


int main(void){
	assert (testSpec()==0);

	
	return 0;
}
