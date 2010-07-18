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

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "prod.h"

/* #define LOCAL_DEBUG */

#define ERROR(x) printf("diff = %e\n", x)

static int dprodsTest(void) {
  double value1 = 3.0;
  double value2 = 1.123456789;

  printf("\n>>>> prod Double Scalar Test\n");
  assert(dprods(value1) == 3.0);
  assert(dprods(value2) == 1.123456789);

  return 0;
}

static int dprodaTest(void) {
  double table1[3] = {3.0, 3.0, 3.0};
  double table2[5] = {9.186784563,
		     9.186784563,
		     9.186784563,
		     9.186784563,
		     9.186784563};

  printf("\n>>>> prod Double Array Test\n");
  printf("%f\n", dproda(table1, 3));
  assert(dproda(table1, 3) == 27.0);
  assert((dproda(table2, 5) - (9.186784563 * 9.186784563 * 9.186784563 * 9.186784563 * 9.186784563)) / dproda(table2, 5) < 3e-15);
  return 0;
}

static int dcolumnprodaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double table2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  double columnProductedTable1_3_3[3] = {0};
  double columnProductedTable1_1_9[1] = {0};
  double columnProductedTable1_9_1[9] = {0};
  double columnProductedTable2_2_5[2] = {0};
  double columnProductedTable2_5_2[5] = {0};

  printf("\n>>>> Column prod Double Array Test\n");
  /*
    [ 1 2 3 ]    [ 1*2*3=6   ]
    [ 4 5 6 ] => [ 4*5*6=120 ]
    [ 7 8 9 ]    [ 7*8*9=504 ]
  */
  dcolumnproda(table1, 3, 3,columnProductedTable1_3_3);
  assert(columnProductedTable1_3_3[0] == 6.0);
  assert(columnProductedTable1_3_3[1] == 120.0);
  assert(columnProductedTable1_3_3[2] == 504.0);

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 362880 ]
  */
  dcolumnproda(table1, 1, 9,columnProductedTable1_1_9);
  assert(columnProductedTable1_1_9[0] == 362880.0);

  /*
    [ 1 ]     [ 1 ]
    [ 2 ]     [ 2 ]
    [ 3 ]     [ 3 ]
    [ 4 ]     [ 4 ]
    [ 5 ] =>  [ 5 ]
    [ 6 ]     [ 6 ]
    [ 7 ]     [ 7 ]
    [ 8 ]     [ 8 ]
    [ 9 ]     [ 9 ]
  */
  dcolumnproda(table1, 9, 1,columnProductedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnProductedTable1_9_1[%d] = %e\n", i, columnProductedTable1_9_1[i]);
    assert(columnProductedTable1_9_1[i] == table1[i]);
  }

  /*
    [ 1 3 5 7  9 ]    [ 945  ]
    [ 2 4 6 8 10 ] => [ 3840 ]
  */
  dcolumnproda(table2, 2, 5,columnProductedTable2_2_5);
  assert(columnProductedTable2_2_5[0] == 945.0);
  assert(columnProductedTable2_2_5[1] == 3840.0);
  for ( i = 0 ; i < 2 ; ++i) {
    printf("columnProductedTable2_2_5[%d] = %e\n", i, columnProductedTable2_2_5[i]);
  }

  /*
    [ 1  6 ]    [ 6  ]
    [ 2  7 ] => [ 14 ]
    [ 3  8 ]    [ 24 ]
    [ 4  9 ]    [ 36 ]
    [ 5 10 ]    [ 50 ]
  */
  dcolumnproda(table2, 5, 2,columnProductedTable2_5_2);
  assert(columnProductedTable2_5_2[0] == 6.0);
  assert(columnProductedTable2_5_2[1] == 14.0);
  assert(columnProductedTable2_5_2[2] == 24.0);
  assert(columnProductedTable2_5_2[3] == 36.0);
  assert(columnProductedTable2_5_2[4] == 50.0);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnProductedTable2_5_2[%d] = %e\n", i, columnProductedTable2_5_2[i]);
  }

  return 0;
}

