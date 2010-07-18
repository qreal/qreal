/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include "testFind.h"

int sfindaTest() {
  int result = 0, i = 0;
  float goodArray[5] = {0.,2.,3.,5.,10.};
  float badArray[5] = {0.,0.,0.,0.,0.};
  float res[4] = {2.,3.,4.,5.};
  float *outGood = NULL, *outBad = NULL;
  int indiceOut; 
  
  outGood=(float*)malloc((unsigned int)5*sizeof(float));
  outBad=(float*)malloc((unsigned int)5*sizeof(float));  
 
  printf(">> Floats \n");
  sfinda(goodArray, 5, outGood, &indiceOut,-1);
  sfinda(badArray, 5, outBad, &indiceOut,-1);
  
  for (i=0;i<4;i++){
  	if ( outGood[i] != res[i]) {
    printf("ERROR ! : Test Failed (non empty array)\n");
    result = ERROR;
      }
	else
    	  printf("%f  ",outGood[i]);
  }
  
  printf("\n");
  
  if (outBad[0]!=-1) {
    printf("ERROR ! : Test Failed (empty array)\n");
    result = ERROR;
  }
  else
     printf("%f  ",outBad[0]);
    
  printf("\n");
  free(outBad);
  free(outGood);
  return result;
}

int dfindaTest() {
  int result = 0, i = 0;
  double goodArray[5] = {0.,2.,3.,5.,10.};
  double badArray[5] = {0.,0.,0.,0.,0.};
  double res[4] = {2.,3.,4.,5.};
  double *outGood = NULL, *outBad = NULL;
  int indiceOut; 
  
  outGood=(double*)malloc((unsigned int)5*sizeof(double));
  outBad=(double*)malloc((unsigned int)5*sizeof(double));  
  
  
  printf(">> Double \n");
  dfinda(goodArray, 5, outGood, &indiceOut,-1);
  dfinda(badArray, 5, outBad, &indiceOut,-1);
  
  for (i=0;i<4;i++){
  	if ( outGood[i] != res[i]) {
    printf("ERROR ! : Test Failed (non empty array)\n");
    result = ERROR;
      }
	else
    	  printf("%f  ",outGood[i]);
  }
  
  printf("\n");
  
  if (outBad[0]!=-1) {
    printf("ERROR ! : Test Failed (empty array)\n");
    result = ERROR;
  }
  else
     printf("%f  ",outBad[0]);
    
  printf("\n");
  free(outBad);
  free(outGood);
  return result;
}

int cfindaTest() {
  int result = 0, i = 0;
  floatComplex goodArray[5]; 
  floatComplex badArray[5]; 
  float res[4] = {2.,3.,4.,5.};
  float *outGood = NULL, *outBad = NULL;
  int indiceOut; 
  
  outGood=(float*)malloc((unsigned int)5*sizeof(float));
  outBad=(float*)malloc((unsigned int)5*sizeof(float));  
 
  /* Good values in goodArray */ 
  goodArray[0] = FloatComplex(0., 0.); 
  goodArray[1] = FloatComplex(0., 2.); 
  goodArray[2] = FloatComplex(3., 50.); 
  goodArray[3] = FloatComplex(5., 10.); 
  goodArray[4] = FloatComplex(10., -10.); 
  /* Bad values in badArray */ 
  badArray[0] = FloatComplex(0., 0.); 
  badArray[1] = FloatComplex(0., 0.); 
  badArray[2] = FloatComplex(0., 0.); 
  badArray[3] = FloatComplex(0., 0.); 
  badArray[4] = FloatComplex(0., 0.);
  
  printf(">> Float Complex \n");
  cfinda(goodArray, 5, outGood, &indiceOut,-1);
  cfinda(badArray, 5, outBad, &indiceOut,-1);
  
  for (i=0;i<4;i++){
  	if ( outGood[i] != res[i]) {
    printf("ERROR ! : Test Failed (non empty array)\n");
    result = ERROR;
      }
	else
    	  printf("%f  ",outGood[i]);
  }
  
  printf("\n");
  
  if (outBad[0]!=-1) {
    printf("ERROR ! : Test Failed (empty array)\n");
    result = ERROR;
  }
  else
     printf("%f  ",outBad[0]);
    
  printf("\n");
  free(outBad);
  free(outGood);
  return result;
}

int zfindaTest() {
  int result = 0, i = 0;
  doubleComplex goodArray[5]; 
  doubleComplex badArray[5]; 
  double res[4] = {2.,3.,4.,5.};
  double *outGood = NULL, *outBad = NULL;
  int indiceOut; 
  
  outGood=(double*)malloc((unsigned int)5*sizeof(double));
  outBad=(double*)malloc((unsigned int)5*sizeof(double));  
  
  
  /* Good values in goodArray */ 
  goodArray[0] = DoubleComplex(0., 0.); 
  goodArray[1] = DoubleComplex(0., 2.); 
  goodArray[2] = DoubleComplex(3., 50.); 
  goodArray[3] = DoubleComplex(5., 10.); 
  goodArray[4] = DoubleComplex(10., -10.); 
  /* Bad values in badArray */ 
  badArray[0] = DoubleComplex(0., 0.); 
  badArray[1] = DoubleComplex(0., 0.); 
  badArray[2] = DoubleComplex(0., 0.); 
  badArray[3] = DoubleComplex(0., 0.); 
  badArray[4] = DoubleComplex(0., 0.);
 
  
  printf(">> Double Complex \n");
  zfinda(goodArray, 5, outGood, &indiceOut,-1);
  zfinda(badArray, 5, outBad, &indiceOut,-1);
  
  for (i=0;i<4;i++){
  	if ( outGood[i] != res[i]) {
    printf("ERROR ! : Test Failed (non empty array)\n");
    result = ERROR;
      }
	else
    	  printf("%f  ",outGood[i]);
  }
  
  printf("\n");

  if (outBad[0]!=-1) {
    printf("ERROR ! : Test Failed (empty array)\n");
    result = ERROR;
  }
  else
     printf("%f  ",outBad[0]);
 
  printf("\n");
  free(outBad);
  free(outGood);
  return result;
}

int testFind() {
  int sfindaStatus, dfindaStatus = 0;
  int cfindaStatus, zfindaStatus = 0;

  printf(">>>> Find Tests\n");
  sfindaStatus = sfindaTest();
  dfindaStatus = dfindaTest();
  cfindaStatus = cfindaTest();
  zfindaStatus = zfindaTest();

  return (sfindaStatus + dfindaStatus +
	  cfindaStatus + zfindaStatus);
}

int main(void) {
  assert(testFind() == 0);
  return 0;
}
