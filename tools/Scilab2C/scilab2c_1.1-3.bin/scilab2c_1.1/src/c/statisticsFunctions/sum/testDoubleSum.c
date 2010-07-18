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

#define ERROR(x) printf("diff = %e\n", x)

static int dsumsTest(void) {
  double value1 = 3.0;
  double value2 = 1.123456789;

  printf("\n>>>> Sum Double Scalar Test\n");
  assert(dsums(value1) == 3.0);
  assert(dsums(value2) == 1.123456789);

  return 0;
}

static int dsumaTest(void) {
  double table1[3] = {3.0, 3.0, 3.0};
  double table2[5] = {9.186784563,
		     9.186784563,
		     9.186784563,
		     9.186784563,
		     9.186784563};

  printf("\n>>>> Sum Double Array Test\n");
  assert(dsuma(table1, 3) == 9.0);
  assert((dsuma(table2, 5) - (9.186784653 + 9.186785634 + 9.186784563 + 9.186784563 + 9.186784563)) < 3e-16);
  return 0;
}

static int dcolumnsumaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double table2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  double columnSummedTable1_3_3[3] = {0};
  double columnSummedTable1_1_9[1] = {0};
  double columnSummedTable1_9_1[9] = {0};
  double columnSummedTable2_2_5[2] = {0};
  double columnSummedTable2_5_2[5] = {0};

  printf("\n>>>> Column Sum Double Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 6 15 24 ]
    [ 7 8 9 ]
  */
  dcolumnsuma(table1, 3, 3,columnSummedTable1_3_3);
  assert(columnSummedTable1_3_3[0] == 6.0);
  assert(columnSummedTable1_3_3[1] == 15.0);
  assert(columnSummedTable1_3_3[2] == 24.0);

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  dcolumnsuma(table1, 1, 9,columnSummedTable1_1_9);
  assert(columnSummedTable1_1_9[0] == 45.0);

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
  dcolumnsuma(table1, 9, 1,columnSummedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnSummedTable1_9_1[%d] = %e\n", i, columnSummedTable1_9_1[i]);
    assert(columnSummedTable1_9_1[i] == table1[i]);
  }

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 25 30 ]
  */
  dcolumnsuma(table2, 2, 5,columnSummedTable2_2_5);
  assert(columnSummedTable2_2_5[0] == 25.0);
  assert(columnSummedTable2_2_5[1] == 30.0);
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
  dcolumnsuma(table2, 5, 2,columnSummedTable2_5_2);
  assert(columnSummedTable2_5_2[0] == 7.0);
  assert(columnSummedTable2_5_2[1] == 9.0);
  assert(columnSummedTable2_5_2[2] == 11.0);
  assert(columnSummedTable2_5_2[3] == 13.0);
  assert(columnSummedTable2_5_2[4] == 15.0);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnSummedTable2_5_2[%d] = %e\n", i, columnSummedTable2_5_2[i]);
  }

  return 0;
}

static int drowsumaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double table2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  double rowSummedTable1_3_3[3] = {0};
  double rowSummedTable1_1_9[9] = {0};
  double rowSummedTable1_9_1[1] = {0};
  double rowSummedTable2_2_5[5] = {0};
  double rowSummedTable2_5_2[2] = {0};

  printf("\n>>>> Row Sum Double Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 12 15 18 ]
    [ 7 8 9 ]
  */
  drowsuma(table1, 3, 3,rowSummedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowSummedTable1_3_3[%d] = %e\n", i, rowSummedTable1_3_3[i]);
  }
  assert(rowSummedTable1_3_3[0] == 12.0);
  assert(rowSummedTable1_3_3[1] == 15.0);
  assert(rowSummedTable1_3_3[2] == 18.0);

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
  drowsuma(table1, 1, 9,rowSummedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowSummedTable1_1_9[%d] = %e\n", i, rowSummedTable1_1_9[i]);
    assert(rowSummedTable1_1_9[i] == table1[i]);
  }

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  drowsuma(table1, 9, 1,rowSummedTable1_9_1);
  assert(rowSummedTable1_9_1[0] == 45.0);

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 3 7 11 15 19 ]
  */
  drowsuma(table2, 2, 5,rowSummedTable2_2_5);
  assert(rowSummedTable2_2_5[0] == 3.0);
  assert(rowSummedTable2_2_5[1] == 7.0);
  assert(rowSummedTable2_2_5[2] == 11.0);
  assert(rowSummedTable2_2_5[3] == 15.0);
  assert(rowSummedTable2_2_5[4] == 19.0);
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
  drowsuma(table2, 5, 2,rowSummedTable2_5_2);
  assert(rowSummedTable2_5_2[0] == 15.0);
  assert(rowSummedTable2_5_2[1] == 40.0);
  for ( i = 0 ; i < 2 ; ++i) {
    printf("rowSummedTable2_5_2[%d] = %e\n", i, rowSummedTable2_5_2[i]);
  }

  return 0;
}

