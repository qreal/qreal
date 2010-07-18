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

#define ERROR(x) printf("diff = %e\n", x)

static int dmeansTest(void) {
  double value1 = 3.0;
  double value2 = 1.123456789;

  printf("\n>>>> Mean Double Scalar Test\n");
  assert( ( fabs(dmeans(value1) ) - (  3.0 ) ) / fabs ( dmeans(value1)  ) <  3e-16 );
  assert( ( fabs(dmeans(value2) ) - (  1.123456789 ) ) / fabs ( dmeans(value2)  ) <  3e-16 );

  return 0;
}

static int dmeanaTest(void) {
  double table1[3] = {3.0, 3.0, 3.0};
  double table2[5] = {9.186784563,
		     9.186784563,
		     9.186784563,
		     9.186784563,
		     9.186784563};

  printf("\n>>>> Mean Double Array Test\n");
  assert( ( fabs(dmeana(table1, 3) ) - (  9.0 / 3.0 ) ) / fabs ( dmeana(table1, 3)  ) <  3e-16 );
  assert(( ( fabs(dmeana(table2, 5) ) - (  (9.186784563 + 9.186784563 + 9.186784563 + 9.186784563 + 9.186784563) / 5.0) ) ) / fabs ( dmeana(table2, 5)  ) <  3e-16 );
  return 0;
}

static int dcolumnmeanaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double table2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  double columnMeanmedTable1_3_3[3] = {0};
  double columnMeanmedTable1_1_9[1] = {0};
  double columnMeanmedTable1_9_1[9] = {0};
  double columnMeanmedTable2_2_5[2] = {0};
  double columnMeanmedTable2_5_2[5] = {0};

  printf("\n>>>> Column Mean Double Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 6 15 24 ]
    [ 7 8 9 ]
  */
  dcolumnmeana(table1, 3, 3,columnMeanmedTable1_3_3);
  assert( ( fabs(columnMeanmedTable1_3_3[0] ) - (  6.0 / 3.0 ) ) / fabs ( columnMeanmedTable1_3_3[0]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable1_3_3[1] ) - (  15.0 / 3.0 ) ) / fabs ( columnMeanmedTable1_3_3[1]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable1_3_3[2] ) - (  24.0 / 3.0 ) ) / fabs ( columnMeanmedTable1_3_3[2]  ) <  3e-16 );

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  dcolumnmeana(table1, 1, 9,columnMeanmedTable1_1_9);
  assert( ( fabs(columnMeanmedTable1_1_9[0] ) - (  45.0 / 9.0 ) ) / fabs ( columnMeanmedTable1_1_9[0]  ) <  3e-16 );

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
  dcolumnmeana(table1, 9, 1,columnMeanmedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnMeanmedTable1_9_1[%d] = %e\n", i, columnMeanmedTable1_9_1[i]);
    assert(columnMeanmedTable1_9_1[i] == table1[i]);
  }

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 25 30 ]
  */
  dcolumnmeana(table2, 2, 5,columnMeanmedTable2_2_5);
  assert( ( fabs(columnMeanmedTable2_2_5[0] ) - (  25.0 / 5.0 ) ) / fabs ( columnMeanmedTable2_2_5[0]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_2_5[1] ) - (  30.0 / 5.0 ) ) / fabs ( columnMeanmedTable2_2_5[1]  ) <  3e-16 );
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
  dcolumnmeana(table2, 5, 2,columnMeanmedTable2_5_2);
  assert( ( fabs(columnMeanmedTable2_5_2[0] ) - (  7.0 / 2.0 ) ) / fabs ( columnMeanmedTable2_5_2[0]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_5_2[1] ) - (  9.0 / 2.0 ) ) / fabs ( columnMeanmedTable2_5_2[1]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_5_2[2] ) - (  11.0 / 2.0 ) ) / fabs ( columnMeanmedTable2_5_2[2]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_5_2[3] ) - (  13.0 / 2.0 ) ) / fabs ( columnMeanmedTable2_5_2[3]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_5_2[4] ) - (  15.0 / 2.0 ) ) / fabs ( columnMeanmedTable2_5_2[4]  ) <  3e-16 );
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnMeanmedTable2_5_2[%d] = %e\n", i, columnMeanmedTable2_5_2[i]);
  }

  return 0;
}

