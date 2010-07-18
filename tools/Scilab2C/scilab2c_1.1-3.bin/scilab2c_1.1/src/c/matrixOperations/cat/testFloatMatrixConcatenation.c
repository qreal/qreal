/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <assert.h>
#include <stdio.h>
#include "constant.h"
#include "cat.h"

static void srowcatsTest(void) {
  int i = 0;
  float output[2] = {0};

  /*
  ** [ 42 51 ]
  */
  srowcats(42.0, 51.0, output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e\n", i, output[i]);
  }
  assert(output[0] == 42.0);
  assert(output[1] == 51.0);

  /*
  ** [ PI PI/2 ]
  */
  srowcats(FPI, FPI / 2, output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e\n", i, output[i]);
  }
  assert(output[0] == FPI);
  assert(output[1] == FPI / 2);
}

static void scolumncatsTest(void) {
  int i = 0;
  float output[2] = {0};

  /*
  ** [ 42 51 ]
  */
  scolumncats(42.0, 51.0, output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e\n", i, output[i]);
  }
  assert(output[0] == 42.0);
  assert(output[1] == 51.0);

  /*
  ** [ PI PI/2 ]
  */
  scolumncats(FPI, FPI / 2, output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e\n", i, output[i]);
  }
  assert(output[0] == FPI);
  assert(output[1] == FPI / 2);
}

static void scolumncataTest(void) {
  int i = 0;
  float in1[12] = {0.0, 1.0, 2.0 , 3.0 , 4.0, 5.0 , 6.0 , 7.0 , 8.0 , 9.0, 10.0 , 11.0};
  float in2_1_1[1] = {12.0};
  float out_1_13[13] = {0};
  float in2_2_2[4] = {FPI, FPI / 2, FPI / 3, FPI / 4};
  float out_2_8[16] = {0};

  /*
  **  columncat( [ 0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 ], [ 12.0 ] )
  ** [ 0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0 ]
  */
  scolumncata(in1, 1, 12, in2_1_1, 1, 1, out_1_13);
  for (i = 0 ; i < 13 ; ++i) {
    printf("out_1_13[%d] = %e\n", i, out_1_13[i]);
    assert(out_1_13[i] == (float) i);
  }

  /*
  ** [ 0 2 4 6 8 10 ] , [ PI   PI/3 ] => [ 0 2 4 6 8 10 PI   PI/3 ]
  ** [ 1 3 5 7 9 11 ]   [ PI/2 PI/4]     [ 1 3 5 7 9 11 PI/2 PI/4 ]
  */
  scolumncata(in1, 2, 6, in2_2_2, 2, 2, out_2_8);
  for (i = 0 ; i < 16 ; ++i) {
    printf("out_2_8[%d] = %e\n", i, out_2_8[i]);
  }
  for (i = 0 ; i < 12 ; ++i) {
    assert(out_2_8[i] == in1[i]);
  }
  for (i = 0 ; i < 4 ; ++i) {
    assert(out_2_8[i + 12] == in2_2_2[i]);
  }

	

}

static void   srowcataTest() {
  int i = 0;
  float in1[6] = {1.0 , 2.0, 5.0 , 6.0, 9.0 , 10.0};
  float in2[6] = {3.0 , 4.0 , 7.0 , 8.0 , 11.0 , 12.0};
  float out_4_3[12] = {0};
  float out_6_2[12] = {0};
  
  float inNono1[6]={1.0,3.0,5.0,2.0,4.0,6.0};
  float inNono2[2]={1.0,2.0};
  float outNono[8]={0};
  /*
  ** [ 1 5 9  ] ; [ 3 7 11 ] => [ 1 5 9  ]
  ** [ 2 6 10 ]   [ 4 8 12 ]	[ 2 6 10 ]
  **				[ 3 7 11 ]
  **				[ 4 8 12 ]
  */
  srowcata(in1, 2, 3, in2, 2, 3, out_4_3);
  for (i = 0 ; i < 12 ; ++i) {
    printf("out_4_3[%d] = %e\n", i, out_4_3[i]);
    assert(out_4_3[i] == (float) i + 1.0);
  }

  /*
  ** [ 1 6  ] ; [ 3 8  ]
  ** [ 2 9  ]   [ 4 11 ]
  ** [ 5 10 ]   [ 7 12 ]
  */
  srowcata(in1, 3, 2, in2, 3, 2, out_6_2);
 for (i = 0 ; i < 12 ; ++i) {
    printf("out_6_2[%d] = %e\n", i, out_6_2[i]);
 }
 assert(out_6_2[0] == 1.0);
 assert(out_6_2[1] == 2.0);
 assert(out_6_2[2] == 5.0);
 assert(out_6_2[3] == 3.0);
 assert(out_6_2[4] == 4.0);
 assert(out_6_2[5] == 7.0);
 assert(out_6_2[6] == 6.0);
 assert(out_6_2[7] == 9.0);
 assert(out_6_2[8] == 10.0);
 assert(out_6_2[9] == 8.0);
 assert(out_6_2[10] == 11.0);
 assert(out_6_2[11] == 12.0);
 
 
 srowcata(inNono1, 3, 2, inNono2, 1, 2, outNono);
 assert(outNono[0] == 1.0);
 assert(outNono[1] == 3.0);
 assert(outNono[2] == 5.0);
 assert(outNono[3] == 1.0);
 assert(outNono[4] == 2.0);
 assert(outNono[5] == 4.0);
 assert(outNono[6] == 6.0);
 assert(outNono[7] == 2.0); 
 
}

