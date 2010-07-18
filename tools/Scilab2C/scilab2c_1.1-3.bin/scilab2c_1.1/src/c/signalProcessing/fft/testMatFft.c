
/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "fft.h"

#define test1 {0.2113249000000000099586,0.3303270999999999846253,0.8497451999999999783242,0.0683740000000000042180,\
	0.7560438999999999909463,0.6653810999999999475918,0.6857309999999999794440,0.5608486000000000304411,\
	0.0002211000000000000075,0.6283917999999999448590,0.8782164999999999555058,0.6623569000000000261963}

#define test2 {0.2113249000000000099586,0.6857309999999999794440,\
	0.3303270999999999846253,0.5608486000000000304411,\
	0.8497451999999999783242,0.0002211000000000000075,\
	0.0683740000000000042180,0.6283917999999999448590,\
	0.7560438999999999909463,0.8782164999999999555058,\
	0.6653810999999999475918,0.6623569000000000261963}

#define test3 {0.2113249000000000099586,0.7560438999999999909463,0.0002211000000000000075,\
	0.3303270999999999846253,0.6653810999999999475918,0.6283917999999999448590,\
	0.8497451999999999783242,0.6857309999999999794440,0.8782164999999999555058,\
	0.0683740000000000042180,0.5608486000000000304411,0.6623569000000000261963}
	
#define test4 {0.2113249000000000099586,0.0683740000000000042180,0.6857309999999999794440,0.6283917999999999448590,\
	0.3303270999999999846253,0.7560438999999999909463,0.5608486000000000304411,0.8782164999999999555058,\
	0.8497451999999999783242,0.6653810999999999475918,0.0002211000000000000075,0.6623569000000000261963}
 
#define test6 {0.2113249000000000099586,0.8497451999999999783242,0.7560438999999999909463,0.6857309999999999794440,0.0002211000000000000075,0.8782164999999999555058,\
	0.3303270999999999846253,0.0683740000000000042180,0.6653810999999999475918,0.5608486000000000304411,0.6283917999999999448590,0.6623569000000000261963}

#define test9 {1,2,3,4,5,6,7,8,9}

#define RRESULT1 {6.2969621000000000066166,-0.1908142591446193858662,0.0676336000000001824617,\
-1.4461027999999997994962,-0.7577484999999999359943,0.2136987591446193046618,\
0.4656031000000000474870,0.2136987591446193046618,-0.7577484999999999359943,\
-1.4461027999999997994962,0.0676336000000001824617,-0.1908142591446193858662}

#define IRESULT1 {0,-0.0438615735192828815059,-1.0572926567728160307524,0.3325204999999997745874,\
0.3011445725036896448046,-1.3036713264807171519521,0,1.3036713264807171519521,\
-0.3011445725036896448046,-0.3325204999999997745874,1.0572926567728160307524,\
0.0438615735192828815059}

#define RRESULT2 {6.2969621000000000066166,-0.5345696999999998144659,\
0.0676336000000000714394,-0.3918127000000001247670,\
-0.7577485000000003800835,-1.2843509999999995763176,\
0.4656030999999999364647,1.0404605000000000103455,\
-0.7577485000000003800835,-1.2843509999999995763176,\
0.0676336000000000714394,-0.3918127000000001247670}

#define IRESULT2 {0,0,\
-1.05729265677281625280,0.6392575178288871784460,\
0.3011445725036897003157,-1.043770536118125757952,\
0,0,\
-0.3011445725036897003157,1.043770536118125757952,\
1.05729265677281625280,-0.6392575178288871784460}

#define RRESULT3   {6.2969621000000000066166,-0.9588242499999999335358,-0.9588242499999999335358,\
-1.4461027999999997994962,-0.3545214899631756733101,-0.1146366100368243767793,\
0.4656031000000000474870,0.7607519500000001766438,0.7607519500000001766438,\
-1.4461027999999997994962,-0.1146366100368243767793,-0.3545214899631756733101}

#define IRESULT3 {0,0.4319893196725672623870,-0.4319893196725672623870,\
0.3325204999999997745874,1.0479284784196345459151,-0.5945896784196345041806,\
0,0.5437395057261036335206,-0.5437395057261036335206,\
-0.3325204999999997745874,0.5945896784196345041806,-1.0479284784196345459151}

#define RRESULT4 {6.2969621000000000066166,0.1445965000000000166835,-1.0205663000000000373291,\
 0.1445965000000000166835,\
-0.7577485000000003800835,-0.6754837907274804376101,0.8107183000000001138474,\
 -0.8923310092725194042629,\
-0.7577485000000003800835,-0.8923310092725194042629,0.8107183000000001138474,\
 -0.6754837907274804376101}
 
#define IRESULT4 {0,-0.6791661999999999421718,0,0.6791661999999999421718,\
0.3011445725036897003157,-1.4357905268456061520510,-0.2297677979542606663266,\
 -0.4349033268456063971996,\
-0.3011445725036897003157,0.4349033268456063971996,0.2297677979542606663266,\
 1.4357905268456061520510}