static int drowmeanaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double table2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  double rowMeanmedTable1_3_3[3] = {0};
  double rowMeanmedTable1_1_9[9] = {0};
  double rowMeanmedTable1_9_1[1] = {0};
  double rowMeanmedTable2_2_5[5] = {0};
  double rowMeanmedTable2_5_2[2] = {0};

  printf("\n>>>> Row Mean Double Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 12 15 18 ]
    [ 7 8 9 ]
  */
  drowmeana(table1, 3, 3,rowMeanmedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowMeanmedTable1_3_3[%d] = %e\n", i, rowMeanmedTable1_3_3[i]);
  }
  assert( ( fabs(rowMeanmedTable1_3_3[0] ) - (  12.0 / 3.0 ) ) / fabs ( rowMeanmedTable1_3_3[0]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable1_3_3[1] ) - (  15.0 / 3.0 ) ) / fabs ( rowMeanmedTable1_3_3[1]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable1_3_3[2] ) - (  18.0 / 3.0 ) ) / fabs ( rowMeanmedTable1_3_3[2]  ) <  3e-16 );

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
  drowmeana(table1, 1, 9,rowMeanmedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowMeanmedTable1_1_9[%d] = %e\n", i, rowMeanmedTable1_1_9[i]);
    assert(rowMeanmedTable1_1_9[i] == table1[i]);
  }

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  drowmeana(table1, 9, 1,rowMeanmedTable1_9_1);
  assert( ( fabs(rowMeanmedTable1_9_1[0] ) - (  45.0 / 9.0 ) ) / fabs ( rowMeanmedTable1_9_1[0]  ) <  3e-16 );

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 3 7 11 15 19 ]
  */
  drowmeana(table2, 2, 5,rowMeanmedTable2_2_5);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("rowMeanmedTable2_2_5[%d] = %e\n", i, rowMeanmedTable2_2_5[i]);
  }
  assert( ( fabs(rowMeanmedTable2_2_5[0] ) - (  3.0 / 2.0 ) ) / fabs ( rowMeanmedTable2_2_5[0]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_2_5[1] ) - (  7.0 / 2.0 ) ) / fabs ( rowMeanmedTable2_2_5[1]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_2_5[2] ) - (  11.0 / 2.0 ) ) / fabs ( rowMeanmedTable2_2_5[2]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_2_5[3] ) - (  15.0 / 2.0 ) ) / fabs ( rowMeanmedTable2_2_5[3]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_2_5[4] ) - (  19.0 / 2.0 ) ) / fabs ( rowMeanmedTable2_2_5[4]  ) <  3e-16 );

  /*
    [ 1  6 ]
    [ 2  7 ] => [ 15 40 ]
    [ 3  8 ]
    [ 4  9 ]
    [ 5 10 ]
  */
  drowmeana(table2, 5, 2,rowMeanmedTable2_5_2);
  assert( ( fabs(rowMeanmedTable2_5_2[0] ) - (  15.0 / 5.0 ) ) / fabs ( rowMeanmedTable2_5_2[0]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_5_2[1] ) - (  40.0 / 5.0 ) ) / fabs ( rowMeanmedTable2_5_2[1]  ) <  3e-16 );
  for ( i = 0 ; i < 2 ; ++i) {
    printf("rowMeanmedTable2_5_2[%d] = %e\n", i, rowMeanmedTable2_5_2[i]);
  }

  return 0;
}

static int zmeansTest(void) {
  doubleComplex value1 = DoubleComplex(3.0, 3.0);
  doubleComplex value2 = DoubleComplex(1.123456789, 1.123456789);

  printf("\n>>>> Mean Double Complex Scalar Test\n");
  assert( ( fabs(zreals(zmeans(value1)) ) - (  3.0 ) ) / fabs ( zreals(zmeans(value1))  ) <  3e-16 );
  assert( ( fabs(zimags(zmeans(value1)) ) - (  3.0 ) ) / fabs ( zimags(zmeans(value1))  ) <  3e-16 );
  assert( ( fabs(zreals(zmeans(value2)) ) - (  1.123456789 ) ) / fabs ( zreals(zmeans(value2))  ) <  3e-16 );
  assert( ( fabs(zimags(zmeans(value2)) ) - (  1.123456789 ) ) / fabs ( zimags(zmeans(value2))  ) <  3e-16 );
  return 0;
}

