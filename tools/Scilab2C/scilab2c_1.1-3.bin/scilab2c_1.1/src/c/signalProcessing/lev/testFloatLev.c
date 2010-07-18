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
#include "lev.h"
#include <math.h>


static void slevaTest(void) {

	printf(" \n -------Float Test------- \n");
	/* TEST 1 */
	{	
	float in[4]={1.0f,2.0f,3.0f,4.0f};
	float result1[3]={ - 1.25f , 0.0000000000000002220446f , - 0.2500000000000001110223f };
	float result2 = -2.5;
	float result3[3]={- 2.0f,- 0.3333333333333333148296f, - 0.2500000000000001110223f };
	float out1[3]={0.0f}, out2=0.0f, out3[3]={0.0f};
	int i;
		
	out2 = sleva(in,4,out1,out3);
	
	;
	for (i=0;i<3;i++) {
		if (out1[i]>1e-6)	assert( (fabs(out1[i]-result1[i]) / fabs (out1[i]) )<1e-8);
		else assert(1);
	}
	
	if (out2>1e-6)	assert((fabs(out2-result2) / fabs (out2))<1e-8);
	else assert (1);
	
	for (i=0;i<3;i++) {
		if (out3[i]>1e-6)	 assert((fabs(out3[i]-result3[i]) / fabs (out3[i]))<1e-8);
		else assert(1);
	}
	
	}
	
	/* TEST 2 */
	{
	float in[8]= {0.9475104575976729393005f,0.0563528141938149929047f,0.8801407832652330398560f,0.5110759926028549671173f,0.8736385302618145942688f, 0.9233113336376845836639f,0.1977134644985198974609f,0.0427226074971258640289f};
	float result1[7]= { - 1.4145989907356759651691f , 
  - 0.1879185484276382211988f  ,
    1.9546862646288580833698f  ,
    0.0431855597974486471458f  ,
  - 0.6934494907248942574540f  ,
    0.3355525090987989678126f  ,
  - 0.7681883584831936673254f};
	float result2 =  1.132375302303337161192f;
	float result3[7]=  {- 0.0594746092161267045362f  ,
  - 0.9286458300417500533186f  ,
  - 3.5018950960448012033f     ,
    1.0467634735526456513099f  ,
    3.0600285663270052616269f  ,
  - 1.8325211983274933835730f  ,
  - 0.7681883584831936673254f  };
	float out1[7]={0.0f}, out2=0.0f, out3[7]={0.0f};
	int i;
		
	out2 = sleva(in,8,out1,out3);
	/*FIXME : Assert à 10^-4*/
	for (i=0;i<7;i++) {
		if (out1[i]>1e-6)	assert((fabs(out1[i]-result1[i]) / fabs (out1[i]))<3e-5);
		else assert(1);
	}
	
	if (out2>1e-6)		assert((fabs(out2-result2) / fabs (out2))<1e-6);
	
	
	for (i=0;i<7;i++) {
		if (out3[i]>1e-6)	  assert((fabs(out3[i]-result3[i]) / fabs (out3[i]))<1e-6);
		else assert(1);
	}
	
	}
}


