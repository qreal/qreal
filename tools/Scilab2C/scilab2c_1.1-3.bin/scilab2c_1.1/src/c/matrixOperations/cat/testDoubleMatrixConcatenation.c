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

static void drowcatsTest(void) {
  int i = 0;
  double output[2] = {0};

  /*
  ** [ 42 51 ]
  */
  drowcats(42.0, 51.0, output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e\n", i, output[i]);
  }
  assert(output[0] == 42.0);
  assert(output[1] == 51.0);

  /*
  ** [ PI PI/2 ]
  */
  drowcats(DPI, DPI / 2, output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e\n", i, output[i]);
  }
  assert(output[0] == DPI);
  assert(output[1] == DPI / 2);
}

static void dcolumncatsTest(void) {
  int i = 0;
  double output[2] = {0};

  /*
  ** [ 42 51 ]
  */
  dcolumncats(42.0, 51.0, output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e\n", i, output[i]);
  }
  assert(output[0] == 42.0);
  assert(output[1] == 51.0);

  /*
  ** [ PI PI/2 ]
  */
  dcolumncats(DPI, DPI / 2, output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e\n", i, output[i]);
  }
  assert(output[0] == DPI);
  assert(output[1] == DPI / 2);
}

static void dcolumncataTest(void) {
  int i = 0;
  double in1[12] = {0.0, 1.0, 2.0 , 3.0 , 4.0, 5.0 , 6.0 , 7.0 , 8.0 , 9.0, 10.0 , 11.0};
  double in2_1_1[1] = {12.0};
  double out_1_13[13] = {0};

  double in2_2_2[4] = {DPI, DPI / 2, DPI / 3, DPI / 4};
  double out_2_8[16] = {0};

  /*
  **  columncat( [ 0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 ], [ 12.0 ] )
  ** [ 0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0 ]
  */
  dcolumncata(in1, 1, 12, in2_1_1, 1, 1, out_1_13);
  for (i = 0 ; i < 13 ; ++i) {
    printf("out_1_13[%d] = %e\n", i, out_1_13[i]);
    assert(out_1_13[i] == (double) i);
  }

  /*
  ** [ 0 2 4 6 8 10 ] , [ PI   PI/3 ] => [ 0 2 4 6 8 10 PI   PI/3 ]
  ** [ 1 3 5 7 9 11 ]   [ PI/2 PI/4]     [ 1 3 5 7 9 11 PI/2 PI/4 ]
  */
  dcolumncata(in1, 2, 6, in2_2_2, 2, 2, out_2_8);
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

static void   drowcataTest() {
  int i = 0;
  double in1[6] = {1.0 , 2.0, 5.0 , 6.0, 9.0 , 10.0};
  double in2[6] = {3.0 , 4.0 , 7.0 , 8.0 , 11.0 , 12.0};
  double out_4_3[12] = {0};
  double out_6_2[12] = {0};
  /*
  ** [ 1 5 9  ] ; [ 3 7 11 ] => [ 1 5 9  ]
  ** [ 2 6 10 ]   [ 4 8 12 ]	[ 2 6 10 ]
  **				[ 3 7 11 ]
  **				[ 4 8 12 ]
  */
  drowcata(in1, 2, 3, in2, 2, 3, out_4_3);
  for (i = 0 ; i < 12 ; ++i) {
    printf("out_4_3[%d] = %e\n", i, out_4_3[i]);
    assert(out_4_3[i] == (double) i + 1.0);
  }

  /*
  ** [ 1 6  ] ; [ 3 8  ]
  ** [ 2 9  ]   [ 4 11 ]
  ** [ 5 10 ]   [ 7 12 ]
  */
  drowcata(in1, 3, 2, in2, 3, 2, out_6_2);
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
 
 drowcata(in1, 1, 6, in2, 1, 6, out_6_2);
 for (i = 0 ; i < 12 ; ++i) {
    printf("out_6_2[%d] = %e\n", i, out_6_2[i]);
 }
 assert(out_6_2[0] == 1.0);
 assert(out_6_2[1] == 3.0);
 assert(out_6_2[2] == 2.0);
 assert(out_6_2[3] == 4.0);
 assert(out_6_2[4] == 5.0);
 assert(out_6_2[5] == 7.0);
 assert(out_6_2[6] == 6.0);
 assert(out_6_2[7] == 8.0);
 assert(out_6_2[8] == 9.0);
 assert(out_6_2[9] == 11.0);
 assert(out_6_2[10] == 10.0);
 assert(out_6_2[11] == 12.0);
 
}

static void zrowcatsTest(void) {
  int i = 0;
  doubleComplex output[2];

  /*
  ** [ 42+25i 51+38i ]
  */
  zrowcats(DoubleComplex(42.0, 25.0),
	   DoubleComplex(51.0, 38.0),
	   output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e + %e i\n", i, zreals(output[i]), zimags(output[i]));
  }
  assert(zreals(output[0]) == 42.0);
  assert(zimags(output[0]) == 25.0);
  assert(zreals(output[1]) == 51.0);
  assert(zimags(output[1]) == 38.0);

  /*
  ** [ PI+PI/2i PI/2+PIi ]
  */
  zrowcats(DoubleComplex(DPI, DPI/2),
	   DoubleComplex(DPI/2, DPI),
	   output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e + %e i\n", i, zreals(output[i]), zimags(output[i]));
  }
  assert(zreals(output[0]) == DPI);
  assert(zimags(output[0]) == DPI / 2);
  assert(zreals(output[1]) == DPI / 2);
  assert(zimags(output[1]) == DPI);
}

static void zcolumncatsTest(void) {
  int i = 0;
  doubleComplex output[2];
  /*
  ** [ 42+25i 51+38i ]
  */
  zcolumncats(DoubleComplex(42.0, 25.0),
	   DoubleComplex(51.0, 38.0),
	   output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e + %e i\n", i, zreals(output[i]), zimags(output[i]));
  }
  assert(zreals(output[0]) == 42.0);
  assert(zimags(output[0]) == 25.0);
  assert(zreals(output[1]) == 51.0);
  assert(zimags(output[1]) == 38.0);

  /*
  ** [ PI+PI/2i PI/2+PIi ]
  */
  zcolumncats(DoubleComplex(DPI, DPI/2),
	   DoubleComplex(DPI/2, DPI),
	   output);
  for (i = 0 ; i < 2 ; ++i) {
    printf("output[%d] = %e + %e i\n", i, zreals(output[i]), zimags(output[i]));
  }
  assert(zreals(output[0]) == DPI);
  assert(zimags(output[0]) == DPI / 2);
  assert(zreals(output[1]) == DPI / 2);
  assert(zimags(output[1]) == DPI);

}

static void zcolumncataTest(void) {
  int i = 0;
  double in1_real[12] = {0.0 , 1.0 , 2.0 , 3.0 , 4.0 , 5.0 , 6.0 , 7.0 , 8.0 , 9.0 , 10.0 , 11.0};
  double in1_imag[12] = {11.0 , 10.0 , 9.0 , 8.0 , 7.0 , 6.0 , 5.0 , 4.0 , 3.0 , 2.0 , 1.0 , 0.0};
  doubleComplex *in1 = DoubleComplexMatrix(in1_real, in1_imag, 12);
  double in2_real[1] = {12.0};
  double in2_imag[1] = {-1.0};
  doubleComplex *in2_1_1 = DoubleComplexMatrix(in2_real, in2_imag, 1);
  doubleComplex out_1_13[13];

  double in2_2_2_real[4] = {DPI, DPI / 2, DPI / 3, DPI / 4};
  double in2_2_2_imag[4] = {DPI / 4, DPI / 3, DPI / 2, DPI};
  doubleComplex *in2_2_2 = DoubleComplexMatrix(in2_2_2_real, in2_2_2_imag, 4);
  doubleComplex out_2_8[16];

  /*
  **  columncat( [ 0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 ], [ 12.0 ] )
  ** [ 0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0 ]
  */
  zcolumncata(in1, 1, 12, in2_1_1, 1, 1, out_1_13);
  for (i = 0 ; i < 13 ; ++i) {
    printf("out_1_13[%d] = %e + %e i\n", i, zreals(out_1_13[i]), zimags(out_1_13[i]));
    assert(zreals(out_1_13[i]) == (double) i);
    assert(zimags(out_1_13[i]) == 11.0 - (double) i);
  }

  /*
  ** [ 0 2 4 6 8 10 ] , [ PI   PI/3 ] => [ 0 2 4 6 8 10 PI   PI/3 ]
  ** [ 1 3 5 7 9 11 ]   [ PI/2 PI/4]     [ 1 3 5 7 9 11 PI/2 PI/4 ]
  */
  zcolumncata(in1, 2, 6, in2_2_2, 2, 2, out_2_8);
  for (i = 0 ; i < 16 ; ++i) {
    printf("out_2_8[%d] = %e + %e i\n", i, zreals(out_2_8[i]), zimags(out_2_8[i]));
  }
  for (i = 0 ; i < 12 ; ++i) {
    assert(zreals(out_2_8[i]) == zreals(in1[i]));
    assert(zimags(out_2_8[i]) == zimags(in1[i]));
  }
  for (i = 0 ; i < 4 ; ++i) {
    assert(zreals(out_2_8[i + 12]) == zreals(in2_2_2[i]));
    assert(zimags(out_2_8[i + 12]) == zimags(in2_2_2[i]));
  }

}

static void   zrowcataTest() {
  int i = 0;
  double in1_real[6] = {1.0 , 2.0, 5.0 , 6.0, 9.0 , 10.0};
  double in1_imag[6] = {1.0 , 2.0, 5.0 , 6.0, 9.0 , 10.0};
  doubleComplex *in1 = DoubleComplexMatrix(in1_real, in1_imag, 6);
  double in2_real[6] = {3.0 , 4.0 , 7.0 , 8.0 , 11.0 , 12.0};
  double in2_imag[6] = {3.0 , 4.0 , 7.0 , 8.0 , 11.0 , 12.0};
  doubleComplex *in2 = DoubleComplexMatrix(in2_real, in2_imag, 6);
  doubleComplex out_4_3[12];
  doubleComplex out_6_2[12];
  /*
  ** [ 1 5 9  ] ; [ 3 7 11 ] => [ 1 5 9  ]
  ** [ 2 6 10 ]   [ 4 8 12 ]	[ 2 6 10 ]
  **				[ 3 7 11 ]
  **				[ 4 8 12 ]
  */
  zrowcata(in1, 2, 3, in2, 2, 3, out_4_3);
  for (i = 0 ; i < 12 ; ++i) {
    printf("out_4_3[%d] = %e + %e i\n", i, zreals(out_4_3[i]), zimags(out_4_3[i]));
    assert(zreals(out_4_3[i]) == (double) i + 1.0);
    assert(zimags(out_4_3[i]) == (double) i + 1.0);
  }

  /*
  ** [ 1 6  ] ; [ 3 8  ]
  ** [ 2 9  ]   [ 4 11 ]
  ** [ 5 10 ]   [ 7 12 ]
  */
  zrowcata(in1, 3, 2, in2, 3, 2, out_6_2);
 for (i = 0 ; i < 12 ; ++i) {
   printf("out_6_2[%d] = %e + %e i\n", i, zreals(out_6_2[i]), zimags(out_6_2[i]));
 }
 assert(zreals(out_6_2[0]) == 1.0);
 assert(zimags(out_6_2[0]) == 1.0);
 assert(zreals(out_6_2[1]) == 2.0);
 assert(zimags(out_6_2[1]) == 2.0);
 assert(zreals(out_6_2[2]) == 5.0);
 assert(zimags(out_6_2[2]) == 5.0);
 assert(zreals(out_6_2[3]) == 3.0);
 assert(zimags(out_6_2[3]) == 3.0);
 assert(zreals(out_6_2[4]) == 4.0);
 assert(zimags(out_6_2[4]) == 4.0);
 assert(zreals(out_6_2[5]) == 7.0);
 assert(zimags(out_6_2[5]) == 7.0);
 assert(zreals(out_6_2[6]) == 6.0);
 assert(zimags(out_6_2[6]) == 6.0);
 assert(zreals(out_6_2[7]) == 9.0);
 assert(zimags(out_6_2[7]) == 9.0);
 assert(zreals(out_6_2[8]) == 10.0);
 assert(zimags(out_6_2[8]) == 10.0);
 assert(zreals(out_6_2[9]) == 8.0);
 assert(zimags(out_6_2[9]) == 8.0);
 assert(zreals(out_6_2[10]) == 11.0);
 assert(zimags(out_6_2[10]) == 11.0);
 assert(zreals(out_6_2[11]) == 12.0);
 assert(zimags(out_6_2[11]) == 12.0);
}

int main(void) {
  drowcatsTest();
  dcolumncatsTest();
  drowcataTest();
  dcolumncataTest();
  zrowcatsTest();
  zcolumncatsTest();
  zrowcataTest();
  zcolumncataTest();
  return 0;
}