static void crowcatsTest(void) {
  int i = 0;
  floatComplex output[2];

  /*
  ** [ 42+25i 51+38i ]
  */
  crowcats(FloatComplex(42.0, 25.0),
	   FloatComplex(51.0, 38.0),
	   output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e + %e i\n", i, creals(output[i]), cimags(output[i]));
  }
  assert(creals(output[0]) == 42.0);
  assert(cimags(output[0]) == 25.0);
  assert(creals(output[1]) == 51.0);
  assert(cimags(output[1]) == 38.0);

  /*
  ** [ PI+PI/2i PI/2+PIi ]
  */
  crowcats(FloatComplex(FPI, FPI/2),
	   FloatComplex(FPI/2, FPI),
	   output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e + %e i\n", i, creals(output[i]), cimags(output[i]));
  }
  assert(creals(output[0]) == FPI);
  assert(cimags(output[0]) == FPI / 2);
  assert(creals(output[1]) == FPI / 2);
  assert(cimags(output[1]) == FPI);
}

static void ccolumncatsTest(void) {
  int i = 0;
  floatComplex output[2];
  /*
  ** [ 42+25i 51+38i ]
  */
  ccolumncats(FloatComplex(42.0, 25.0),
	   FloatComplex(51.0, 38.0),
	   output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e + %e i\n", i, creals(output[i]), cimags(output[i]));
  }
  assert(creals(output[0]) == 42.0);
  assert(cimags(output[0]) == 25.0);
  assert(creals(output[1]) == 51.0);
  assert(cimags(output[1]) == 38.0);

  /*
  ** [ PI+PI/2i PI/2+PIi ]
  */
  ccolumncats(FloatComplex(FPI, FPI/2),
	   FloatComplex(FPI/2, FPI),
	   output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e + %e i\n", i, creals(output[i]), cimags(output[i]));
  }
  assert(creals(output[0]) == FPI);
  assert(cimags(output[0]) == FPI / 2);
  assert(creals(output[1]) == FPI / 2);
  assert(cimags(output[1]) == FPI);

}

static void ccolumncataTest(void) {
  int i = 0;
  float in1_real[12] = {0.0 , 1.0 , 2.0 , 3.0 , 4.0 , 5.0 , 6.0 , 7.0 , 8.0 , 9.0 , 10.0 , 11.0};
  float in1_imag[12] = {11.0 , 10.0 , 9.0 , 8.0 , 7.0 , 6.0 , 5.0 , 4.0 , 3.0 , 2.0 , 1.0 , 0.0};
  floatComplex *in1 = FloatComplexMatrix(in1_real, in1_imag, 12);
  float in2_real[1] = {12.0};
  float in2_imag[1] = {-1.0};
  floatComplex *in2_1_1 = FloatComplexMatrix(in2_real, in2_imag, 1);
  floatComplex out_1_13[13];

  float in2_2_2_real[4] = {FPI, FPI / 2, FPI / 3, FPI / 4};
  float in2_2_2_imag[4] = {FPI / 4, FPI / 3, FPI / 2, FPI};
  floatComplex *in2_2_2 = FloatComplexMatrix(in2_2_2_real, in2_2_2_imag, 4);
  floatComplex out_2_8[16];

  /*
  **  columncat( [ 0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 ], [ 12.0 ] )
  ** [ 0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0 ]
  */
  ccolumncata(in1, 1, 12, in2_1_1, 1, 1, out_1_13);
  for (i = 0 ; i < 13 ; ++i) {
    printf("out_1_13[%d] = %e + %e i\n", i, creals(out_1_13[i]), cimags(out_1_13[i]));
    assert(creals(out_1_13[i]) == (float) i);
    assert(cimags(out_1_13[i]) == 11.0 - (float) i);
  }

  /*
  ** [ 0 2 4 6 8 10 ] , [ PI   PI/3 ] => [ 0 2 4 6 8 10 PI   PI/3 ]
  ** [ 1 3 5 7 9 11 ]   [ PI/2 PI/4]     [ 1 3 5 7 9 11 PI/2 PI/4 ]
  */
  ccolumncata(in1, 2, 6, in2_2_2, 2, 2, out_2_8);
  for (i = 0 ; i < 16 ; ++i) {
    printf("out_2_8[%d] = %e + %e i\n", i, creals(out_2_8[i]), cimags(out_2_8[i]));
  }
  for (i = 0 ; i < 12 ; ++i) {
    assert(creals(out_2_8[i]) == creals(in1[i]));
    assert(cimags(out_2_8[i]) == cimags(in1[i]));
  }
  for (i = 0 ; i < 4 ; ++i) {
    assert(creals(out_2_8[i + 12]) == creals(in2_2_2[i]));
    assert(cimags(out_2_8[i + 12]) == cimags(in2_2_2[i]));
  }

}