static void clevaTest(void) {

	/* TEST 1 */
	{
	float inR[4]={1.0f,2.0f,3.0f,4.0f};
	float inI[4]={1.0f,1.0f,1.0f,1.0f};
	float result1R[3]={  0.6666666666666662965923f ,- 4.6666666666666687390830f , 2.0000000000000004440892f };
	float result1I[3]={ - 1.1666666666666669627261f , - 2.1666666666666665186369f , 2.5f };
	float result2R = 6.1666666666666678509046f;
	float result2I = 6.1666666666666678509046f;
	float result3R[3]={- 1.5f,0.3333333333333333148f, 2.0000000000000004440f };
	float result3I[3]={ + 0.5000000000000000000000f , + 0.6666666666666666296592f , 2.5f };
	floatComplex out1[3], out2=FloatComplex(0,0), out3[3];
	floatComplex *in;
	int i;
	
	printf(" \n -------FloatComplex Test------- \n");
	
	in=FloatComplexMatrix(inR,inI,4);
	
	out2 = cleva(in,4,out1,out3);
	
	for (i=0;i<3;i++){
		if (creals(out1[i])>1e-6) assert((fabs(creals(out1[i])-result1R[i]) / fabs(creals(out1[i]))) <1e-6);	
		else assert(1);
		if (cimags(out1[i])>1e-6) assert((fabs(cimags(out1[i])-result1I[i]) / fabs(cimags(out1[i]))) <1e-7);	
		else assert(1);
	}
	
	if (creals(out2)>1e-6) assert((fabs(creals(out2)-result2R) / fabs(creals(out2))) <1e-6);	
	else assert(1);
	if (cimags(out2)>1e-6) assert((fabs(cimags(out2)-result2I) / fabs(cimags(out2))) <1e-6);	
	else assert(1);
	
	for (i=0;i<3;i++){
		if (creals(out3[i])>1e-6) assert((fabs(creals(out3[i])-result3R[i]) / fabs(creals(out3[i]))) <1e-6);	
		else assert(1);
		if (cimags(out3[i])>1e-6) assert((fabs(cimags(out3[i])-result3I[i]) / fabs(cimags(out3[i]))) <1e-8);	
		else assert(1);
	}
	}
	
	
	
	/* TEST 2 */

	{
	float inR[6]={0.0143211213871836662292f,0.7033217861317098140717f,0.5212594550102949142456f,0.0582674746401607990265f,
			   0.488313094712793827057f,0.8035244266502559185028f};
	float inI[6]={0.3819901309907436370850f,0.4311556280590593814850f,0.7573182275518774986267f,0.0120028085075318813324f,
			   0.9994637314230203628540f,0.9508706149645149707794f};
	float result1R[5]={- 3.6095794839789050456602f,- 22.476200430215509840082f,- 8.9817944433992220609753f,
				  12.625498164379671095503f, 3.5273512791323136106314f};
	float result1I[5]={9.8938676519388195629290f,- 11.160861495580743252276f,- 12.396447957003889683847f,
				  - 20.810995133509468502098f,3.8571422461904862899473f};
	float result2R = 1.2259658790319762911025f;
	float result2I = 32.700432742697543631039f;
	float result3R[5]={ - 1.1960559267342236733356f,- 0.0224784059157190282963f,- 2.232787465146694128038f,
				   0.4864901943454941202205f,3.5273512791323136106314f};
	float result3I[5]={1.7963629642455771762144f,0.7825308030566066053169f,1.4948702216638325435127f,
				  - 1.0642505925602050442791f,3.8571422461904862899473f};
	floatComplex out1[5], out2=FloatComplex(0,0), out3[5];
	floatComplex *in;
	int i;
	
	printf(" \n -------FloatComplex Test------- \n");
	
	in=FloatComplexMatrix(inR,inI,6);
	
	out2 = cleva(in,6,out1,out3);
	
	
	
	for (i=0;i<5;i++) printf("out : %1.20f+%1.20f\n",creals(out1[i]),cimags(out1[i]));
	for (i=0;i<5;i++){
		if (creals(out1[i])>1e-6) assert((fabs(creals(out1[i])-result1R[i]) / fabs(creals(out1[i]))) <1e-6);	
		else assert(1);
		if (cimags(out1[i])>1e-6) assert((fabs(cimags(out1[i])-result1I[i]) / fabs(cimags(out1[i]))) <1e-6);	
		else assert(1);
	}
	
	if (creals(out2)>1e-6) assert((fabs(creals(out2)-result2R) / fabs(creals(out2))) <1e-8);	
	else assert(1);
	if (cimags(out2)>1e-6) assert((fabs(cimags(out2)-result2I) / fabs(cimags(out2))) <1e-8);	
	else assert(1);
	
	for (i=0;i<5;i++){
		if (creals(out3[i])>1e-6) assert((fabs(creals(out3[i])-result3R[i]) / fabs(creals(out3[i]))) <1e-7);	
		else assert(1);
		if (cimags(out3[i])>1e-6) assert((fabs(cimags(out3[i])-result3I[i]) / fabs(cimags(out3[i]))) <1e-6);	
		else assert(1);
	}
	}
}




static int levTest(void) {
  printf("\n>>>> Lev Tests\n");
  slevaTest();
  clevaTest();
  return 0;
}

int main(void) {
  assert( levTest()== 0);
  return 0;
}
