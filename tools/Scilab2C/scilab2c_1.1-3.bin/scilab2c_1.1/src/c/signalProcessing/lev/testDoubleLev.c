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


static void dlevaTest(void) {

	printf(" \n -------Double Test------- \n");
	/* TEST 1 */
	{	
	double in[4]={1,2,3,4};
	double result1[3]={ - 1.25 , 0.0000000000000002220446 , - 0.2500000000000001110223 };
	double result2 = -2.5;
	double result3[3]={- 2.,- 0.3333333333333333148296, - 0.2500000000000001110223 };
	double out1[3]={0}, out2=0, out3[3]={0};
	int i;
		
	out2 = dleva(in,4,out1,out3);
	
	;
	for (i=0;i<3;i++) {
	  printf ("assert : %e \n" , (fabs(out1[i]-result1[i]) )/ fabs (out1[i]))  ;
		if (out1[i]>1e-15)	assert((fabs(out1[i]-result1[i]) / fabs (out1[i]))<3e-15);
		else assert(1);
	}
	
	if (out2>1e-16)	assert((fabs(out2-result2) / fabs (out2))<1e-16);
	else assert (1);
	
	for (i=0;i<3;i++) {
		if (out3[i]>1e-16)	 assert((fabs(out3[i]-result3[i]) / fabs (out3[i]))<1e-14);
		else assert(1);
	}
	
	}
	
	/* TEST 2 */
	{
	double in[8]= {0.9475104575976729393005,0.0563528141938149929047,0.8801407832652330398560,0.5110759926028549671173,0.8736385302618145942688, 0.9233113336376845836639,0.1977134644985198974609,0.0427226074971258640289};
	double result1[7]= { - 1.4145989907356759651691 , 
  - 0.1879185484276382211988  ,
    1.9546862646288580833698  ,
    0.0431855597974486471458  ,
  - 0.6934494907248942574540  ,
    0.3355525090987989678126  ,
  - 0.7681883584831936673254};
	double result2 =  1.132375302303337161192;
	double result3[7]=  {- 0.0594746092161267045362  ,
  - 0.9286458300417500533186  ,
  - 3.5018950960448012033     ,
    1.0467634735526456513099  ,
    3.0600285663270052616269  ,
  - 1.8325211983274933835730  ,
  - 0.7681883584831936673254  };
	double out1[7]={0}, out2=0, out3[7]={0};
	int i;
		
	out2 = dleva(in,8,out1,out3);
	/*FIXME : Assert à 10^-13*/
	for (i=0;i<7;i++) {
		if (out1[i]>1e-16)	assert((fabs(out1[i]-result1[i]) / fabs (out1[i]))<1e-13);
		else assert(1);
	}
	
	if (out2>1e-16)		assert((fabs(out2-result2) / fabs (out2))<3e-16);
	
	
	for (i=0;i<7;i++) {
		if (out3[i]>1e-15)	  assert((fabs(out3[i]-result3[i]) / fabs (out3[i]))<3e-14);
		else assert(1);
	}
	
	}
}