#define RRESULT6 {6.2969621000000000066166,-0.5006002499999999688640,-0.4661336499999999549715,\
 -1.113582300000000024909,-0.4661336499999999549715,-0.5006002499999999688640,\
0.46560309999999915931,0.5234847499999999431708,-0.2239812499999997985611,\
 -1.7786232999999995740836,-0.2239812499999997985611,0.5234847499999999431708}

#define IRESULT6 {0,0.1475342691353691004785,-1.2256565689200087376776,0,1.2256565689200087376776,\
 -0.1475342691353691004785,\
0,1.112275483826065114457,-0.1327806603564972709464,0,0.1327806603564972709464,\
 -1.112275483826065114457}

#define RRESULT9 {45,-4.5,-4.5,-13.5,0,0,-13.5,0,0}

#define IRESULT9 {0,2.598076211353316011810,- 2.598076211353316011810,7.7942286340599471472501,0,0,- 7.7942286340599471472501,0,0}       

static void dfftmaTest(void){
	int i;
	
	double in1[]=test1;
	double in2[]=test2;
	double in3[]=test3;
	double in4[]=test4;
	double in6[]=test6;
	double in9[]=test9;

	double resR1[]=RRESULT1;
	double resR2[]=RRESULT2;
	double resR3[]=RRESULT3;
	double resR4[]=RRESULT4;
	double resR6[]=RRESULT6;
	double resR9[]=RRESULT9;
	
	double out1[12], out2[12], out3[12], out4[12], out6[12], out9[9];


	printf(" >>> Matrice 1*12 <<< \n");
	dfftma(in1, 1, 12, out1);
	for (i=0;i<12;i++){
	printf("\t\n out : %e \t result : %e \t assert: %e",out1[i], resR1[i] ,(fabs(out1[i]-resR1[i]) / fabs(out1[i])) );
	if (out1[i]>1e-16)	assert( (fabs(out1[i]-resR1[i]) / fabs(out1[i])) < 1e-13);
	else			assert(1);
	}

	printf(" >>> Matrice 2*6 <<< \n");
	dfftma(in2, 2, 6, out2);
	for (i=0;i<12;i++){
	printf("\n\t out : %e \t result : %e \t assert: %e\n",out2[i], resR2[i] ,(fabs(out2[i]-resR2[i]) / fabs(out2[i])) );
	if (out2[i]>1e-16)	assert( (fabs(out2[i]-resR2[i]) / fabs(out2[i])) < 1e-13);
	else			assert(1);
	}
	

	printf(" >>> Matrice 3*4 <<< \n");	
	dfftma(in3, 3, 4, out3);
	for (i=0;i<12;i++){	
	if (out3[i]>1e-16)	assert( (fabs(out3[i]-resR3[i]) / fabs(out3[i])) < 1e-13);
	else			assert(1);
	}

	printf(" >>> Matrice 4*3 <<< \n");	
	dfftma(in4, 4, 3, out4);
	for (i=0;i<12;i++){	
	if (out4[i]>1e-16)	assert( (fabs(out4[i]-resR4[i]) / fabs(out4[i])) < 1e-13);
	else			assert(1);
	}

	printf(" >>> Matrice 6*2 <<< \n");	
	dfftma(in6, 6, 2, out6);
	for (i=0;i<12;i++){	
	if (out6[i]>1e-16)	assert( (fabs(out6[i]-resR6[i]) / fabs(out6[i])) < 1e-13);
	else			assert(1);
	}

	printf(" >>> Matrice 3*3 <<< \n");	
	dfftma(in9, 3, 3, out9);
	for (i=0;i<9;i++){
	if (out9[i]>1e-16)	assert( (fabs(out9[i]-resR9[i]) / fabs(out9[i])) < 1e-13);
	else			assert(1);
	}
	

}