static int zmeanaTest(void) {
  doubleComplex value1 = DoubleComplex(3.0, 3.0);
  doubleComplex table1[3];
  doubleComplex value2 = DoubleComplex(9.186784563,9.186784563);
  doubleComplex table2[5];

  printf("\n>>>> Mean Double Complex Array Test\n");
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
  printf("%e\n", zimags(zmeana(table1, 3)));
  printf("%e\n", zreals(zmeana(table2, 5)));
  printf("%e\n", zimags(zmeana(table2, 5)));
  printf("%e\n", zreals(zmeana(table2, 5)));
  printf("%e\n", zimags(zmeana(table2, 5)));
  printf("--------\n");
#endif
  assert( ( fabs(zreals(zmeana(table1, 3)) ) - (  9.0 / 3.0 ) ) / fabs ( zreals(zmeana(table1, 3))  ) <  3e-16 );
  assert( ( fabs(zimags(zmeana(table1, 3)) ) - (  9.0 / 3.0 ) ) / fabs ( zimags(zmeana(table1, 3))  ) <  3e-16 );
  assert( ( fabs(zreals(zmeana(table2, 5)) ) - (  (9.186784563 + 9.186784563 + 9.186784563 + 9.186784563 + 9.186784563) / 5.0 ) ) / fabs ( zreals(zmeana(table2, 5))  ) <  3e-16 );
  assert( ( fabs(zimags(zmeana(table2, 5)) ) - (  (9.186784563 + 9.186784563 + 9.186784563 + 9.186784563 + 9.186784563) / 5.0 ) ) / fabs ( zimags(zmeana(table2, 5))  ) <  3e-16 );

  return 0;
}