static void zlevaTest(void) {

	/* TEST 1 */
	{
	double inR[4]={1,2,3,4};
	double inI[4]={1,1,1,1};
	double result1R[3]={  0.6666666666666662965923 ,- 4.6666666666666687390830 , 2.0000000000000004440892 };
	double result1I[3]={ - 1.1666666666666669627261 , - 2.1666666666666665186369 , 2.5 };
	double result2R = 6.1666666666666678509046;
	double result2I = 6.1666666666666678509046;
	double result3R[3]={- 1.5,0.3333333333333333148, 2.0000000000000004440 };
	double result3I[3]={ + 0.5000000000000000000000 , + 0.6666666666666666296592 , 2.5 };
	doubleComplex out1[3], out2=DoubleComplex(0,0), out3[3];
	doubleComplex *in;
	int i;
	
	printf(" \n -------DoubleComplex Test------- \n");
	
	in=DoubleComplexMatrix(inR,inI,4);
	
	out2 = zleva(in,4,out1,out3);
	
	for (i=0;i<3;i++){
		if (zreals(out1[i])>1e-16) assert((fabs(zreals(out1[i])-result1R[i]) / fabs(zreals(out1[i]))) <3e-15);	
		else assert(1);
		if (zimags(out1[i])>1e-16) assert((fabs(zimags(out1[i])-result1I[i]) / fabs(zimags(out1[i]))) <1e-16);	
		else assert(1);
	}
	
	if (zreals(out2)>1e-16) assert((fabs(zreals(out2)-result2R) / fabs(zreals(out2))) <3e-15);	
	else assert(1);
	if (zimags(out2)>1e-16) assert((fabs(zimags(out2)-result2I) / fabs(zimags(out2))) <3e-15);	
	else assert(1);
	
	for (i=0;i<3;i++){
		if (zreals(out3[i])>1e-16) assert((fabs(zreals(out3[i])-result3R[i]) / fabs(zreals(out3[i]))) <3e-15);	
		else assert(1);
		if (zimags(out3[i])>1e-16) assert((fabs(zimags(out3[i])-result3I[i]) / fabs(zimags(out3[i]))) <1e-16);	
		else assert(1);
	}
	}
	
	
	
	/* TEST 2 */

	{
	double inR[6]={0.0143211213871836662292,0.7033217861317098140717,0.5212594550102949142456,0.0582674746401607990265,
			   0.488313094712793827057,0.8035244266502559185028};
	double inI[6]={0.3819901309907436370850,0.4311556280590593814850,0.7573182275518774986267,0.0120028085075318813324,
			   0.9994637314230203628540,0.9508706149645149707794};
	double result1R[5]={- 3.6095794839789050456602,- 22.476200430215509840082,- 8.9817944433992220609753,
				  12.625498164379671095503, 3.5273512791323136106314};
	double result1I[5]={9.8938676519388195629290,- 11.160861495580743252276,- 12.396447957003889683847,
				  - 20.810995133509468502098,3.8571422461904862899473};
	double result2R = 1.2259658790319762911025;
	double result2I = 32.700432742697543631039;
	double result3R[5]={ - 1.1960559267342236733356,- 0.0224784059157190282963,- 2.232787465146694128038,
				   0.4864901943454941202205,3.5273512791323136106314};
	double result3I[5]={1.7963629642455771762144,0.7825308030566066053169,1.4948702216638325435127,
				  - 1.0642505925602050442791,3.8571422461904862899473};
	doubleComplex out1[5], out2=DoubleComplex(0,0), out3[5];
	doubleComplex *in;
	int i;
	
	printf(" \n -------DoubleComplex Test------- \n");
	
	in=DoubleComplexMatrix(inR,inI,6);
	
	out2 = zleva(in,6,out1,out3);
	
	/*FIXME : Assert à 10^-14*/
	
	for (i=0;i<5;i++) printf("out : %1.20f+%1.20f\n",zreals(out1[i]),zimags(out1[i]));
	for (i=0;i<5;i++){
		if (zreals(out1[i])>1e-16) assert((fabs(zreals(out1[i])-result1R[i]) / fabs(zreals(out1[i]))) <3e-14);	
		else assert(1);
		if (zimags(out1[i])>1e-16) assert((fabs(zimags(out1[i])-result1I[i]) / fabs(zimags(out1[i]))) <3e-15);	
		else assert(1);
	}
	
	if (zreals(out2)>1e-16) assert((fabs(zreals(out2)-result2R) / fabs(zreals(out2))) <3e-15);	
	else assert(1);
	if (zimags(out2)>1e-16) assert((fabs(zimags(out2)-result2I) / fabs(zimags(out2))) <3e-15);	
	else assert(1);
	
	for (i=0;i<5;i++){
		if (zreals(out3[i])>1e-16) assert((fabs(zreals(out3[i])-result3R[i]) / fabs(zreals(out3[i]))) <3e-15);	
		else assert(1);
		if (zimags(out3[i])>1e-16) assert((fabs(zimags(out3[i])-result3I[i]) / fabs(zimags(out3[i]))) <3e-15);	
		else assert(1);
	}
	}
}




static int levTest(void) {
  printf("\n>>>> Lev Tests\n");
  dlevaTest();
  zlevaTest();
  return 0;
}

int main(void) {
  assert( levTest()== 0);
  return 0;
}