static int drowprodaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double table2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  double rowProductedTable1_3_3[3] = {0};
  double rowProductedTable1_1_9[9] = {0};
  double rowProductedTable1_9_1[1] = {0};
  double rowProductedTable2_2_5[5] = {0};
  double rowProductedTable2_5_2[2] = {0};

  printf("\n>>>> Row prod Double Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 28 80 162 ]
    [ 7 8 9 ]
  */
  drowproda(table1, 3, 3,rowProductedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowProductedTable1_3_3[%d] = %e\n", i, rowProductedTable1_3_3[i]);
  }
  assert(rowProductedTable1_3_3[0] == 28.0);
  assert(rowProductedTable1_3_3[1] == 80.0);
  assert(rowProductedTable1_3_3[2] == 162.0);

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 1 2 3 4 5 6 7 8 9 ]
  */
  drowproda(table1, 1, 9,rowProductedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowProductedTable1_1_9[%d] = %e\n", i, rowProductedTable1_1_9[i]);
    assert(rowProductedTable1_1_9[i] == table1[i]);
  }

  /*
    [ 1 ]
    [ 2 ]
    [ 3 ]
    [ 4 ]
    [ 5 ] =>  [ 362880 ]
    [ 6 ]
    [ 7 ]
    [ 8 ]
    [ 9 ]
  */
  drowproda(table1, 9, 1,rowProductedTable1_9_1);
  assert(rowProductedTable1_9_1[0] == 362880.0);

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 2 12 30 56 90 ]
  */
  drowproda(table2, 2, 5,rowProductedTable2_2_5);
  assert(rowProductedTable2_2_5[0] == 2.0);
  assert(rowProductedTable2_2_5[1] == 12.0);
  assert(rowProductedTable2_2_5[2] == 30.0);
  assert(rowProductedTable2_2_5[3] == 56.0);
  assert(rowProductedTable2_2_5[4] == 90.0);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("rowProductedTable2_2_5[%d] = %e\n", i, rowProductedTable2_2_5[i]);
  }

  /*
    [ 1  6 ]
    [ 2  7 ] => [ 120 30240 ]
    [ 3  8 ]
    [ 4  9 ]
    [ 5 10 ]
  */
  drowproda(table2, 5, 2,rowProductedTable2_5_2);
  assert(rowProductedTable2_5_2[0] == 120.0);
  assert(rowProductedTable2_5_2[1] == 30240.0);
  for ( i = 0 ; i < 2 ; ++i) {
    printf("rowProductedTable2_5_2[%d] = %e\n", i, rowProductedTable2_5_2[i]);
  }

  return 0;
}

static int zprodsTest(void) {
  doubleComplex value1 = DoubleComplex(3.0, 3.0);
  doubleComplex value2 = DoubleComplex(1.123456789, 1.123456789);

  printf("\n>>>> prod Double Complex Scalar Test\n");
  assert(zreals(zprods(value1)) == 3.0);
  assert(zimags(zprods(value1)) == 3.0);
  assert(zreals(zprods(value2)) == 1.123456789);
  assert(zimags(zprods(value2)) == 1.123456789);
  return 0;
}

static int zprodaTest(void) {
  doubleComplex value1 = DoubleComplex(3.0, 3.0);
  doubleComplex table1[3];
  doubleComplex value2 = DoubleComplex(9.186784563,9.186784563);
  doubleComplex table2[5];

  printf("\n>>>> prod Double Complex Array Test\n");
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
  printf("%e\n", zreals(zproda(table1, 3)));
  printf("%e\n", zimags(zproda(table1, 3)));
  printf("%1.20f\n", zreals(zproda(table2, 5)));
  printf("%1.20f\n", zimags(zproda(table2, 5)));
  printf("%1.20f\n", zreals(zproda(table2, 5)) + 261744.55211053110542707);
  printf("--------\n");
#endif
  assert(zreals(zproda(table1, 3)) == -54.0);
  assert(zimags(zproda(table1, 3)) == 54.0);
  assert(fabs(zreals(zproda(table2, 5)) + 261744.55211053110542707) < 3e-16);
  assert(fabs(zimags(zproda(table2, 5)) + 261744.55211053110542707) < 3e-16);

  return 0;
}

