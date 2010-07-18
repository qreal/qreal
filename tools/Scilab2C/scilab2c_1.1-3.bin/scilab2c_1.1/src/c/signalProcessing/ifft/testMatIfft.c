
/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Arnaud TORSET
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
#include "ifft.h"

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

#define RRESULT1 {0.5247468416666665191883,-0.0159011882620516131759,0.0056361333333333485385,\
-0.1205085666666666222024,-0.0631457083333333279995,0.0178082299287182777014,\
0.0388002583333332817794,0.0178082299287182777014,-0.0631457083333333279995,\
-0.1205085666666666222024,0.0056361333333333485385,-0.0159011882620516131759}

#define IRESULT1 {0,0.0036551311266069114181,0.0881077213977346646034,-0.0277100416666666432564,\
-0.0250953810419741324411,0.1086392772067264061997,0,-0.1086392772067264061997,\
0.0250953810419741324411,0.0277100416666666432564,-0.0881077213977346646034,\
-0.0036551311266069114181}

#define RRESULT2 {0.5247468416666666302106,-0.0445474749999999961037,\
0.0056361333333333554774,-0.0326510583333333367917,\
-0.0631457083333333279995,-0.1070292499999999646931,\
0.0388002583333333234128,0.0867050416666666767807,\
-0.0631457083333333279995,-0.1070292499999999646931,\
0.0056361333333333554774,-0.0326510583333333367917}

#define IRESULT2 {0,0,\
0.0881077213977346646034,-0.0532714598190739199723,\
-0.0250953810419741081550,0.0869808780098438039108,\
0,0,\
0.0250953810419741081550,-0.0869808780098438039108,\
-0.0881077213977346646034,0.0532714598190739199723}

#define RRESULT3   {0.5247468416666665191883,-0.0799020208333333092909,-0.0799020208333333092909,\
-0.1205085666666666222024,-0.0295434574969313107351,-0.0095530508364020244594,\
0.0388002583333332817794,0.0633959958333333295499,0.0633959958333333295499,\
-0.1205085666666666222024,-0.0095530508364020244594,-0.0295434574969313107351}

#define IRESULT3 {0,-0.0359991099727139385323,0.0359991099727139385323,\
-0.0277100416666666432564,-0.0873273732016361936559,0.0495491398683028591576,\
0,-0.0453116254771752935415,0.0453116254771752935415,\
0.0277100416666666432564,-0.0495491398683028591576,0.0873273732016361936559}

#define RRESULT4 {0.5247468416666666302106,0.0120497083333333254718,-0.0850471916666666466478,\
 0.0120497083333333254718,\
-0.0631457083333333279995,-0.0562903158939566938823,0.0675598583333333335688,\
 -0.0743609174393766170219,\
-0.0631457083333333279995,-0.0743609174393766170219,0.0675598583333333335688,\
 -0.0562903158939566938823}
 
#define IRESULT4 {0,0.0565971833333333285143,0,-0.0565971833333333285143,\
-0.0250953810419741081550,0.1196492105704671793376,0.0191473164961883796087,\
 0.0362419439038005331000,\
0.0250953810419741081550,-0.0362419439038005331000,-0.0191473164961883796087,\
 -0.1196492105704671793376}

#define RRESULT6 {0.5247468416666666302106,-0.0417166874999999950924,-0.0388444708333333249550,\
 -0.0927985249999999928239,-0.0388444708333333249550,-0.0417166874999999950924,\
0.0388002583333333372906,0.0436237291666666596179,-0.0186651041666666545060,\
 -0.1482186083333333070922,-0.0186651041666666545060,0.0436237291666666596179}

#define IRESULT6 {0,-0.0122945224279474088491,0.1021380474100006957583,0,-0.1021380474100006957583,\
 0.0122945224279474088491,\
0,-0.0926896236521720928714,0.0110650550297080874085,0,-0.0110650550297080874085,\
 0.0926896236521720928714}

#define RRESULT9 {5,-0.5000000000000002220446,-0.5000000000000002220446,\
		-1.4999999999999997779554,- 0.0000000000000000286185, 0.0000000000000001396408,\
		-1.4999999999999997779554,0.0000000000000001396408,- 0.0000000000000000286185}

#define IRESULT9 {0,- 0.2886751345948128100183,0.2886751345948128100183,\
		- 0.8660254037844383745437,- 0.0000000000000000138778,0.0000000000000000138778,\
		+ 0.8660254037844383745437,- 0.0000000000000000138778,+ 0.0000000000000000138778}


