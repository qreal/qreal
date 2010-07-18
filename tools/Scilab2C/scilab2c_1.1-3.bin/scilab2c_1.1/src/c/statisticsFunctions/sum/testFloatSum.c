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
#include <math.h>
#include "sum.h"

/* #define LOCAL_DEBUG */

static int ssumsTest(void) {
  float value1 = 3.0f;
  float value2 = 1.123456789f;

  printf("\n>>>> Sum Float Scalar Test\n");
  assert(ssums(value1) == 3.0f);
  assert(ssums(value2) == 1.123456789f);

  return 0;
}

static int ssumaTest(void) {
  float table1[3] = {3.0f, 3.0f, 3.0f};
  float table2[5] = {9.186784563f,
		     9.186784563f,
		     9.186784563f,
		     9.186784563f,
		     9.186784563f};

  printf("\n>>>> Sum Float Array Test\n");
  assert(ssuma(table1, 3) == 9.0f);
  assert((ssuma(table2, 5) - (9.186784f + 9.186784f + 9.186784f + 9.186784f + 9.186784f)) / ssuma(table2, 5) < 1e-6);
  return 0;
}

static int scolumnsumaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float table2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  float columnSummedTable1_3_3[3] = {0};
  float columnSummedTable1_1_9[1] = {0};
  float columnSummedTable1_9_1[9] = {0};
  float columnSummedTable2_2_5[2] = {0};
  float columnSummedTable2_5_2[5] = {0};

  printf("\n>>>> Column Sum Float Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 6 15 24 ]
    [ 7 8 9 ]
  */
  scolumnsuma(table1, 3, 3,columnSummedTable1_3_3);
  assert(columnSummedTable1_3_3[0] == 6.0f);
  assert(columnSummedTable1_3_3[1] == 15.0f);
  assert(columnSummedTable1_3_3[2] == 24.0f);

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  scolumnsuma(table1, 1, 9,columnSummedTable1_1_9);
  assert(columnSummedTable1_1_9[0] == 45.0f);

  /*
    [ 1 ]
    [ 2 ]
    [ 3 ]
    [ 4 ]
    [ 5 ] =>  [ 1 2 3 4 5 6 7 8 9 ]
    [ 6 ]
    [ 7 ]
    [ 8 ]
    [ 9 ]
  */
  scolumnsuma(table1, 9, 1,columnSummedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnSummedTable1_9_1[%d] = %e\n", i, columnSummedTable1_9_1[i]);
    assert(columnSummedTable1_9_1[i] == table1[i]);
  }

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 25 30 ]
  */
  scolumnsuma(table2, 2, 5,columnSummedTable2_2_5);
  assert(columnSummedTable2_2_5[0] == 25.0f);
  assert(columnSummedTable2_2_5[1] == 30.0f);
  for ( i = 0 ; i < 2 ; ++i) {
    printf("columnSummedTable2_2_5[%d] = %e\n", i, columnSummedTable2_2_5[i]);
  }

  /*
    [ 1  6 ]
    [ 2  7 ] => [ 7 9 11 13 15 ]
    [ 3  8 ]
    [ 4  9 ]
    [ 5 10 ]
  */
  scolumnsuma(table2, 5, 2,columnSummedTable2_5_2);
  assert(columnSummedTable2_5_2[0] == 7.0f);
  assert(columnSummedTable2_5_2[1] == 9.0f);
  assert(columnSummedTable2_5_2[2] == 11.0f);
  assert(columnSummedTable2_5_2[3] == 13.0f);
  assert(columnSummedTable2_5_2[4] == 15.0f);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnSummedTable2_5_2[%d] = %e\n", i, columnSummedTable2_5_2[i]);
  }

  return 0;
}