static int zrowprodaTest(void) {
  int i = 0;
  doubleComplex in[12];
  doubleComplex rowProductedIn_4_3[3];
  doubleComplex rowProductedIn_3_4[4];
  doubleComplex rowProductedIn_6_2[2];
  doubleComplex rowProductedIn_2_6[6];
  doubleComplex rowProductedIn_1_12[12];
  doubleComplex rowProductedIn_12_1[1];

  printf("\n>>>> Row prod Double Complex Array Test\n");
  /* Init input var */
  for (i = 0 ; i < 12 ; ++i)
    {
      in[i] = DoubleComplex((double) i / 10.0, (11.0 - (double) i) / 10.0);
    }

  /*
    [ 1.1i     0.4+0.7i 0.8+0.3i ]
    [ 0.1+i    0.5+0.6i 0.9+0.2i ] => [ 0.6787000000000000810019 - 0.5456i
    [ 0.2+0.9i 0.6+0.5i 1+0.1i   ]    - 0.3964999999999999635847,
    [ 0.3+0.8i 0.7+0.4i 1.1      ]      0.678700000000000192024 + 0.5456i]
  */
  zrowproda(in, 4, 3, rowProductedIn_4_3);
  for (i = 0 ; i < 3 ; ++i) {
    printf("rowProductedIn_4_3[%d] = %e + %ei\n", i, zreals(rowProductedIn_4_3[i]), zimags(rowProductedIn_4_3[i]));
  }
  assert(fabs(zreals(rowProductedIn_4_3[0]) - 0.6787000000000000810019) < 3e-16);
  assert(fabs(zimags(rowProductedIn_4_3[0]) + 0.5456) < 3e-16);
  assert(fabs(zreals(rowProductedIn_4_3[1]) + 0.3964999999999999635847) < 3e-16);
  assert(fabs(zimags(rowProductedIn_4_3[1]) - 0.0) < 3e-16);
  assert(fabs(zreals(rowProductedIn_4_3[2]) - 0.6787000000000000810019) < 3e-16);
  assert(fabs(zimags(rowProductedIn_4_3[2]) - 0.5456) < 3e-16);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ - 0.3190000000000000612843 - 0.9680000000000000826006i,
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]	  - 0.5380000000000000337508 + 0.0010000000000000563993i,  
								  - 0.0010000000000000008882 + 0.5379999999999999227285i,    
								    0.9680000000000000826006 + 0.3190000000000000612843i ]
  */
  zrowproda(in, 3, 4, rowProductedIn_3_4);
  for (i = 0 ; i < 4 ; ++i) {
    printf("rowProductedIn_3_4[%d] = %e + %ei\n", i, zreals(rowProductedIn_3_4[i]), zimags(rowProductedIn_3_4[i]));
  }
  assert(fabs(zreals(rowProductedIn_3_4[0]) + 0.3190000000000000612843) < 3e-16);
  assert(fabs(zimags(rowProductedIn_3_4[0]) + 0.9680000000000000826006) < 3e-16);
  assert(fabs(zreals(rowProductedIn_3_4[1]) + 0.5380000000000000337508) < 3e-16);
  assert(fabs(zimags(rowProductedIn_3_4[1]) - 0.0010000000000000563993 ) < 3e-16);
  assert(fabs(zreals(rowProductedIn_3_4[2]) + 0.0010000000000000008882) < 3e-16);
  assert(fabs(zimags(rowProductedIn_3_4[2]) - 0.5379999999999999227285) < 3e-16);
  assert(fabs(zreals(rowProductedIn_3_4[3]) - 0.9680000000000000826006) < 3e-16);
  assert(fabs(zimags(rowProductedIn_3_4[3]) - 0.3190000000000000612843) < 3e-16);

  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 0.1725900000000000766853 + 0.5204650000000000664713i,
    [ 0.3+0.8i 0.9+0.2i ]    - 0.1725899999999999934186 + 0.5204649999999999554490i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  zrowproda(in, 6, 2, rowProductedIn_6_2);
  for (i = 0 ; i < 2 ; ++i) {
    printf("rowProductedIn_6_2[%d] = %e + %ei\n", i, zreals(rowProductedIn_6_2[i]), zimags(rowProductedIn_6_2[i]));
  }
  assert(fabs(zreals(rowProductedIn_6_2[0]) - 0.1725900000000000766853) < 3e-16);
  assert(fabs(zimags(rowProductedIn_6_2[0]) - 0.5204650000000000664713) < 3e-16);
  assert(fabs(zreals(rowProductedIn_6_2[1]) + 0.1725899999999999934186) < 3e-16);
  assert(fabs(zimags(rowProductedIn_6_2[1]) - 0.5204649999999999554490 ) < 3e-16);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ - 1.1000000000000000888178 + 0.11i,
    										  - 0.6600000000000001421086 + 0.4300000000000000488498i,
    										  - 0.2199999999999999733547 + 0.5899999999999999689138i,
    										    0.2199999999999999733547 + 0.5899999999999999689138i,
    										    0.6600000000000001421086 + 0.4300000000000000488498i,
    										    1.1000000000000000888178 + 0.11i  ]
  */
  zrowproda(in, 2, 6, rowProductedIn_2_6);
  for (i = 0 ; i < 6 ; ++i) {
    printf("rowProductedIn_2_6[%d] = %e + %ei\n", i, zreals(rowProductedIn_2_6[i]), zimags(rowProductedIn_2_6[i]));
  }
  assert(fabs(zreals(rowProductedIn_2_6[0]) + 1.1000000000000000888178) < 3e-16);
  assert(fabs(zimags(rowProductedIn_2_6[0]) - 0.11) < 3e-16);
  assert(fabs(zreals(rowProductedIn_2_6[1]) + 0.6600000000000001421086) < 3e-16);
  assert(fabs(zimags(rowProductedIn_2_6[1]) - 0.4300000000000000488498 ) < 3e-16);
  assert(fabs(zreals(rowProductedIn_2_6[2]) + 0.2199999999999999733547) < 3e-16);
  assert(fabs(zimags(rowProductedIn_2_6[2]) - 0.5899999999999999689138) < 3e-16);
  assert(fabs(zreals(rowProductedIn_2_6[3]) - 0.2199999999999999733547) < 3e-16);
  assert(fabs(zimags(rowProductedIn_2_6[3]) - 0.5899999999999999689138 ) < 3e-16);
  assert(fabs(zreals(rowProductedIn_2_6[4]) - 0.6600000000000001421086) < 3e-16);
  assert(fabs(zimags(rowProductedIn_2_6[4]) - 0.4300000000000000488498) < 3e-16);
  assert(fabs(zreals(rowProductedIn_2_6[5]) - 1.1000000000000000888178) < 3e-16);
  assert(fabs(zimags(rowProductedIn_2_6[5]) - 0.11 ) < 3e-16);

  /*
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
  */
  zrowproda(in, 1, 12, rowProductedIn_1_12);
  for (i = 0 ; i < 12 ; ++i) {
    printf("rowProductedIn_1_12[%d] = %e + %ei\n", i, zreals(rowProductedIn_1_12[i]), zimags(rowProductedIn_1_12[i]));
    assert(zreals(rowProductedIn_1_12[i]) == zreals(in[i]) && zimags(rowProductedIn_1_12[i]) == zimags(in[i]));
  }

  /*
    [ 1.1i     ] 
    [ 0.1+i    ] 
    [ 0.2+0.9i ] 
    [ 0.3+0.8i ] 
    [ 0.4+0.7i ] 
    [ 0.5+0.6i ]  
    [ 0.6+0.5i ] => [- 0.3006711243250001497351 + 0.0000000000000000038164i ]
    [ 0.7+0.4i ] 
    [ 0.8+0.3i ] 
    [ 0.9+0.2i ] 
    [ 1+0.1i   ] 
    [ 1.1      ] 
 
  */
  zrowproda(in, 12, 1, rowProductedIn_12_1);
  printf("rowProductedIn_12_1[%d] = %e + %ei\n", 0, zreals(rowProductedIn_12_1[0]), zimags(rowProductedIn_12_1[0]));
  assert(fabs(zreals(rowProductedIn_12_1[0]) + 0.3006711243250001497351) < 3e-16);
  assert(fabs(zimags(rowProductedIn_12_1[0]) - 0.0000000000000000038164) < 3e-16);

  return 0;
}