static void difftmaTest(void){
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
	difftma(in1, 1, 12, out1);
	for (i=0;i<12;i++){
	if (out1[i]>1e-16)	assert( (fabs(out1[i]-resR1[i]) / fabs(out1[i])) < 3e-14 );
	else			assert(1);
	}

	printf(" >>> Matrice 2*6 <<< \n");
	difftma(in2, 2, 6, out2);
	for (i=0;i<12;i++){
	if (out2[i]>1e-16)	assert( (fabs(out2[i]-resR2[i]) / fabs(out2[i])) < 3e-15 );
	else			assert(1);
	}
	

	printf(" >>> Matrice 3*4 <<< \n");	
	difftma(in3, 3, 4, out3);
	for (i=0;i<12;i++){	
	if (out3[i]>1e-16)	assert( (fabs(out3[i]-resR3[i]) / fabs(out3[i])) < 3e-15 );
	else			assert(1);
	}

	printf(" >>> Matrice 4*3 <<< \n");	
	difftma(in4, 4, 3, out4);
	for (i=0;i<12;i++){	
	if (out4[i]>1e-16)	assert( (fabs(out4[i]-resR4[i]) / fabs(out4[i])) < 3e-15 );
	else			assert(1);
	}

	printf(" >>> Matrice 6*2 <<< \n");	
	difftma(in6, 6, 2, out6);
	for (i=0;i<12;i++){	
	if (out6[i]>1e-16)	assert( (fabs(out6[i]-resR6[i]) / fabs(out6[i])) < 3e-15 );
	else			assert(1);
	}

	printf(" >>> Matrice 3*3 <<< \n");	
	difftma(in9, 3, 3, out9);
	for (i=0;i<9;i++){
	if (out9[i]>1e-16)	assert( (fabs(out9[i]-resR9[i]) / fabs(out9[i])) < 3e-15 );
	else			assert(1);
	}
	

}

static void zifftmaTest(void){
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

	
	doubleComplex *in1, *in2, *in3, *in4, *in6, *in9, out1[12], out2[12], out3[12], out4[12], out6[12];

	in1=(doubleComplex *)malloc((unsigned int)12*sizeof(doubleComplex));
	in2=(doubleComplex *)malloc((unsigned int)12*sizeof(doubleComplex));
	in3=(doubleComplex *)malloc((unsigned int)12*sizeof(doubleComplex));
	in4=(doubleComplex *)malloc((unsigned int)12*sizeof(doubleComplex));
	in6=(doubleComplex *)malloc((unsigned int)12*sizeof(doubleComplex));
	in9=(doubleComplex *)malloc((unsigned int)9*sizeof(doubleComplex));
	
	
	
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
	zifftma(in1, 1, 12, out1);
	for (i=0;i<12;i++){
	if (zreals(out1[i])>1e-16)	assert( (fabs(zreals(out1[i])-resR1[i]) / fabs(zreals(out1[i]))) < 3e-14 );
	else			assert(1);
	if (zimags(out1[i])>1e-16)	assert( (fabs(zimags(out1[i])+resI1[i]) / fabs(zimags(out1[i]))) < 3e-15 );
	else			assert(1);
	}

	printf(" >>> Matrice 2*6 <<< \n");
	zifftma(in2, 2, 6, out2);
	for (i=0;i<12;i++){
	if (zreals(out2[i])>1e-16)	assert( (fabs(zreals(out2[i])-resR2[i]) / fabs(zreals(out2[i]))) < 3e-15 );
	else			assert(1);
	if (zimags(out2[i])>1e-16)	assert( (fabs(zimags(out2[i])+resI2[i]) / fabs(zimags(out2[i]))) < 3e-13 );
	else			assert(1);
	}
	

	printf(" >>> Matrice 3*4 <<< \n");	
	zifftma(in3, 3, 4, out3);
	for (i=0;i<12;i++){	
	if (zreals(out3[i])>1e-16)	assert( (fabs(zreals(out3[i])-resR3[i]) / fabs(zreals(out3[i]))) < 3e-15 );
	else			assert(1);
	if (zimags(out3[i])>1e-16)	assert( (fabs(zimags(out3[i])+resI3[i]) / fabs(zimags(out3[i]))) < 3e-15 );
	else			assert(1);
	}

	printf(" >>> Matrice 4*3 <<< \n");	
	zifftma(in4, 4, 3, out4);
	for (i=0;i<12;i++){	
	if (zreals(out4[i])>1e-16)	assert( (fabs(zreals(out4[i])-resR4[i]) / fabs(zreals(out4[i]))) < 3e-15 );
	else			assert(1);
	if (zimags(out4[i])>1e-16)	assert( (fabs(zimags(out4[i])+resI4[i]) / fabs(zimags(out4[i]))) < 3e-15 );
	else			assert(1);
	}

	printf(" >>> Matrice 6*2 <<< \n");	
	zifftma(in6, 6, 2, out6);
	for (i=0;i<12;i++){	
	if (zreals(out6[i])>1e-16)	assert( (fabs(zreals(out6[i])-resR6[i]) / fabs(zreals(out6[i]))) < 3e-15 );
	else			assert(1);
	if (zimags(out6[i])>1e-16)	assert( (fabs(zimags(out6[i])+resI6[i]) / fabs(zimags(out6[i]))) < 3e-15 );
	else			assert(1);
	}


}



static int testiFft(void){
	difftmaTest();
	zifftmaTest();
	return 0;
}


int main(void) {
  printf(">>> Fft Matrices Double Tests <<<\n");
  assert(testiFft() == 0);
  return 0;
}