static int srowsumaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float table2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  float rowSummedTable1_3_3[3] = {0};
  float rowSummedTable1_1_9[9] = {0};
  float rowSummedTable1_9_1[1] = {0};
  float rowSummedTable2_2_5[5] = {0};
  float rowSummedTable2_5_2[2] = {0};

  printf("\n>>>> Row Sum Float Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 12 15 18 ]
    [ 7 8 9 ]
  */
  srowsuma(table1, 3, 3,rowSummedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowSummedTable1_3_3[%d] = %e\n", i, rowSummedTable1_3_3[i]);
  }
  assert(rowSummedTable1_3_3[0] == 12.0f);
  assert(rowSummedTable1_3_3[1] == 15.0f);
  assert(rowSummedTable1_3_3[2] == 18.0f);

  /*
    [ 1 ]
    [ 2 ]
    [ 3 ]
    [ 4 ]
    [ 5 ] =>  [ 1 2 3 4 5 6 7 8 9 ]
    [ 6 ]
    [ 7 ]
    [ 8 ]
    [ 9 ]
  */
  srowsuma(table1, 1, 9,rowSummedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowSummedTable1_1_9[%d] = %e\n", i, rowSummedTable1_1_9[i]);
    assert(rowSummedTable1_1_9[i] == table1[i]);
  }

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  srowsuma(table1, 9, 1,rowSummedTable1_9_1);
  assert(rowSummedTable1_9_1[0] == 45.0f);

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 3 7 11 15 19 ]
  */
  srowsuma(table2, 2, 5,rowSummedTable2_2_5);
  assert(rowSummedTable2_2_5[0] == 3.0f);
  assert(rowSummedTable2_2_5[1] == 7.0f);
  assert(rowSummedTable2_2_5[2] == 11.0f);
  assert(rowSummedTable2_2_5[3] == 15.0f);
  assert(rowSummedTable2_2_5[4] == 19.0f);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("rowSummedTable2_2_5[%d] = %e\n", i, rowSummedTable2_2_5[i]);
  }

  /*
    [ 1  6 ]
    [ 2  7 ] => [ 15 40 ]
    [ 3  8 ]
    [ 4  9 ]
    [ 5 10 ]
  */
  srowsuma(table2, 5, 2,rowSummedTable2_5_2);
  assert(rowSummedTable2_5_2[0] == 15.0f);
  assert(rowSummedTable2_5_2[1] == 40.0f);
  for ( i = 0 ; i < 2 ; ++i) {
    printf("rowSummedTable2_5_2[%d] = %e\n", i, rowSummedTable2_5_2[i]);
  }

  return 0;
}

static int csumsTest(void) {
  floatComplex value1 = FloatComplex(3.0f, 3.0f);
  floatComplex value2 = FloatComplex(1.123456789f, 1.123456789f);

  printf("\n>>>> Sum Float Complex Scalar Test\n");
  assert(creals(csums(value1)) == 3.0f);
  assert(cimags(csums(value1)) == 3.0f);
  assert(creals(csums(value2)) == 1.123456789f);
  assert(cimags(csums(value2)) == 1.123456789f);
  return 0;
}

static int csumaTest(void) {
  floatComplex value1 = FloatComplex(3.0f, 3.0f);
  floatComplex table1[3];
  floatComplex value2 = FloatComplex(9.186784563f,9.186784563f);
  floatComplex table2[5];

  printf("\n>>>> Sum Float Complex Array Test\n");
  table1[0] = value1;
  table1[1] = value1;
  table1[2] = value1;

  table2[0] = value2;
  table2[1] = value2;
  table2[2] = value2;
  table2[3] = value2;
  table2[4] = value2;

#ifdef LOCAL_DEBUG
  printf("--------\n");
  printf("%e\n", cimags(csuma(table1, 3)));
  printf("%e\n", creals(csuma(table2, 5)));
  printf("%e\n", cimags(csuma(table2, 5)));
  printf("%e\n", creals(csuma(table2, 5)));
  printf("%e\n", cimags(csuma(table2, 5)));
  printf("--------\n");
#endif
  assert(creals(csuma(table1, 3)) == 9.0f);
  assert(cimags(csuma(table1, 3)) == 9.0f);
  assert(creals(csuma(table2, 5)) == 9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f);
  assert(cimags(csuma(table2, 5)) == 9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f);

  return 0;
}

