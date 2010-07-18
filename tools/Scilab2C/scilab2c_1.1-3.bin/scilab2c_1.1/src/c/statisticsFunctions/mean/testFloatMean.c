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
#include "mean.h"

/* #define LOCAL_DEBUG */

static int smeansTest(void) {
  float value1 = 3.0f;
  float value2 = 1.123456789f;

  printf("\n>>>> Mean Float Scalar Test\n");
  assert(smeans(value1) == 3.0f);
  assert(smeans(value2) == 1.123456789f);

  return 0;
}

static int smeanaTest(void) {
  float table1[3] = {3.0f, 3.0f, 3.0f};
  float table2[5] = {9.186784563f,
		     9.186784563f,
		     9.186784563f,
		     9.186784563f,
		     9.186784563f};

  printf("\n>>>> Mean Float Array Test\n");
  assert(smeana(table1, 3) == 9.0f / 3.0f);
  assert((smeana(table2, 5) - (9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f) / 5.0f) / smeana(table2, 5) < 1e-6);
  return 0;
}

static int scolumnmeanaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float table2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  float columnMeanmedTable1_3_3[3] = {0};
  float columnMeanmedTable1_1_9[1] = {0};
  float columnMeanmedTable1_9_1[9] = {0};
  float columnMeanmedTable2_2_5[2] = {0};
  float columnMeanmedTable2_5_2[5] = {0};

  printf("\n>>>> Column Mean Float Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 6 15 24 ]
    [ 7 8 9 ]
  */
  scolumnmeana(table1, 3, 3,columnMeanmedTable1_3_3);
  assert(columnMeanmedTable1_3_3[0] == 6.0f / 3.0f);
  assert(columnMeanmedTable1_3_3[1] == 15.0f / 3.0f);
  assert(columnMeanmedTable1_3_3[2] == 24.0f / 3.0f);

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  scolumnmeana(table1, 1, 9,columnMeanmedTable1_1_9);
  assert(columnMeanmedTable1_1_9[0] == 45.0f / 9.0f);

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
  scolumnmeana(table1, 9, 1,columnMeanmedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnMeanmedTable1_9_1[%d] = %e\n", i, columnMeanmedTable1_9_1[i]);
    assert(columnMeanmedTable1_9_1[i] == table1[i]);
  }

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 25 30 ]
  */
  scolumnmeana(table2, 2, 5,columnMeanmedTable2_2_5);
  assert(columnMeanmedTable2_2_5[0] == 25.0f / 5.0f);
  assert(columnMeanmedTable2_2_5[1] == 30.0f / 5.0f);
  for ( i = 0 ; i < 2 ; ++i) {
    printf("columnMeanmedTable2_2_5[%d] = %e\n", i, columnMeanmedTable2_2_5[i]);
  }

  /*
    [ 1  6 ]
    [ 2  7 ] => [ 7 9 11 13 15 ]
    [ 3  8 ]
    [ 4  9 ]
    [ 5 10 ]
  */
  scolumnmeana(table2, 5, 2,columnMeanmedTable2_5_2);
  assert(columnMeanmedTable2_5_2[0] == 7.0f / 2.0f);
  assert(columnMeanmedTable2_5_2[1] == 9.0f / 2.0f);
  assert(columnMeanmedTable2_5_2[2] == 11.0f / 2.0f);
  assert(columnMeanmedTable2_5_2[3] == 13.0f / 2.0f);
  assert(columnMeanmedTable2_5_2[4] == 15.0f / 2.0f);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnMeanmedTable2_5_2[%d] = %e\n", i, columnMeanmedTable2_5_2[i]);
  }

  return 0;
}