static int zcolumnprodaTest(void) {
  int i = 0;
  doubleComplex in[12];
  doubleComplex columnProductedIn_4_3[4];
  doubleComplex columnProductedIn_3_4[3];
  doubleComplex columnProductedIn_6_2[6];
  doubleComplex columnProductedIn_2_6[2];
  doubleComplex columnProductedIn_1_12[1];
  doubleComplex columnProductedIn_12_1[12];

  printf("\n>>>> Columnprod Double Complex Array Test\n");
  /* Init input var */
  for (i = 0 ; i < 12 ; ++i)
    {
      in[i] = DoubleComplex((double) i / 10.0, (11.0 - (double) i) / 10.0);
    }

  /*
    [ 1.1i     0.4+0.7i 0.8+0.3i ]    [ - 0.7480000000000001092460 + 0.1210000000000001074696i ]
    [ 0.1+i    0.5+0.6i 0.9+0.2i ] => [ - 0.6069999999999999840128 + 0.3940000000000001278977i ] 
    [ 0.2+0.9i 0.6+0.5i 1+0.1i   ]    [ - 0.3940000000000000168754 + 0.6069999999999999840128i ]
    [ 0.3+0.8i 0.7+0.4i 1.1      ]    [ - 0.1210000000000000935918 + 0.7479999999999999982236i ]
  */
  zcolumnproda(in, 4, 3, columnProductedIn_4_3);
  for (i = 0 ; i < 4 ; ++i) {
    printf("columnProductedIn_4_3[%d] = %e + %ei\n", i, zreals(columnProductedIn_4_3[i]), zimags(columnProductedIn_4_3[i]));
  }
  assert(fabs(zreals(columnProductedIn_4_3[0]) + 0.7480000000000001092460) < 3e-16);
  assert(fabs(zimags(columnProductedIn_4_3[0]) - 0.1210000000000001074696) < 3e-16);
  assert(fabs(zreals(columnProductedIn_4_3[1]) + 0.6069999999999999840128) < 3e-16);
  assert(fabs(zimags(columnProductedIn_4_3[1]) - 0.394000000000000127897) < 3e-16);
  assert(fabs(zreals(columnProductedIn_4_3[2]) + 0.3940000000000000168754) < 3e-16);
  assert(fabs(zimags(columnProductedIn_4_3[2]) - 0.606999999999999984012) < 3e-16);
  assert(fabs(zreals(columnProductedIn_4_3[3]) + 0.1210000000000000935918) < 3e-16);
  assert(fabs(zimags(columnProductedIn_4_3[3]) - 0.7479999999999999982236) < 3e-16);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]    [ - 0.5753000000000000335731 - 0.3564000000000000500933i ]  
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ - 0.6564999999999998614442 + 0.0000000000000000693889i ] 
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]    [ - 0.5753000000000001445955 + 0.3564000000000001056044i ]
  */
  zcolumnproda(in, 3, 4, columnProductedIn_3_4);
  for (i = 0 ; i < 3 ; ++i) {
    printf("columnProductedIn_3_4[%d] = %e + %ei\n", i, zreals(columnProductedIn_3_4[i]), zimags(columnProductedIn_3_4[i]));
  }
  assert(fabs(zreals(columnProductedIn_3_4[0]) + 0.5753000000000000335731) < 3e-16);
  assert(fabs(zimags(columnProductedIn_3_4[0]) + 0.3564000000000000500933) < 3e-16);
  assert(fabs(zreals(columnProductedIn_3_4[1]) + 0.6564999999999998614442) < 3e-16);
  assert(fabs(zimags(columnProductedIn_3_4[1]) - 0.0000000000000000693889) < 3e-16);
  assert(fabs(zreals(columnProductedIn_3_4[2]) + 0.5753000000000001445955) < 3e-16);
  assert(fabs(zimags(columnProductedIn_3_4[2]) - 0.3564000000000001056044) < 3e-16);

  /*
    [ 1.1i     0.6+0.5i ]    [ - 0.5500000000000000444089 + 0.6600000000000000310862i ]
    [ 0.1+i    0.7+0.4i ]    [ - 0.3300000000000000155431 + 0.7399999999999999911182i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ - 0.1099999999999999866773 + 0.7800000000000000266454i ]
    [ 0.3+0.8i 0.9+0.2i ]    [   0.1099999999999999866773 + 0.7800000000000000266454i ]
    [ 0.4+0.7i 1+0.1i   ]    [   0.3300000000000000155431 + 0.7399999999999999911182i ]
    [ 0.5+0.6i 1.1      ]    [   0.5500000000000000444089 + 0.6600000000000000310862i ] 
  */
  zcolumnproda(in, 6, 2, columnProductedIn_6_2);
  for (i = 0 ; i < 6 ; ++i) {
    printf("columnProductedIn_6_2[%d] = %e + %ei\n", i, zreals(columnProductedIn_6_2[i]), zimags(columnProductedIn_6_2[i]));
  }
  assert(fabs(zreals(columnProductedIn_6_2[0]) + 0.5500000000000000444089) < 3e-16);
  assert(fabs(zimags(columnProductedIn_6_2[0]) - 0.6600000000000000310862) < 3e-16);
  assert(fabs(zreals(columnProductedIn_6_2[1]) + 0.3300000000000000155431) < 3e-16);
  assert(fabs(zimags(columnProductedIn_6_2[1]) - 0.7399999999999999911182) < 3e-16);
  assert(fabs(zreals(columnProductedIn_6_2[2]) + 0.1099999999999999866773) < 3e-16);
  assert(fabs(zimags(columnProductedIn_6_2[2]) - 0.7800000000000000266454) < 3e-16);
  assert(fabs(zreals(columnProductedIn_6_2[3]) - 0.1099999999999999866773) < 3e-16);
  assert(fabs(zimags(columnProductedIn_6_2[3]) - 0.7800000000000000266454) < 3e-16);
  assert(fabs(zreals(columnProductedIn_6_2[4]) - 0.3300000000000000155431) < 3e-16);
  assert(fabs(zimags(columnProductedIn_6_2[4]) - 0.7399999999999999911182) < 3e-16);
  assert(fabs(zreals(columnProductedIn_6_2[5]) - 0.5500000000000000444089) < 3e-16);
  assert(fabs(zimags(columnProductedIn_6_2[5]) - 0.6600000000000000310862) < 3e-16);

  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]    [   0.2212649999999999894662 - 0.5017100000000001003286i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ - 0.2212649999999999894662 - 0.5017100000000001003286i ] 
  */
  zcolumnproda(in, 2, 6, columnProductedIn_2_6);
  for (i = 0 ; i < 2 ; ++i) {
    printf("columnProductedIn_2_6[%d] = %e + %ei\n", i, zreals(columnProductedIn_2_6[i]), zimags(columnProductedIn_2_6[i]));
  }
  assert(fabs(zreals(columnProductedIn_2_6[0]) - 0.2212649999999999894662) < 3e-16);
  assert(fabs(zimags(columnProductedIn_2_6[0]) + 0.5017100000000001003286) < 3e-16);
  assert(fabs(zreals(columnProductedIn_2_6[1]) + 0.2212649999999999894662) < 3e-16);
  assert(fabs(zimags(columnProductedIn_2_6[1]) + 0.5017100000000001003286) < 3e-16);

  /*
  [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [  - 0.3006711243250001497351 + 0.0000000000000000038164i ]
  */
  zcolumnproda(in, 1, 12, columnProductedIn_1_12);
  printf("columnProductedIn_1_12[%d] = %e + %ei\n", 0, zreals(columnProductedIn_1_12[0]), zimags(columnProductedIn_1_12[0]));
  assert(fabs(zreals(columnProductedIn_1_12[0]) + 0.3006711243250001497351) < 3e-16);
  assert(fabs(zimags(columnProductedIn_1_12[0]) - 0.0000000000000000038164) < 3e-16);


  /*
    [ 1.1i     ] =>     [ 1.1i     ]
    [ 0.1+i    ] =>     [ 0.1+i    ]
    [ 0.2+0.9i ] =>     [ 0.2+0.9i ]
    [ 0.3+0.8i ] =>     [ 0.3+0.8i ]
    [ 0.4+0.7i ] =>     [ 0.4+0.7i ]
    [ 0.5+0.6i ] =>     [ 0.5+0.6i ]
    [ 0.6+0.5i ] =>     [ 0.6+0.5i ]
    [ 0.7+0.4i ] =>     [ 0.7+0.4i ]
    [ 0.8+0.3i ] =>     [ 0.8+0.3i ]
    [ 0.9+0.2i ] =>     [ 0.9+0.2i ]
    [ 1+0.1i   ] =>     [ 1+0.1i   ]
    [ 1.1      ] =>     [ 1.1      ]
  */
  zcolumnproda(in, 12, 1, columnProductedIn_12_1);
 for (i = 0 ; i < 12 ; ++i) {
   printf("columnProductedIn_12_1[%d] = %e + %ei\n", i, zreals(columnProductedIn_12_1[i]), zimags(columnProductedIn_12_1[i]));
   assert(zreals(columnProductedIn_12_1[i]) == zreals(in[i]) && zimags(columnProductedIn_12_1[i]) == zimags(in[i]));
 }

  return 0;
}

static int testprod(void) {

  dprodsTest();
  dprodaTest();
  drowprodaTest();
  dcolumnprodaTest();
  zprodsTest();
  zprodaTest();
  zrowprodaTest();
  zcolumnprodaTest();

  return 0;
}

int main(void) {
  assert(testprod() == 0);
  return 0;
}