static int crowsumaTest(void) {
  int i = 0;
  floatComplex in[12];
  floatComplex rowSummedIn_4_3[3];
  floatComplex rowSummedIn_3_4[4];
  floatComplex rowSummedIn_6_2[2];
  floatComplex rowSummedIn_2_6[6];
  floatComplex rowSummedIn_1_12[12];
  floatComplex rowSummedIn_12_1[1];

  printf("\n>>>> Row Sum Float Complex Array Test\n");
  /* Init input var */
  for (i = 0 ; i < 12 ; ++i)
    {
      in[i] = FloatComplex((float) i / 10.0f, (11.0f - (float) i) / 10.0f);
    }

  /*
    [ 1.1i     0.4+0.7i 0.8+0.3i ]
    [ 0.1+i    0.5+0.6i 0.9+0.2i ] => [ 0.6+3.8i 2.2+2.2i 3.8+0.6i ]
    [ 0.2+0.9i 0.6+0.5i 1+0.1i   ]
    [ 0.3+0.8i 0.7+0.4i 1.1      ]
  */
  crowsuma(in, 4, 3, rowSummedIn_4_3);
  for (i = 0 ; i < 3 ; ++i) {
    printf("rowSummedIn_4_3[%d] = %e + %ei\n", i, creals(rowSummedIn_4_3[i]), cimags(rowSummedIn_4_3[i]));
  }
  assert(creals(rowSummedIn_4_3[0]) == 0.6f && cimags(rowSummedIn_4_3[0]) == 3.8f);
  assert(creals(rowSummedIn_4_3[1]) == 2.2f && cimags(rowSummedIn_4_3[1]) == 2.2f);
  assert(fabs(creals(rowSummedIn_4_3[2]) - 3.8f) < 1e-06);
  assert(cimags(rowSummedIn_4_3[2]) == 0.6f);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ 0.3+3.i 1.2+2.1i 2.1+1.2i 3.+0.3i]
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]
  */
  crowsuma(in, 3, 4, rowSummedIn_3_4);
  for (i = 0 ; i < 4 ; ++i) {
    printf("rowSummedIn_3_4[%d] = %e + %ei\n", i, creals(rowSummedIn_3_4[i]), cimags(rowSummedIn_3_4[i]));
  }
  assert(creals(rowSummedIn_3_4[0]) == 0.3f && cimags(rowSummedIn_3_4[0]) == 3.0f);
  assert(creals(rowSummedIn_3_4[1]) == 1.2f && cimags(rowSummedIn_3_4[1]) == 2.1f);
  assert(creals(rowSummedIn_3_4[2]) == 2.1f && cimags(rowSummedIn_3_4[2]) == 1.2f);
  assert(creals(rowSummedIn_3_4[3]) == 3.0f && cimags(rowSummedIn_3_4[3]) == 0.3f);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 1.5+5.1i 5.1+1.5i ]
    [ 0.3+0.8i 0.9+0.2i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  crowsuma(in, 6, 2, rowSummedIn_6_2);
  for (i = 0 ; i < 2 ; ++i) {
    printf("rowSummedIn_6_2[%d] = %e + %ei\n", i, creals(rowSummedIn_6_2[i]), cimags(rowSummedIn_6_2[i]));
  }
  assert(creals(rowSummedIn_6_2[0]) == 1.5f && cimags(rowSummedIn_6_2[0]) == 5.1f);
  assert(creals(rowSummedIn_6_2[1]) == 5.1f);
  assert(fabs(cimags(rowSummedIn_6_2[1]) - 1.5f) < 1e-06);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ 0.1+2.1i 0.5+1.7i 0.9+1.3i 1.3+0.9i 1.7+0.5i 2.1+0.1i ]
  */
  crowsuma(in, 2, 6, rowSummedIn_2_6);
  for (i = 0 ; i < 6 ; ++i) {
    printf("rowSummedIn_2_6[%d] = %e + %ei\n", i, creals(rowSummedIn_2_6[i]), cimags(rowSummedIn_2_6[i]));
  }
  assert(creals(rowSummedIn_2_6[0]) == 0.1f && cimags(rowSummedIn_2_6[0]) == 2.1f);
  assert(creals(rowSummedIn_2_6[1]) == 0.5f && cimags(rowSummedIn_2_6[1]) == 1.7f);
  assert(creals(rowSummedIn_2_6[2]) == 0.9f && cimags(rowSummedIn_2_6[2]) == 1.3f);
  assert(creals(rowSummedIn_2_6[3]) == 1.3f && cimags(rowSummedIn_2_6[3]) == 0.9f);
  assert(creals(rowSummedIn_2_6[4]) == 1.7f && cimags(rowSummedIn_2_6[4]) == 0.5f);
  assert(creals(rowSummedIn_2_6[5]) == 2.1f && cimags(rowSummedIn_2_6[5]) == 0.1f);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
  */
  crowsuma(in, 1, 12, rowSummedIn_1_12);
  for (i = 0 ; i < 12 ; ++i) {
    printf("rowSummedIn_1_12[%d] = %e + %ei\n", i, creals(rowSummedIn_1_12[i]), cimags(rowSummedIn_1_12[i]));
    assert(creals(rowSummedIn_1_12[i]) == creals(in[i]) && cimags(rowSummedIn_1_12[i]) == cimags(in[i]));
  }

  /*
    [ 1.1i     ]
    [ 0.1+i    ]
    [ 0.2+0.9i ]
    [ 0.3+0.8i ]
    [ 0.4+0.7i ]
    [ 0.5+0.6i ]
    [ 0.6+0.5i ] => [ 6.6+6.6i ]
    [ 0.7+0.4i ]
    [ 0.8+0.3i ]
    [ 0.9+0.2i ]
    [ 1+0.1i   ]
    [ 1.1      ]
  */
  crowsuma(in, 12, 1, rowSummedIn_12_1);
  printf("rowSummedIn_12_1[%d] = %e + %ei\n", 0, creals(rowSummedIn_12_1[0]), cimags(rowSummedIn_12_1[0]));
  assert(creals(rowSummedIn_12_1[0]) == 6.6f && cimags(rowSummedIn_12_1[0]) == 6.6f);

  return 0;
}