static int srowmeanaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float table2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  float rowMeanmedTable1_3_3[3] = {0};
  float rowMeanmedTable1_1_9[9] = {0};
  float rowMeanmedTable1_9_1[1] = {0};
  float rowMeanmedTable2_2_5[5] = {0};
  float rowMeanmedTable2_5_2[2] = {0};

  printf("\n>>>> Row Mean Float Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 12 15 18 ]
    [ 7 8 9 ]
  */
  srowmeana(table1, 3, 3,rowMeanmedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowMeanmedTable1_3_3[%d] = %e\n", i, rowMeanmedTable1_3_3[i]);
  }
  assert(rowMeanmedTable1_3_3[0] == 12.0f / 3.0f);
  assert(rowMeanmedTable1_3_3[1] == 15.0f / 3.0f);
  assert(rowMeanmedTable1_3_3[2] == 18.0f / 3.0f);

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
  srowmeana(table1, 1, 9,rowMeanmedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowMeanmedTable1_1_9[%d] = %e\n", i, rowMeanmedTable1_1_9[i]);
    assert(rowMeanmedTable1_1_9[i] == table1[i]);
  }

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  srowmeana(table1, 9, 1,rowMeanmedTable1_9_1);
  assert(rowMeanmedTable1_9_1[0] == 45.0f / 9.0f);

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 3 7 11 15 19 ]
  */
  srowmeana(table2, 2, 5,rowMeanmedTable2_2_5);
  assert(rowMeanmedTable2_2_5[0] == 3.0f / 2.0f);
  assert(rowMeanmedTable2_2_5[1] == 7.0f / 2.0f);
  assert(rowMeanmedTable2_2_5[2] == 11.0f / 2.0f);
  assert(rowMeanmedTable2_2_5[3] == 15.0f / 2.0f);
  assert(rowMeanmedTable2_2_5[4] == 19.0f / 2.0f);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("rowMeanmedTable2_2_5[%d] = %e\n", i, rowMeanmedTable2_2_5[i]);
  }

  /*
    [ 1  6 ]
    [ 2  7 ] => [ 15 40 ]
    [ 3  8 ]
    [ 4  9 ]
    [ 5 10 ]
  */
  srowmeana(table2, 5, 2,rowMeanmedTable2_5_2);
  assert(rowMeanmedTable2_5_2[0] == 15.0f / 5.0f);
  assert(rowMeanmedTable2_5_2[1] == 40.0f / 5.0f);
  for ( i = 0 ; i < 2 ; ++i) {
    printf("rowMeanmedTable2_5_2[%d] = %e\n", i, rowMeanmedTable2_5_2[i]);
  }

  return 0;
}

static int cmeansTest(void) {
  floatComplex value1 = FloatComplex(3.0f, 3.0f);
  floatComplex value2 = FloatComplex(1.123456789f, 1.123456789f);

  printf("\n>>>> Mean Float Complex Scalar Test\n");
  assert(creals(cmeans(value1)) == 3.0f);
  assert(cimags(cmeans(value1)) == 3.0f);
  assert(creals(cmeans(value2)) == 1.123456789f);
  assert(cimags(cmeans(value2)) == 1.123456789f);
  return 0;
}

static int cmeanaTest(void) {
  floatComplex value1 = FloatComplex(3.0f, 3.0f);
  floatComplex table1[3];
  floatComplex value2 = FloatComplex(9.186784563f,9.186784563f);
  floatComplex table2[5];

  printf("\n>>>> Mean Float Complex Array Test\n");
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
  printf("%e\n", cimags(cmeana(table1, 3)));
  printf("%e\n", creals(cmeana(table2, 5)));
  printf("%e\n", cimags(cmeana(table2, 5)));
  printf("%e\n", creals(cmeana(table2, 5)));
  printf("%e\n", cimags(cmeana(table2, 5)));
  printf("--------\n");
#endif
  assert(creals(cmeana(table1, 3)) == 9.0f / 3.0f);
  assert(cimags(cmeana(table1, 3)) == 9.0f / 3.0f);
  assert(creals(cmeana(table2, 5)) == (9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f) / 5.0f);
  assert(cimags(cmeana(table2, 5)) == (9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f + 9.186784563f) / 5.0f);

  return 0;
}