static int zrowmeanaTest(void) {
  int i = 0;
  doubleComplex in[12];
  doubleComplex rowMeanmedIn_4_3[3];
  doubleComplex rowMeanmedIn_3_4[4];
  doubleComplex rowMeanmedIn_6_2[2];
  doubleComplex rowMeanmedIn_2_6[6];
  doubleComplex rowMeanmedIn_1_12[12];
  doubleComplex rowMeanmedIn_12_1[1];

  printf("\n>>>> Row Mean Double Complex Array Test\n");
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
  zrowmeana(in, 4, 3, rowMeanmedIn_4_3);
  for (i = 0 ; i < 3 ; ++i) {
    printf("rowMeanmedIn_4_3[%d] = %e + %ei\n", i, zreals(rowMeanmedIn_4_3[i]), zimags(rowMeanmedIn_4_3[i]));
  }
  assert(fabs(zreals(rowMeanmedIn_4_3[0]) - 0.6 / 4.0) < 1e-16);
  assert( ( fabs(zimags(rowMeanmedIn_4_3[0]) ) - (  3.8 / 4.0 ) ) / fabs ( zimags(rowMeanmedIn_4_3[0])  ) <  3e-16 );
  assert( ( fabs(zreals(rowMeanmedIn_4_3[1]) ) - (  2.2 / 4.0 ) ) / fabs ( zreals(rowMeanmedIn_4_3[1])  ) <  3e-16 );
  assert(fabs(zimags(rowMeanmedIn_4_3[1]) - 2.2 / 4.0) < 3e-16);
  assert(fabs(zreals(rowMeanmedIn_4_3[2]) - 3.8 / 4.0) < 3e-16);
  assert( ( fabs(zimags(rowMeanmedIn_4_3[2]) ) - (  0.6 / 4.0 ) ) / fabs ( zimags(rowMeanmedIn_4_3[2])  ) <  3e-16 );

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ 0.3+3.i 1.2+2.1i 2.1+1.2i 3.+0.3i]
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]
  */
  zrowmeana(in, 3, 4, rowMeanmedIn_3_4);
  for (i = 0 ; i < 4 ; ++i) {
    printf("rowMeanmedIn_3_4[%d] = %e + %ei\n", i, zreals(rowMeanmedIn_3_4[i]), zimags(rowMeanmedIn_3_4[i]));
  }
  assert(fabs(zreals(rowMeanmedIn_3_4[0]) - 0.3 / 3.0) < 1e-16);
  assert( ( fabs(zimags(rowMeanmedIn_3_4[0]) ) - (  3.0 / 3.0 ) ) / fabs ( zimags(rowMeanmedIn_3_4[0])  ) <  3e-16 );
  assert(zreals(rowMeanmedIn_3_4[1]) == 1.2 / 3.0 && zimags(rowMeanmedIn_3_4[1]) == 2.1 / 3.0);
  assert(fabs(zreals(rowMeanmedIn_3_4[2]) - 2.1 / 3.0) < 3e-16);
  assert( ( fabs(zimags(rowMeanmedIn_3_4[2]) ) - (  1.2 / 3.0 ) ) / fabs ( zimags(rowMeanmedIn_3_4[2])  ) <  3e-16 );
  assert( ( fabs(zreals(rowMeanmedIn_3_4[3]) ) - (  3.0 / 3.0 ) ) / fabs ( zreals(rowMeanmedIn_3_4[3])  ) <  3e-16 );
  assert(fabs(zimags(rowMeanmedIn_3_4[3]) - 0.3 / 3.0) < 1e-16);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 1.5+5.1i 5.1+1.5i ]
    [ 0.3+0.8i 0.9+0.2i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  zrowmeana(in, 6, 2, rowMeanmedIn_6_2);
  for (i = 0 ; i < 2 ; ++i) {
    printf("rowMeanmedIn_6_2[%d] = %e + %ei\n", i, zreals(rowMeanmedIn_6_2[i]), zimags(rowMeanmedIn_6_2[i]));
  }
  assert(zreals(rowMeanmedIn_6_2[0]) == 1.5 / 6.0 && zimags(rowMeanmedIn_6_2[0]) == 5.1 / 6.0);
  assert(zreals(rowMeanmedIn_6_2[1]) == 5.1 / 6.0 && zimags(rowMeanmedIn_6_2[1]) == 1.5 / 6.0);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ 0.1+2.1i 0.5+1.7i 0.9+1.3i 1.3+0.9i 1.7+0.5i 2.1+0.1i ]
  */
  zrowmeana(in, 2, 6, rowMeanmedIn_2_6);
  for (i = 0 ; i < 6 ; ++i) {
    printf("rowMeanmedIn_2_6[%d] = %e + %ei\n", i, zreals(rowMeanmedIn_2_6[i]), zimags(rowMeanmedIn_2_6[i]));
  }
  assert(zreals(rowMeanmedIn_2_6[0]) == 0.1 / 2.0 && zimags(rowMeanmedIn_2_6[0]) == 2.1 / 2.0);
  assert( ( fabs(zreals(rowMeanmedIn_2_6[1]) ) - ( 0.5 / 2.0 ) ) / fabs(zreals(rowMeanmedIn_2_6[1])) < 3e-16);
  assert( ( fabs(zimags(rowMeanmedIn_2_6[1]) ) - ( 1.7 / 2.0 ) ) / fabs(zimags(rowMeanmedIn_2_6[1])) < 3e-16);
  assert( ( fabs(zreals(rowMeanmedIn_2_6[2]) ) - ( 0.9 / 2.0 ) ) / fabs(zreals(rowMeanmedIn_2_6[2])) < 3e-16);
  assert( ( fabs(zimags(rowMeanmedIn_2_6[2]) ) - ( 1.3 / 2.0 ) ) / fabs(zimags(rowMeanmedIn_2_6[2])) < 3e-16);
  assert( ( fabs(zreals(rowMeanmedIn_2_6[3]) ) - ( 1.3 / 2.0 ) ) / fabs(zreals(rowMeanmedIn_2_6[3])) < 3e-16);
  assert( ( fabs(zimags(rowMeanmedIn_2_6[3]) ) - ( 0.9 / 2.0 ) ) / fabs(zimags(rowMeanmedIn_2_6[3])) < 3e-16);
  assert( ( fabs(zreals(rowMeanmedIn_2_6[4]) ) - ( 1.7 / 2.0 ) ) / fabs(zreals(rowMeanmedIn_2_6[4])) < 3e-16);
  assert( ( fabs(zimags(rowMeanmedIn_2_6[4]) ) - ( 0.5 / 2.0 ) ) / fabs(zimags(rowMeanmedIn_2_6[4])) < 3e-16);
  assert( ( fabs(zreals(rowMeanmedIn_2_6[5]) ) - ( 2.1 / 2.0 ) ) / fabs(zreals(rowMeanmedIn_2_6[5])) < 3e-16);
  assert( ( fabs(zimags(rowMeanmedIn_2_6[5]) ) - ( 0.1 / 2.0 ) ) / fabs(zimags(rowMeanmedIn_2_6[5])) < 3e-16);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
  */
  zrowmeana(in, 1, 12, rowMeanmedIn_1_12);
  for (i = 0 ; i < 12 ; ++i) {
    printf("rowMeanmedIn_1_12[%d] = %e + %ei\n", i, zreals(rowMeanmedIn_1_12[i]), zimags(rowMeanmedIn_1_12[i]));
    assert(zreals(rowMeanmedIn_1_12[i]) == zreals(in[i]) && zimags(rowMeanmedIn_1_12[i]) == zimags(in[i]));
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
  zrowmeana(in, 12, 1, rowMeanmedIn_12_1);
  printf("rowMeanmedIn_12_1[%d] = %e + %ei\n", 0, zreals(rowMeanmedIn_12_1[0]), zimags(rowMeanmedIn_12_1[0]));
  assert(zreals(rowMeanmedIn_12_1[0]) == 6.6 / 12.0 && zimags(rowMeanmedIn_12_1[0]) == 6.6 / 12.0);

  return 0;
}