static int ccolumnsumaTest(void) {
  int i = 0;
  floatComplex in[12];
  floatComplex columnSummedIn_4_3[4];
  floatComplex columnSummedIn_3_4[3];
  floatComplex columnSummedIn_6_2[6];
  floatComplex columnSummedIn_2_6[2];
  floatComplex columnSummedIn_1_12[1];
  floatComplex columnSummedIn_12_1[12];

  printf("\n>>>> ColumnSum Float Complex Array Test\n");
  /* Init input var */
  for (i = 0 ; i < 12 ; ++i)
    {
      in[i] = FloatComplex((float) i / 10.0f, (11.0f - (float) i) / 10.0f);
    }

  /*
    [ 1.1i     0.4+0.7i 0.8+0.3i ]
    [ 0.1+i    0.5+0.6i 0.9+0.2i ] => [ 1.2+2.1i 1.5+1.8i 1.8+1.5i 2.1+1.2i ]
    [ 0.2+0.9i 0.6+0.5i 1+0.1i   ]
    [ 0.3+0.8i 0.7+0.4i 1.1      ]
  */
  ccolumnsuma(in, 4, 3, columnSummedIn_4_3);
  for (i = 0 ; i < 4 ; ++i) {
    printf("columnSummedIn_4_3[%d] = %e + %ei\n", i, creals(columnSummedIn_4_3[i]), cimags(columnSummedIn_4_3[i]));
  }
  assert(creals(columnSummedIn_4_3[0]) == 1.2f && cimags(columnSummedIn_4_3[0]) == 2.1f);
  assert(creals(columnSummedIn_4_3[1]) == 1.5f);
  assert(fabs(cimags(columnSummedIn_4_3[1]) - 1.8f) < 1e-06);
  assert(creals(columnSummedIn_4_3[2]) == 1.8f && cimags(columnSummedIn_4_3[2]) == 1.5f);
  assert(creals(columnSummedIn_4_3[3]) == 2.1f && cimags(columnSummedIn_4_3[3]) == 1.2f);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ 1.8+2.6i 2.2+2.2i 2.6+1.8i ]
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]
  */
  ccolumnsuma(in, 3, 4, columnSummedIn_3_4);
  for (i = 0 ; i < 3 ; ++i) {
    printf("columnSummedIn_3_4[%d] = %e + %ei\n", i, creals(columnSummedIn_3_4[i]), cimags(columnSummedIn_3_4[i]));
  }
  assert(creals(columnSummedIn_3_4[0]) == 1.8f);
  assert(fabs(cimags(columnSummedIn_3_4[0]) - 2.6f) < 1e-06);
  assert(creals(columnSummedIn_3_4[1]) == 2.2f && cimags(columnSummedIn_3_4[1]) == 2.2f);
  assert(creals(columnSummedIn_3_4[2]) == 2.6f && cimags(columnSummedIn_3_4[2]) == 1.8f);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 0.6+1.6i 0.8+1.4i 1+1.2i 1.2+i 1.4+0.8i 1.6+0.6i ]
    [ 0.3+0.8i 0.9+0.2i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  ccolumnsuma(in, 6, 2, columnSummedIn_6_2);
  for (i = 0 ; i < 6 ; ++i) {
    printf("columnSummedIn_6_2[%d] = %e + %ei\n", i, creals(columnSummedIn_6_2[i]), cimags(columnSummedIn_6_2[i]));
  }
  assert(creals(columnSummedIn_6_2[0]) == 0.6f && cimags(columnSummedIn_6_2[0]) == 1.6f);
  assert(creals(columnSummedIn_6_2[1]) == 0.8f && cimags(columnSummedIn_6_2[1]) == 1.4f);
  assert(creals(columnSummedIn_6_2[2]) == 1.0f && cimags(columnSummedIn_6_2[2]) == 1.2f);
  assert(creals(columnSummedIn_6_2[3]) == 1.2f && cimags(columnSummedIn_6_2[3]) == 1.0f);
  assert(creals(columnSummedIn_6_2[4]) == 1.4f && cimags(columnSummedIn_6_2[4]) == 0.8f);
  assert(creals(columnSummedIn_6_2[5]) == 1.6f && cimags(columnSummedIn_6_2[5]) == 0.6f);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ 3+3.6i 3.6+3i ]
  */
  ccolumnsuma(in, 2, 6, columnSummedIn_2_6);
  for (i = 0 ; i < 2 ; ++i) {
    printf("columnSummedIn_2_6[%d] = %e + %ei\n", i, creals(columnSummedIn_2_6[i]), cimags(columnSummedIn_2_6[i]));
  }
  assert(creals(columnSummedIn_2_6[0]) == 3.0f && cimags(columnSummedIn_2_6[0]) == 3.6f);
  assert(creals(columnSummedIn_2_6[1]) == 3.6f);
  assert(fabs(cimags(columnSummedIn_2_6[1]) -3.0f) < 1e-06);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 6.6+6.6i ]
  */
  ccolumnsuma(in, 1, 12, columnSummedIn_1_12);
  printf("columnSummedIn_1_12[%d] = %e + %ei\n", 0, creals(columnSummedIn_1_12[0]), cimags(columnSummedIn_1_12[0]));
  assert(creals(columnSummedIn_1_12[0]) == 6.6f && cimags(columnSummedIn_1_12[0]) == 6.6f);


  /*
    [ 1.1i     ]
    [ 0.1+i    ]
    [ 0.2+0.9i ]
    [ 0.3+0.8i ]
    [ 0.4+0.7i ]
    [ 0.5+0.6i ]
    [ 0.6+0.5i ] =>     [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    [ 0.7+0.4i ]
    [ 0.8+0.3i ]
    [ 0.9+0.2i ]
    [ 1+0.1i   ]
    [ 1.1      ]
  */
  ccolumnsuma(in, 12, 1, columnSummedIn_12_1);
 for (i = 0 ; i < 12 ; ++i) {
   printf("columnSummedIn_12_1[%d] = %e + %ei\n", i, creals(columnSummedIn_12_1[i]), cimags(columnSummedIn_12_1[i]));
   assert(creals(columnSummedIn_12_1[i]) == creals(in[i]) && cimags(columnSummedIn_12_1[i]) == cimags(in[i]));
 }

  return 0;
}

static int testSum(void) {

  ssumsTest();
  ssumaTest();
  srowsumaTest();
  scolumnsumaTest();
  csumsTest();
  csumaTest();
  crowsumaTest();
  ccolumnsumaTest();

  return 0;
}

int main(void) {
  assert(testSum() == 0);
  return 0;
}