static int crowmeanaTest(void) {
  int i = 0;
  floatComplex in[12];
  floatComplex rowMeanmedIn_4_3[3];
  floatComplex rowMeanmedIn_3_4[4];
  floatComplex rowMeanmedIn_6_2[2];
  floatComplex rowMeanmedIn_2_6[6];
  floatComplex rowMeanmedIn_1_12[12];
  floatComplex rowMeanmedIn_12_1[1];

  printf("\n>>>> Row Mean Float Complex Array Test\n");
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
  crowmeana(in, 4, 3, rowMeanmedIn_4_3);
  for (i = 0 ; i < 3 ; ++i) {
    printf("rowMeanmedIn_4_3[%d] = %e + %ei\n", i, creals(rowMeanmedIn_4_3[i]), cimags(rowMeanmedIn_4_3[i]));
  }
  assert(creals(rowMeanmedIn_4_3[0]) == 0.6f / 4.0f && cimags(rowMeanmedIn_4_3[0]) == 3.8f / 4.0f);
  assert(creals(rowMeanmedIn_4_3[1]) == 2.2f / 4.0f && cimags(rowMeanmedIn_4_3[1]) == 2.2f / 4.0f);
  assert(fabs(creals(rowMeanmedIn_4_3[2]) - 3.8f / 4.0f) < 1e-06);
  assert(cimags(rowMeanmedIn_4_3[2]) == 0.6f / 4.0f);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ 0.3+3.i 1.2+2.1i 2.1+1.2i 3.+0.3i]
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]
  */
  crowmeana(in, 3, 4, rowMeanmedIn_3_4);
  for (i = 0 ; i < 4 ; ++i) {
    printf("rowMeanmedIn_3_4[%d] = %e + %ei\n", i, creals(rowMeanmedIn_3_4[i]), cimags(rowMeanmedIn_3_4[i]));
  }
  assert(creals(rowMeanmedIn_3_4[0]) == 0.3f / 3.0f && cimags(rowMeanmedIn_3_4[0]) == 3.0f / 3.0f);
  assert(creals(rowMeanmedIn_3_4[1]) == 1.2f / 3.0f && cimags(rowMeanmedIn_3_4[1]) == 2.1f / 3.0f);
  assert(creals(rowMeanmedIn_3_4[2]) == 2.1f / 3.0f && cimags(rowMeanmedIn_3_4[2]) == 1.2f / 3.0f);
  assert(creals(rowMeanmedIn_3_4[3]) == 3.0f / 3.0f && cimags(rowMeanmedIn_3_4[3]) == 0.3f / 3.0f);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 1.5+5.1i 5.1+1.5i ]
    [ 0.3+0.8i 0.9+0.2i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  crowmeana(in, 6, 2, rowMeanmedIn_6_2);
  for (i = 0 ; i < 2 ; ++i) {
    printf("rowMeanmedIn_6_2[%d] = %e + %ei\n", i, creals(rowMeanmedIn_6_2[i]), cimags(rowMeanmedIn_6_2[i]));
  }
  assert(creals(rowMeanmedIn_6_2[0]) == 1.5f / 6.0f && cimags(rowMeanmedIn_6_2[0]) == 5.1f / 6.0f);
  assert(creals(rowMeanmedIn_6_2[1]) == 5.1f / 6.0f);
  assert(fabs(cimags(rowMeanmedIn_6_2[1]) - 1.5f / 6.0f) < 1e-06);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ 0.1+2.1i 0.5+1.7i 0.9+1.3i 1.3+0.9i 1.7+0.5i 2.1+0.1i ]
  */
  crowmeana(in, 2, 6, rowMeanmedIn_2_6);
  for (i = 0 ; i < 6 ; ++i) {
    printf("rowMeanmedIn_2_6[%d] = %e + %ei\n", i, creals(rowMeanmedIn_2_6[i]), cimags(rowMeanmedIn_2_6[i]));
  }
  assert(creals(rowMeanmedIn_2_6[0]) == 0.1f / 2.0f && cimags(rowMeanmedIn_2_6[0]) == 2.1f / 2.0f);
  assert(creals(rowMeanmedIn_2_6[1]) == 0.5f / 2.0f && cimags(rowMeanmedIn_2_6[1]) == 1.7f / 2.0f);
  assert(creals(rowMeanmedIn_2_6[2]) == 0.9f / 2.0f && cimags(rowMeanmedIn_2_6[2]) == 1.3f / 2.0f);
  assert(creals(rowMeanmedIn_2_6[3]) == 1.3f / 2.0f && cimags(rowMeanmedIn_2_6[3]) == 0.9f / 2.0f);
  assert(creals(rowMeanmedIn_2_6[4]) == 1.7f / 2.0f && cimags(rowMeanmedIn_2_6[4]) == 0.5f / 2.0f);
  assert(creals(rowMeanmedIn_2_6[5]) == 2.1f / 2.0f && cimags(rowMeanmedIn_2_6[5]) == 0.1f / 2.0f);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
  */
  crowmeana(in, 1, 12, rowMeanmedIn_1_12);
  for (i = 0 ; i < 12 ; ++i) {
    printf("rowMeanmedIn_1_12[%d] = %e + %ei\n", i, creals(rowMeanmedIn_1_12[i]), cimags(rowMeanmedIn_1_12[i]));
    assert(creals(rowMeanmedIn_1_12[i]) == creals(in[i]) && cimags(rowMeanmedIn_1_12[i]) == cimags(in[i]));
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
  crowmeana(in, 12, 1, rowMeanmedIn_12_1);
  printf("rowMeanmedIn_12_1[%d] = %e + %ei\n", 0, creals(rowMeanmedIn_12_1[0]), cimags(rowMeanmedIn_12_1[0]));
  assert(creals(rowMeanmedIn_12_1[0]) == 6.6f / 12.0f && cimags(rowMeanmedIn_12_1[0]) == 6.6f / 12.0f);

  return 0;
}