static void zfftmaTest(void){
	int i;
	
	double inR1[]=test1;
	double inR2[]=test2;
	double inR3[]=test3;
	double inR4[]=test4;
	double inR6[]=test6;
	double inR9[]=test9;

	double resR1[]=RRESULT1;
	double resI1[]=IRESULT1;
	double resR2[]=RRESULT2;
	double resI2[]=IRESULT2;
	double resR3[]=RRESULT3;
	double resI3[]=IRESULT3;
	double resR4[]=RRESULT4;
	double resI4[]=IRESULT4;
	double resR6[]=RRESULT6;
	double resI6[]=IRESULT6;
	double resR9[]=RRESULT9;
	double resI9[]=IRESULT9;
	
	doubleComplex *in1, *in2, *in3, *in4, *in6, *in9, out1[12], out2[12], out3[12], out4[12], out6[12], out9[12];

	in1=(doubleComplex*)malloc((unsigned int)12*sizeof(doubleComplex));
	in2=(doubleComplex*)malloc((unsigned int)12*sizeof(doubleComplex));
	in3=(doubleComplex*)malloc((unsigned int)12*sizeof(doubleComplex));
	in4=(doubleComplex*)malloc((unsigned int)12*sizeof(doubleComplex));
	in6=(doubleComplex*)malloc((unsigned int)12*sizeof(doubleComplex));
	in9=(doubleComplex*)malloc((unsigned int)9*sizeof(doubleComplex));
	
	
	
	for (i=0;i<12;i++){
		in1[i]=DoubleComplex(inR1[i],0);
		in2[i]=DoubleComplex(inR2[i],0);
		in3[i]=DoubleComplex(inR3[i],0);
		in4[i]=DoubleComplex(inR4[i],0);
		in6[i]=DoubleComplex(inR6[i],0);
	}
	for (i=0;i<9;i++){
		in9[i]=DoubleComplex(inR9[i],0);

	}
	
	/* !!!!!!!!!!!!!!!!!!!!!!!
	   for the imaginary part, the assert is out + res instead of out - res	
	   cause I export the transposate of the result matrix and the transposate change the sign 
	   of the imaginary part.
	   And instead of change all the define, I only change the sign of the assert.*/
	printf(" >>> Matrice 1*12 <<< \n");
	zfftma(in1, 1, 12, out1);
	for (i=0;i<12;i++){
	if (zreals(out1[i])>1e-16)	assert( (fabs(zreals(out1[i])-resR1[i]) / fabs(zreals(out1[i]))) < 1e-13);
	else			assert(1);
	if (zimags(out1[i])>1e-16)	assert( (fabs(zimags(out1[i])+resI1[i]) / fabs(zimags(out1[i]))) < 1e-13);
	else			assert(1);
	}

	printf(" >>> Matrice 2*6 <<< \n");
	zfftma(in2, 2, 6, out2);
	for (i=0;i<12;i++){
	printf("\t\n outR : %e \t result : %e \t assert: %e",zreals(out2[i]), resR2[i] ,(fabs(zreals(out2[i])-resR2[i]) / fabs(zreals(out2[i]))) );
	printf("\t\n outI : %e \t result : %e \t assert: %e",zimags(out2[i]), resI2[i] ,(fabs(zimags(out2[i])-resI2[i]) / fabs(zimags(out2[i]))) );
	if (zreals(out2[i])>1e-16)	assert( (fabs(zreals(out2[i])-resR2[i]) / fabs(zreals(out2[i]))) < 1e-13);
	else			assert(1);
	if (zimags(out2[i])>2e-16)	assert( (fabs(zimags(out2[i])+resI2[i]) / fabs(zimags(out2[i]))) < 1e-13 );
	else			assert(1);
	}
	

	printf(" >>> Matrice 3*4 <<< \n");	
	zfftma(in3, 3, 4, out3);
	for (i=0;i<12;i++){	
	if (zreals(out3[i])>1e-16)	assert( (fabs(zreals(out3[i])-resR3[i]) / fabs(zreals(out3[i]))) < 1e-13);
	else			assert(1);
	if (zimags(out3[i])>1e-16)	assert( (fabs(zimags(out3[i])+resI3[i]) / fabs(zimags(out3[i]))) < 1e-13);
	else			assert(1);
	}

	printf(" >>> Matrice 4*3 <<< \n");	
	zfftma(in4, 4, 3, out4);
	for (i=0;i<12;i++){	
	if (zreals(out4[i])>1e-16)	assert( (fabs(zreals(out4[i])-resR4[i]) / fabs(zreals(out4[i]))) < 1e-13);
	else			assert(1);
	if (zimags(out4[i])>1e-16)	assert( (fabs(zimags(out4[i])+resI4[i]) / fabs(zimags(out4[i]))) < 1e-13);
	else			assert(1);
	}

	printf(" >>> Matrice 6*2 <<< \n");	
	zfftma(in6, 6, 2, out6);
	for (i=0;i<12;i++){	
	if (zreals(out6[i])>1e-16)	assert( (fabs(zreals(out6[i])-resR6[i]) / fabs(zreals(out6[i]))) < 1e-13);
	else			assert(1);
	if (zimags(out6[i])>1e-16)	assert( (fabs(zimags(out6[i])+resI6[i]) / fabs(zimags(out6[i]))) < 1e-13);
	else			assert(1);
	}

	printf(" >>> Matrice 3*3 <<< \n");	
	zfftma(in9, 3, 3, out9);
	for (i=0;i<9;i++){
	if (zreals(out9[i])>1e-16)	assert( (fabs(zreals(out9[i])-resR9[i]) / fabs(zreals(out9[i]))) < 1e-13);
	else			assert(1);
	
	if (zimags(out9[i])>1e-16)	assert( (fabs(zimags(out9[i])-resI9[i]) / fabs(zimags(out9[i]))) < 1e-13);
	else			assert(1);
	}
}



static int testFft(void){
	dfftmaTest();
	zfftmaTest();
	return 0;
}


int main(void) {
  printf(">>> Fft Matrices Double Tests <<<\n");
  assert(testFft() == 0);
  return 0;
}