static int zsumsTest(void) {
  doubleComplex value1 = DoubleComplex(3.0, 3.0);
  doubleComplex value2 = DoubleComplex(1.123456789, 1.123456789);

  printf("\n>>>> Sum Double Complex Scalar Test\n");
  assert(zreals(zsums(value1)) == 3.0);
  assert(zimags(zsums(value1)) == 3.0);
  assert(zreals(zsums(value2)) == 1.123456789);
  assert(zimags(zsums(value2)) == 1.123456789);
  return 0;
}

static int zsumaTest(void) {
  doubleComplex value1 = DoubleComplex(3.0, 3.0);
  doubleComplex table1[3];
  doubleComplex value2 = DoubleComplex(9.186784563,9.186784563);
  doubleComplex table2[5];

  printf("\n>>>> Sum Double Complex Array Test\n");
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
  printf("%e\n", zimags(zsuma(table1, 3)));
  printf("%e\n", zreals(zsuma(table2, 5)));
  printf("%e\n", zimags(zsuma(table2, 5)));
  printf("%e\n", zreals(zsuma(table2, 5)));
  printf("%e\n", zimags(zsuma(table2, 5)));
  printf("--------\n");
#endif
  assert(zreals(zsuma(table1, 3)) == 9.0);
  assert(zimags(zsuma(table1, 3)) == 9.0);
  assert(zreals(zsuma(table2, 5)) == 9.186784563 + 9.186784563 + 9.186784563 + 9.186784563 + 9.186784563);
  assert(zimags(zsuma(table2, 5)) == 9.186784563 + 9.186784563 + 9.186784563 + 9.186784563 + 9.186784563);

  return 0;
}