static int ccolumnmeanaTest(void) {
  int i = 0;
  floatComplex in[12];
  floatComplex columnMeanmedIn_4_3[4];
  floatComplex columnMeanmedIn_3_4[3];
  floatComplex columnMeanmedIn_6_2[6];
  floatComplex columnMeanmedIn_2_6[2];
  floatComplex columnMeanmedIn_1_12[1];
  floatComplex columnMeanmedIn_12_1[12];

  printf("\n>>>> ColumnMean Float Complex Array Test\n");
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
  ccolumnmeana(in, 4, 3, columnMeanmedIn_4_3);
  for (i = 0 ; i < 4 ; ++i) {
    printf("columnMeanmedIn_4_3[%d] = %e + %ei\n", i, creals(columnMeanmedIn_4_3[i]), cimags(columnMeanmedIn_4_3[i]));
  }
  assert(creals(columnMeanmedIn_4_3[0]) == 1.2f / 3.0f && cimags(columnMeanmedIn_4_3[0]) == 2.1f / 3.0f);
  assert(creals(columnMeanmedIn_4_3[1]) == 1.5f / 3.0f);
  assert(fabs(cimags(columnMeanmedIn_4_3[1]) - 1.8f / 3.0f) < 1e-06);
  assert(creals(columnMeanmedIn_4_3[2]) == 1.8f / 3.0f && cimags(columnMeanmedIn_4_3[2]) == 1.5f / 3.0f);
  assert(creals(columnMeanmedIn_4_3[3]) == 2.1f / 3.0f && cimags(columnMeanmedIn_4_3[3]) == 1.2f / 3.0f);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ 1.8+2.6i 2.2+2.2i 2.6+1.8i ]
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]
  */
  ccolumnmeana(in, 3, 4, columnMeanmedIn_3_4);
  for (i = 0 ; i < 3 ; ++i) {
    printf("columnMeanmedIn_3_4[%d] = %e + %ei\n", i, creals(columnMeanmedIn_3_4[i]), cimags(columnMeanmedIn_3_4[i]));
  }
  assert(creals(columnMeanmedIn_3_4[0]) == 1.8f / 4.0f);
  assert(fabs(cimags(columnMeanmedIn_3_4[0]) - 2.6f / 4.0f) < 1e-06);
  assert(creals(columnMeanmedIn_3_4[1]) == 2.2f / 4.0f && cimags(columnMeanmedIn_3_4[1]) == 2.2f / 4.0f);
  assert(creals(columnMeanmedIn_3_4[2]) == 2.6f / 4.0f && cimags(columnMeanmedIn_3_4[2]) == 1.8f / 4.0f);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 0.6+1.6i 0.8+1.4i 1+1.2i 1.2+i 1.4+0.8i 1.6+0.6i ]
    [ 0.3+0.8i 0.9+0.2i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  ccolumnmeana(in, 6, 2, columnMeanmedIn_6_2);
  for (i = 0 ; i < 6 ; ++i) {
    printf("columnMeanmedIn_6_2[%d] = %e + %ei\n", i, creals(columnMeanmedIn_6_2[i]), cimags(columnMeanmedIn_6_2[i]));
  }
  assert(creals(columnMeanmedIn_6_2[0]) == 0.6f / 2.0f && cimags(columnMeanmedIn_6_2[0]) == 1.6f / 2.0f);
  assert(creals(columnMeanmedIn_6_2[1]) == 0.8f / 2.0f && cimags(columnMeanmedIn_6_2[1]) == 1.4f / 2.0f);
  assert(creals(columnMeanmedIn_6_2[2]) == 1.0f / 2.0f && cimags(columnMeanmedIn_6_2[2]) == 1.2f / 2.0f);
  assert(creals(columnMeanmedIn_6_2[3]) == 1.2f / 2.0f && cimags(columnMeanmedIn_6_2[3]) == 1.0f / 2.0f);
  assert(creals(columnMeanmedIn_6_2[4]) == 1.4f / 2.0f && cimags(columnMeanmedIn_6_2[4]) == 0.8f / 2.0f);
  assert(creals(columnMeanmedIn_6_2[5]) == 1.6f / 2.0f && cimags(columnMeanmedIn_6_2[5]) == 0.6f / 2.0f);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ 3+3.6i 3.6+3i ]
  */
  ccolumnmeana(in, 2, 6, columnMeanmedIn_2_6);
  for (i = 0 ; i < 2 ; ++i) {
    printf("columnMeanmedIn_2_6[%d] = %e + %ei\n", i, creals(columnMeanmedIn_2_6[i]), cimags(columnMeanmedIn_2_6[i]));
  }
  assert(creals(columnMeanmedIn_2_6[0]) == 3.0f / 6.0f && cimags(columnMeanmedIn_2_6[0]) == 3.6f / 6.0f);
  assert(creals(columnMeanmedIn_2_6[1]) == 3.6f / 6.0f);
  assert(fabs(cimags(columnMeanmedIn_2_6[1]) -3.0f / 6.0f) < 1e-06);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 6.6+6.6i ]
  */
  ccolumnmeana(in, 1, 12, columnMeanmedIn_1_12);
  printf("columnMeanmedIn_1_12[%d] = %e + %ei\n", 0, creals(columnMeanmedIn_1_12[0]), cimags(columnMeanmedIn_1_12[0]));
  assert(creals(columnMeanmedIn_1_12[0]) == 6.6f / 12.0f && cimags(columnMeanmedIn_1_12[0]) == 6.6f / 12.0f);


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
  ccolumnmeana(in, 12, 1, columnMeanmedIn_12_1);
 for (i = 0 ; i < 12 ; ++i) {
   printf("columnMeanmedIn_12_1[%d] = %e + %ei\n", i, creals(columnMeanmedIn_12_1[i]), cimags(columnMeanmedIn_12_1[i]));
   assert(creals(columnMeanmedIn_12_1[i]) == creals(in[i]) && cimags(columnMeanmedIn_12_1[i]) == cimags(in[i]));
 }

  return 0;
}

static int testMean(void) {

  smeansTest();
  smeanaTest();
  srowmeanaTest();
  scolumnmeanaTest();
  cmeansTest();
  cmeanaTest();
  crowmeanaTest();
  ccolumnmeanaTest();

  return 0;
}

int main(void) {
  assert(testMean() == 0);
  return 0;
}