static void   crowcataTest() {
  int i = 0;
  float in1_real[6] = {1.0 , 2.0, 5.0 , 6.0, 9.0 , 10.0};
  float in1_imag[6] = {1.0 , 2.0, 5.0 , 6.0, 9.0 , 10.0};
  floatComplex *in1 = FloatComplexMatrix(in1_real, in1_imag, 6);
  float in2_real[6] = {3.0 , 4.0 , 7.0 , 8.0 , 11.0 , 12.0};
  float in2_imag[6] = {3.0 , 4.0 , 7.0 , 8.0 , 11.0 , 12.0};
  floatComplex *in2 = FloatComplexMatrix(in2_real, in2_imag, 6);
  floatComplex out_4_3[12];
  floatComplex out_6_2[12];
  /*
  ** [ 1 5 9  ] ; [ 3 7 11 ] => [ 1 5 9  ]
  ** [ 2 6 10 ]   [ 4 8 12 ]	[ 2 6 10 ]
  **				[ 3 7 11 ]
  **				[ 4 8 12 ]
  */
  crowcata(in1, 2, 3, in2, 2, 3, out_4_3);
  for (i = 0 ; i < 12 ; ++i) {
    printf("out_4_3[%d] = %e + %e i\n", i, creals(out_4_3[i]), cimags(out_4_3[i]));
    assert(creals(out_4_3[i]) == (float) i + 1.0);
    assert(cimags(out_4_3[i]) == (float) i + 1.0);
  }

  /*
  ** [ 1 6  ] ; [ 3 8  ]
  ** [ 2 9  ]   [ 4 11 ]
  ** [ 5 10 ]   [ 7 12 ]
  */
  crowcata(in1, 3, 2, in2, 3, 2, out_6_2);
 for (i = 0 ; i < 12 ; ++i) {
   printf("out_6_2[%d] = %e + %e i\n", i, creals(out_6_2[i]), cimags(out_6_2[i]));
 }
 assert(creals(out_6_2[0]) == 1.0);
 assert(cimags(out_6_2[0]) == 1.0);
 assert(creals(out_6_2[1]) == 2.0);
 assert(cimags(out_6_2[1]) == 2.0);
 assert(creals(out_6_2[2]) == 5.0);
 assert(cimags(out_6_2[2]) == 5.0);
 assert(creals(out_6_2[3]) == 3.0);
 assert(cimags(out_6_2[3]) == 3.0);
 assert(creals(out_6_2[4]) == 4.0);
 assert(cimags(out_6_2[4]) == 4.0);
 assert(creals(out_6_2[5]) == 7.0);
 assert(cimags(out_6_2[5]) == 7.0);
 assert(creals(out_6_2[6]) == 6.0);
 assert(cimags(out_6_2[6]) == 6.0);
 assert(creals(out_6_2[7]) == 9.0);
 assert(cimags(out_6_2[7]) == 9.0);
 assert(creals(out_6_2[8]) == 10.0);
 assert(cimags(out_6_2[8]) == 10.0);
 assert(creals(out_6_2[9]) == 8.0);
 assert(cimags(out_6_2[9]) == 8.0);
 assert(creals(out_6_2[10]) == 11.0);
 assert(cimags(out_6_2[10]) == 11.0);
 assert(creals(out_6_2[11]) == 12.0);
 assert(cimags(out_6_2[11]) == 12.0);
}

int main(void) {
  srowcatsTest();
  scolumncatsTest();
  srowcataTest();
  scolumncataTest();
  crowcatsTest();
  ccolumncatsTest();
  crowcataTest();
  ccolumncataTest();
  return 0;
}