static int zrowsumaTest(void) {
  int i = 0;
  doubleComplex in[12];
  doubleComplex rowSummedIn_4_3[3];
  doubleComplex rowSummedIn_3_4[4];
  doubleComplex rowSummedIn_6_2[2];
  doubleComplex rowSummedIn_2_6[6];
  doubleComplex rowSummedIn_1_12[12];
  doubleComplex rowSummedIn_12_1[1];

  printf("\n>>>> Row Sum Double Complex Array Test\n");
  /* Init input var */
  for (i = 0 ; i < 12 ; ++i)
    {
      in[i] = DoubleComplex((double) i / 10.0, (11.0 - (double) i) / 10.0);
    }

  /*
    [ 1.1i     0.4+0.7i 0.8+0.3i ]
    [ 0.1+i    0.5+0.6i 0.9+0.2i ] => [ 0.6+3.8i 2.2+2.2i 3.8+0.6i ]
    [ 0.2+0.9i 0.6+0.5i 1+0.1i   ]
    [ 0.3+0.8i 0.7+0.4i 1.1      ]
  */
  zrowsuma(in, 4, 3, rowSummedIn_4_3);
  for (i = 0 ; i < 3 ; ++i) {
    printf("rowSummedIn_4_3[%d] = %e + %ei\n", i, zreals(rowSummedIn_4_3[i]), zimags(rowSummedIn_4_3[i]));
  }
  assert(fabs(zreals(rowSummedIn_4_3[0]) -0.6) < 2e-16);
  assert(zimags(rowSummedIn_4_3[0]) == 3.8);
  assert(zreals(rowSummedIn_4_3[1]) == 2.2);
  assert(fabs(zimags(rowSummedIn_4_3[1]) - 2.2) < 1e-15);
  assert(fabs(zreals(rowSummedIn_4_3[2]) - 3.8) < 1e-15);
  assert(zimags(rowSummedIn_4_3[2]) == 0.6);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ 0.3+3.i 1.2+2.1i 2.1+1.2i 3.+0.3i]
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]
  */
  zrowsuma(in, 3, 4, rowSummedIn_3_4);
  for (i = 0 ; i < 4 ; ++i) {
    printf("rowSummedIn_3_4[%d] = %e + %ei\n", i, zreals(rowSummedIn_3_4[i]), zimags(rowSummedIn_3_4[i]));
  }
  assert(fabs(zreals(rowSummedIn_3_4[0]) - 0.3) < 1e-16);
  assert(zimags(rowSummedIn_3_4[0]) == 3.0);
  assert(zreals(rowSummedIn_3_4[1]) == 1.2 && zimags(rowSummedIn_3_4[1]) == 2.1);
  assert(fabs(zreals(rowSummedIn_3_4[2]) - 2.1) < 1e-15);
  assert(zimags(rowSummedIn_3_4[2]) == 1.2);
  assert(zreals(rowSummedIn_3_4[3]) == 3.0);
  assert(fabs(zimags(rowSummedIn_3_4[3]) - 0.3) < 1e-16);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 1.5+5.1i 5.1+1.5i ]
    [ 0.3+0.8i 0.9+0.2i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  zrowsuma(in, 6, 2, rowSummedIn_6_2);
  for (i = 0 ; i < 2 ; ++i) {
    printf("rowSummedIn_6_2[%d] = %e + %ei\n", i, zreals(rowSummedIn_6_2[i]), zimags(rowSummedIn_6_2[i]));
  }
  assert(zreals(rowSummedIn_6_2[0]) == 1.5 && zimags(rowSummedIn_6_2[0]) == 5.1);
  assert(zreals(rowSummedIn_6_2[1]) == 5.1 && zimags(rowSummedIn_6_2[1]) == 1.5);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ 0.1+2.1i 0.5+1.7i 0.9+1.3i 1.3+0.9i 1.7+0.5i 2.1+0.1i ]
  */
  zrowsuma(in, 2, 6, rowSummedIn_2_6);
  for (i = 0 ; i < 6 ; ++i) {
    printf("rowSummedIn_2_6[%d] = %e + %ei\n", i, zreals(rowSummedIn_2_6[i]), zimags(rowSummedIn_2_6[i]));
  }
  assert(zreals(rowSummedIn_2_6[0]) == 0.1 && zimags(rowSummedIn_2_6[0]) == 2.1);
  assert(zreals(rowSummedIn_2_6[1]) == 0.5);
  assert(fabs(zimags(rowSummedIn_2_6[1]) - 1.7) < 3e-16);
  assert(zreals(rowSummedIn_2_6[2]) == 0.9);
  assert(fabs(zimags(rowSummedIn_2_6[2]) - 1.3) < 3e-16);
  assert(fabs(zreals(rowSummedIn_2_6[3]) - 1.3) < 3e-16);
  assert(zimags(rowSummedIn_2_6[3]) == 0.9);
  assert(fabs(zreals(rowSummedIn_2_6[4]) - 1.7) < 3e-16);
  assert(zimags(rowSummedIn_2_6[4]) == 0.5);
  assert(zreals(rowSummedIn_2_6[5]) == 2.1 && zimags(rowSummedIn_2_6[5]) == 0.1);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
  */
  zrowsuma(in, 1, 12, rowSummedIn_1_12);
  for (i = 0 ; i < 12 ; ++i) {
    printf("rowSummedIn_1_12[%d] = %e + %ei\n", i, zreals(rowSummedIn_1_12[i]), zimags(rowSummedIn_1_12[i]));
    assert(zreals(rowSummedIn_1_12[i]) == zreals(in[i]) && zimags(rowSummedIn_1_12[i]) == zimags(in[i]));
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
  zrowsuma(in, 12, 1, rowSummedIn_12_1);
  printf("rowSummedIn_12_1[%d] = %e + %ei\n", 0, zreals(rowSummedIn_12_1[0]), zimags(rowSummedIn_12_1[0]));
  assert(zreals(rowSummedIn_12_1[0]) == 6.6 && zimags(rowSummedIn_12_1[0]) == 6.6);

  return 0;
}