static int zcolumnmeanaTest(void) {
  int i = 0;
  doubleComplex in[12];
  doubleComplex columnMeanmedIn_4_3[4];
  doubleComplex columnMeanmedIn_3_4[3];
  doubleComplex columnMeanmedIn_6_2[6];
  doubleComplex columnMeanmedIn_2_6[2];
  doubleComplex columnMeanmedIn_1_12[1];
  doubleComplex columnMeanmedIn_12_1[12];

  printf("\n>>>> ColumnMean Double Complex Array Test\n");
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
  zcolumnmeana(in, 4, 3, columnMeanmedIn_4_3);
  for (i = 0 ; i < 4 ; ++i) {
    printf("columnMeanmedIn_4_3[%d] = %e + %ei\n", i, zreals(columnMeanmedIn_4_3[i]), zimags(columnMeanmedIn_4_3[i]));
  }
  assert(fabs(zreals(columnMeanmedIn_4_3[0]) - 1.2 / 3.0) < 3e-16);
  assert(fabs(zimags(columnMeanmedIn_4_3[0]) - 2.1 / 3.0) < 3e-16);
  assert(fabs(zreals(columnMeanmedIn_4_3[1]) - 1.5 / 3.0) < 3e-16);
  assert(fabs(zimags(columnMeanmedIn_4_3[1]) - 1.8 / 3.0) < 3e-16);
  assert(fabs(zreals(columnMeanmedIn_4_3[2]) - 1.8 / 3.0) < 3e-16);
  assert(fabs(zimags(columnMeanmedIn_4_3[2]) - 1.5 / 3.0) < 3e-16);
  assert(fabs(zreals(columnMeanmedIn_4_3[3]) - 2.1 / 3.0) < 3e-16);
  assert(fabs(zimags(columnMeanmedIn_4_3[3]) - 1.2 / 3.0) < 3e-16);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ 1.8+2.6i 2.2+2.2i 2.6+1.8i ]
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]
  */
  zcolumnmeana(in, 3, 4, columnMeanmedIn_3_4);
  for (i = 0 ; i < 3 ; ++i) {
    printf("columnMeanmedIn_3_4[%d] = %e + %ei\n", i, zreals(columnMeanmedIn_3_4[i]), zimags(columnMeanmedIn_3_4[i]));
  }
  assert(fabs(zreals(columnMeanmedIn_3_4[0]) - 1.8 / 4.0) < 3e-16);
  assert(fabs(zimags(columnMeanmedIn_3_4[0]) - 2.6 / 4.0) < 3e-16);
  assert(zreals(columnMeanmedIn_3_4[1]) == 2.2 / 4.0 && zimags(columnMeanmedIn_3_4[1]) == 2.2 / 4.0);
  assert(zreals(columnMeanmedIn_3_4[2]) == 2.6 / 4.0 && zimags(columnMeanmedIn_3_4[2]) == 1.8 / 4.0);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 0.6+1.6i 0.8+1.4i 1+1.2i 1.2+i 1.4+0.8i 1.6+0.6i ]
    [ 0.3+0.8i 0.9+0.2i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  zcolumnmeana(in, 6, 2, columnMeanmedIn_6_2);
  for (i = 0 ; i < 6 ; ++i) {
    printf("columnMeanmedIn_6_2[%d] = %e + %ei\n", i, zreals(columnMeanmedIn_6_2[i]), zimags(columnMeanmedIn_6_2[i]));
  }
  assert(zreals(columnMeanmedIn_6_2[0]) == 0.6 / 2.0 && zimags(columnMeanmedIn_6_2[0]) == 1.6 / 2.0);
  assert(fabs(zreals(columnMeanmedIn_6_2[1]) - 0.8 / 2.0) < 3e-16);
  assert(zimags(columnMeanmedIn_6_2[1]) == 1.4 / 2.0);
  assert(zreals(columnMeanmedIn_6_2[2]) == 1.0  / 2.0 && zimags(columnMeanmedIn_6_2[2]) == 1.2 / 2.0);
  assert(zreals(columnMeanmedIn_6_2[3]) == 1.2  / 2.0 && zimags(columnMeanmedIn_6_2[3]) == 1.0 / 2.0);
  assert(zreals(columnMeanmedIn_6_2[4]) == 1.4 / 2.0);
  assert(fabs(zimags(columnMeanmedIn_6_2[4]) - 0.8 / 2.0) < 3e-16);
  assert(zreals(columnMeanmedIn_6_2[5]) == 1.6 / 2.0 && zimags(columnMeanmedIn_6_2[5]) == 0.6 / 2.0);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ 3+3.6i 3.6+3i ]
  */
  zcolumnmeana(in, 2, 6, columnMeanmedIn_2_6);
  for (i = 0 ; i < 2 ; ++i) {
    printf("columnMeanmedIn_2_6[%d] = %e + %ei\n", i, zreals(columnMeanmedIn_2_6[i]), zimags(columnMeanmedIn_2_6[i]));
  }
  assert(fabs(zreals(columnMeanmedIn_2_6[0]) - 3.0 / 6.0) < 3e-16);
  assert(fabs(zimags(columnMeanmedIn_2_6[0]) - 3.6 / 6.0) < 3e-16);
  assert(fabs(zreals(columnMeanmedIn_2_6[1]) - 3.6 / 6.0) < 3e-16); 
  assert(fabs(zimags(columnMeanmedIn_2_6[1]) - 3.0 / 6.0) < 3e-16);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 6.6+6.6i ]
  */
  zcolumnmeana(in, 1, 12, columnMeanmedIn_1_12);
  printf("columnMeanmedIn_1_12[%d] = %e + %ei\n", 0, zreals(columnMeanmedIn_1_12[0]), zimags(columnMeanmedIn_1_12[0]));
  assert(zreals(columnMeanmedIn_1_12[0]) == 6.6 / 12.0 && zimags(columnMeanmedIn_1_12[0]) == 6.6 / 12.0);


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
  zcolumnmeana(in, 12, 1, columnMeanmedIn_12_1);
 for (i = 0 ; i < 12 ; ++i) {
   printf("columnMeanmedIn_12_1[%d] = %e + %ei\n", i, zreals(columnMeanmedIn_12_1[i]), zimags(columnMeanmedIn_12_1[i]));
   assert(zreals(columnMeanmedIn_12_1[i]) == zreals(in[i]) && zimags(columnMeanmedIn_12_1[i]) == zimags(in[i]));
 }

  return 0;
}

static int testMean(void) {

  dmeansTest();
  dmeanaTest();
  drowmeanaTest();
  dcolumnmeanaTest();
  zmeansTest();
  zmeanaTest();
  zrowmeanaTest();
  zcolumnmeanaTest();

  return 0;
}

int main(void) {
  assert(testMean() == 0);
  return 0;
}