static int zcolumnsumaTest(void) {
  int i = 0;
  doubleComplex in[12];
  doubleComplex columnSummedIn_4_3[4];
  doubleComplex columnSummedIn_3_4[3];
  doubleComplex columnSummedIn_6_2[6];
  doubleComplex columnSummedIn_2_6[2];
  doubleComplex columnSummedIn_1_12[1];
  doubleComplex columnSummedIn_12_1[12];

  printf("\n>>>> ColumnSum Double Complex Array Test\n");
  /* Init input var */
  for (i = 0 ; i < 12 ; ++i)
    {
      in[i] = DoubleComplex((double) i / 10.0, (11.0 - (double) i) / 10.0);
    }

  /*
    [ 1.1i     0.4+0.7i 0.8+0.3i ]
    [ 0.1+i    0.5+0.6i 0.9+0.2i ] => [ 1.2+2.1i 1.5+1.8i 1.8+1.5i 2.1+1.2i ]
    [ 0.2+0.9i 0.6+0.5i 1+0.1i   ]
    [ 0.3+0.8i 0.7+0.4i 1.1      ]
  */
  zcolumnsuma(in, 4, 3, columnSummedIn_4_3);
  for (i = 0 ; i < 4 ; ++i) {
    printf("columnSummedIn_4_3[%d] = %e + %ei\n", i, zreals(columnSummedIn_4_3[i]), zimags(columnSummedIn_4_3[i]));
  }
  assert(fabs(zreals(columnSummedIn_4_3[0]) - 1.2) < 1e-15);
  assert(zimags(columnSummedIn_4_3[0]) == 2.1);
  assert(zreals(columnSummedIn_4_3[1]) == 1.5 && zimags(columnSummedIn_4_3[1]) == 1.8);
  assert(zreals(columnSummedIn_4_3[2]) == 1.8 && zimags(columnSummedIn_4_3[2]) == 1.5);
  assert(zreals(columnSummedIn_4_3[3]) == 2.1);
  assert(fabs(zimags(columnSummedIn_4_3[3]) - 1.2) < 1e-15);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ 1.8+2.6i 2.2+2.2i 2.6+1.8i ]
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]
  */
  zcolumnsuma(in, 3, 4, columnSummedIn_3_4);
  for (i = 0 ; i < 3 ; ++i) {
    printf("columnSummedIn_3_4[%d] = %e + %ei\n", i, zreals(columnSummedIn_3_4[i]), zimags(columnSummedIn_3_4[i]));
  }
  assert(fabs(zreals(columnSummedIn_3_4[0]) - 1.8) < 1e-15);
  assert(fabs(zimags(columnSummedIn_3_4[0]) - 2.6) < 1e-15);
  assert(zreals(columnSummedIn_3_4[1]) == 2.2 && zimags(columnSummedIn_3_4[1]) == 2.2);
  assert(zreals(columnSummedIn_3_4[2]) == 2.6 && zimags(columnSummedIn_3_4[2]) == 1.8);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 0.6+1.6i 0.8+1.4i 1+1.2i 1.2+i 1.4+0.8i 1.6+0.6i ]
    [ 0.3+0.8i 0.9+0.2i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  zcolumnsuma(in, 6, 2, columnSummedIn_6_2);
  for (i = 0 ; i < 6 ; ++i) {
    printf("columnSummedIn_6_2[%d] = %e + %ei\n", i, zreals(columnSummedIn_6_2[i]), zimags(columnSummedIn_6_2[i]));
  }
  assert(zreals(columnSummedIn_6_2[0]) == 0.6 && zimags(columnSummedIn_6_2[0]) == 1.6);
  assert(fabs(zreals(columnSummedIn_6_2[1]) - 0.8) < 1e-15);
  assert(zimags(columnSummedIn_6_2[1]) == 1.4);
  assert(zreals(columnSummedIn_6_2[2]) == 1.0 && zimags(columnSummedIn_6_2[2]) == 1.2);
  assert(zreals(columnSummedIn_6_2[3]) == 1.2 && zimags(columnSummedIn_6_2[3]) == 1.0);
  assert(zreals(columnSummedIn_6_2[4]) == 1.4);
  assert(fabs(zimags(columnSummedIn_6_2[4]) - 0.8) < 1e-15);
  assert(zreals(columnSummedIn_6_2[5]) == 1.6 && zimags(columnSummedIn_6_2[5]) == 0.6);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ 3+3.6i 3.6+3i ]
  */
  zcolumnsuma(in, 2, 6, columnSummedIn_2_6);
  for (i = 0 ; i < 2 ; ++i) {
    printf("columnSummedIn_2_6[%d] = %e + %ei\n", i, zreals(columnSummedIn_2_6[i]), zimags(columnSummedIn_2_6[i]));
  }
  assert(zreals(columnSummedIn_2_6[0]) == 3.0 && zimags(columnSummedIn_2_6[0]) == 3.6);
  assert(zreals(columnSummedIn_2_6[1]) == 3.6 && zimags(columnSummedIn_2_6[1]) == 3.0);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 6.6+6.6i ]
  */
  zcolumnsuma(in, 1, 12, columnSummedIn_1_12);
  printf("columnSummedIn_1_12[%d] = %e + %ei\n", 0, zreals(columnSummedIn_1_12[0]), zimags(columnSummedIn_1_12[0]));
  assert(zreals(columnSummedIn_1_12[0]) == 6.6 && zimags(columnSummedIn_1_12[0]) == 6.6);


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
  zcolumnsuma(in, 12, 1, columnSummedIn_12_1);
 for (i = 0 ; i < 12 ; ++i) {
   printf("columnSummedIn_12_1[%d] = %e + %ei\n", i, zreals(columnSummedIn_12_1[i]), zimags(columnSummedIn_12_1[i]));
   assert(zreals(columnSummedIn_12_1[i]) == zreals(in[i]) && zimags(columnSummedIn_12_1[i]) == zimags(in[i]));
 }

  return 0;
}

static int testSum(void) {

  dsumsTest();
  dsumaTest();
  drowsumaTest();
  dcolumnsumaTest();
  zsumsTest();
  zsumaTest();
  zrowsumaTest();
  zcolumnsumaTest();

  return 0;
}

int main(void) {
  assert(testSum() == 0);
  return 0;
}
