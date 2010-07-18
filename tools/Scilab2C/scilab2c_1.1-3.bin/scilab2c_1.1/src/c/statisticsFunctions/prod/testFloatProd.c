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

 #define LOCAL_DEBUG

static int sprodsTest(void) {
  float value1 = 3.0f;
  float value2 = 1.123456789f;

  printf("\n>>>> Prod Float Scalar Test\n");
  assert(sprods(value1) == 3.0f);
  assert(sprods(value2) == 1.123456789f);

  return 0;
}

static int sprodaTest(void) {
  float table1[3] = {3.0f, 3.0f, 3.0f};
  float table2[5] = {9.186784563f,
		     9.186784563f,
		     9.186784563f,
		     9.186784563f,
		     9.186784563f};

  printf("\n>>>> Prod Float Array Test\n");
  printf("%f\n", sproda(table1, 3));
  assert(sproda(table1, 3) == 27.0f);
  assert((sproda(table2, 5) - (9.186784f * 9.186784f * 9.186784f * 9.186784f * 9.186784f)) / sproda(table2, 5) < 1e-6);
  return 0;
}

static int scolumnprodaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float table2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  float columnProductedTable1_3_3[3] = {0};
  float columnProductedTable1_1_9[1] = {0};
  float columnProductedTable1_9_1[9] = {0};
  float columnProductedTable2_2_5[2] = {0};
  float columnProductedTable2_5_2[5] = {0};

  printf("\n>>>> Column Prod Float Array Test\n");
  /*
    [ 1 2 3 ]    [ 1*2*3=6   ]
    [ 4 5 6 ] => [ 4*5*6=120 ]
    [ 7 8 9 ]    [ 7*8*9=504 ]
  */
  scolumnproda(table1, 3, 3,columnProductedTable1_3_3);
  assert(columnProductedTable1_3_3[0] == 6.0f);
  assert(columnProductedTable1_3_3[1] == 120.0f);
  assert(columnProductedTable1_3_3[2] == 504.0f);

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 362880 ]
  */
  scolumnproda(table1, 1, 9,columnProductedTable1_1_9);
  assert(columnProductedTable1_1_9[0] == 362880.0f);

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
  scolumnproda(table1, 9, 1,columnProductedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnProductedTable1_9_1[%d] = %e\n", i, columnProductedTable1_9_1[i]);
    assert(columnProductedTable1_9_1[i] == table1[i]);
  }

  /*
    [ 1 3 5 7  9 ]    [ 945  ]
    [ 2 4 6 8 10 ] => [ 3840 ]
  */
  scolumnproda(table2, 2, 5,columnProductedTable2_2_5);
  assert(columnProductedTable2_2_5[0] == 945.0f);
  assert(columnProductedTable2_2_5[1] == 3840.0f);
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
  scolumnproda(table2, 5, 2,columnProductedTable2_5_2);
  assert(columnProductedTable2_5_2[0] == 6.0f);
  assert(columnProductedTable2_5_2[1] == 14.0f);
  assert(columnProductedTable2_5_2[2] == 24.0f);
  assert(columnProductedTable2_5_2[3] == 36.0f);
  assert(columnProductedTable2_5_2[4] == 50.0f);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnProductedTable2_5_2[%d] = %e\n", i, columnProductedTable2_5_2[i]);
  }

  return 0;
}

static int srowprodaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float table2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  float rowProductedTable1_3_3[3] = {0};
  float rowProductedTable1_1_9[9] = {0};
  float rowProductedTable1_9_1[1] = {0};
  float rowProductedTable2_2_5[5] = {0};
  float rowProductedTable2_5_2[2] = {0};

  printf("\n>>>> Row Prod Float Array Test\n");
  /*
    [ 1 2 3 ]
    [ 4 5 6 ] => [ 28 80 162 ]
    [ 7 8 9 ]
  */
  srowproda(table1, 3, 3,rowProductedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowProductedTable1_3_3[%d] = %e\n", i, rowProductedTable1_3_3[i]);
  }
  assert(rowProductedTable1_3_3[0] == 28.0f);
  assert(rowProductedTable1_3_3[1] == 80.0f);
  assert(rowProductedTable1_3_3[2] == 162.0f);

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 1 2 3 4 5 6 7 8 9 ]
  */
  srowproda(table1, 1, 9,rowProductedTable1_1_9);
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
  srowproda(table1, 9, 1,rowProductedTable1_9_1);
  assert(rowProductedTable1_9_1[0] == 362880.0f);

  /*
    [ 1 3 5 7  9 ]
    [ 2 4 6 8 10 ] => [ 2 12 30 56 90 ]
  */
  srowproda(table2, 2, 5,rowProductedTable2_2_5);
  assert(rowProductedTable2_2_5[0] == 2.0f);
  assert(rowProductedTable2_2_5[1] == 12.0f);
  assert(rowProductedTable2_2_5[2] == 30.0f);
  assert(rowProductedTable2_2_5[3] == 56.0f);
  assert(rowProductedTable2_2_5[4] == 90.0f);
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
  srowproda(table2, 5, 2,rowProductedTable2_5_2);
  assert(rowProductedTable2_5_2[0] == 120.0f);
  assert(rowProductedTable2_5_2[1] == 30240.0f);
  for ( i = 0 ; i < 2 ; ++i) {
    printf("rowProductedTable2_5_2[%d] = %e\n", i, rowProductedTable2_5_2[i]);
  }

  return 0;
}

static int cprodsTest(void) {
  floatComplex value1 = FloatComplex(3.0f, 3.0f);
  floatComplex value2 = FloatComplex(1.123456789f, 1.123456789f);

  printf("\n>>>> Prod Float Complex Scalar Test\n");
  assert(creals(cprods(value1)) == 3.0f);
  assert(cimags(cprods(value1)) == 3.0f);
  assert(creals(cprods(value2)) == 1.123456789f);
  assert(cimags(cprods(value2)) == 1.123456789f);
  return 0;
}

static int cprodaTest(void) {
  floatComplex value1 = FloatComplex(3.0f, 3.0f);
  floatComplex table1[3];
  floatComplex value2 = FloatComplex(9.186784563f,9.186784563f);
  floatComplex table2[5];

  printf("\n>>>> Prod Float Complex Array Test\n");
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
  printf("%e\n", creals(cproda(table1, 3)));
  printf("%e\n", cimags(cproda(table1, 3)));
  printf("%1.20f\n", creals(cproda(table2, 5)));
  printf("%1.20f\n", cimags(cproda(table2, 5)));
  printf("%1.20f\n", creals(cproda(table2, 5)) + 261744.55211053110542707f);
  printf("--------\n");
#endif

  assert(creals(cproda(table1, 3)) == -54.0f);
  assert(cimags(cproda(table1, 3)) == 54.0f);
  assert(fabs(creals(cproda(table2, 5)) + 261744.55211053110542707f)/creals(cproda(table2, 5)) < 1e-06);
  assert(fabs(cimags(cproda(table2, 5)) + 261744.55211053110542707f)/cimags(cproda(table2, 5)) < 1e-06);

  return 0;
}

static int crowprodaTest(void) {
  int i = 0;
  floatComplex in[12];
  floatComplex rowProductedIn_4_3[3];
  floatComplex rowProductedIn_3_4[4];
  floatComplex rowProductedIn_6_2[2];
  floatComplex rowProductedIn_2_6[6];
  floatComplex rowProductedIn_1_12[12];
  floatComplex rowProductedIn_12_1[1];

  printf("\n>>>> Row Prod Float Complex Array Test\n");
  /* Init input var */
  for (i = 0 ; i < 12 ; ++i)
    {
      in[i] = FloatComplex((float) i / 10.0f, (11.0f - (float) i) / 10.0f);
    }

  /*
    [ 1.1i     0.4+0.7i 0.8+0.3i ]
    [ 0.1+i    0.5+0.6i 0.9+0.2i ] => [ 0.6787000000000000810019 - 0.5456i
    [ 0.2+0.9i 0.6+0.5i 1+0.1i   ]    - 0.3964999999999999635847,
    [ 0.3+0.8i 0.7+0.4i 1.1      ]      0.678700000000000192024 + 0.5456i]
  */
  crowproda(in, 4, 3, rowProductedIn_4_3);
  for (i = 0 ; i < 3 ; ++i) {
    printf("rowProductedIn_4_3[%d] = %e + %ei\n", i, creals(rowProductedIn_4_3[i]), cimags(rowProductedIn_4_3[i]));
  }
  assert(fabs(creals(rowProductedIn_4_3[0]) - 0.6787000000000000810019f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_4_3[0]) + 0.5456f) < 1e-06);
  assert(fabs(creals(rowProductedIn_4_3[1]) + 0.3964999999999999635847f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_4_3[1]) - 0.0f) < 1e-06);
  assert(fabs(creals(rowProductedIn_4_3[2]) - 0.6787000000000000810019f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_4_3[2]) - 0.5456f) < 1e-06);
  
  
  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ - 0.3190000000000000612843 - 0.9680000000000000826006i,
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]	  - 0.5380000000000000337508 + 0.0010000000000000563993i,  
								  - 0.0010000000000000008882 + 0.5379999999999999227285i,    
								    0.9680000000000000826006 + 0.3190000000000000612843i ]
  */
  crowproda(in, 3, 4, rowProductedIn_3_4);
  for (i = 0 ; i < 4 ; ++i) {
    printf("rowProductedIn_3_4[%d] = %e + %ei\n", i, creals(rowProductedIn_3_4[i]), cimags(rowProductedIn_3_4[i]));
  }
  assert(fabs(creals(rowProductedIn_3_4[0]) + 0.3190000000000000612843f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_3_4[0]) + 0.9680000000000000826006f) < 1e-06);
  assert(fabs(creals(rowProductedIn_3_4[1]) + 0.5380000000000000337508f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_3_4[1]) - 0.0010000000000000563993 ) < 1e-06);
  assert(fabs(creals(rowProductedIn_3_4[2]) + 0.0010000000000000008882f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_3_4[2]) - 0.5379999999999999227285f) < 1e-06);
  assert(fabs(creals(rowProductedIn_3_4[3]) - 0.9680000000000000826006f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_3_4[3]) - 0.3190000000000000612843f) < 1e-06);
  
  
  /*
    [ 1.1i     0.6+0.5i ]
    [ 0.1+i    0.7+0.4i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ 0.1725900000000000766853 + 0.5204650000000000664713i,
    [ 0.3+0.8i 0.9+0.2i ]    - 0.1725899999999999934186 + 0.5204649999999999554490i ]
    [ 0.4+0.7i 1+0.1i   ]
    [ 0.5+0.6i 1.1      ]
  */
  crowproda(in, 6, 2, rowProductedIn_6_2);
  for (i = 0 ; i < 2 ; ++i) {
    printf("rowProductedIn_6_2[%d] = %e + %ei\n", i, creals(rowProductedIn_6_2[i]), cimags(rowProductedIn_6_2[i]));
  }
  
  assert(fabs(creals(rowProductedIn_6_2[0]) - 0.1725900000000000766853f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_6_2[0]) - 0.5204650000000000664713f) < 1e-06);
  assert(fabs(creals(rowProductedIn_6_2[1]) + 0.1725899999999999934186f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_6_2[1]) - 0.5204649999999999554490 ) < 1e-06);


  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ - 1.1000000000000000888178 + 0.11i,
    										  - 0.6600000000000001421086 + 0.4300000000000000488498i,
    										  - 0.2199999999999999733547 + 0.5899999999999999689138i,
    										    0.2199999999999999733547 + 0.5899999999999999689138i,
    										    0.6600000000000001421086 + 0.4300000000000000488498i,
    										    1.1000000000000000888178 + 0.11i  ]
 
  */
  crowproda(in, 2, 6, rowProductedIn_2_6);
  for (i = 0 ; i < 6 ; ++i) {
    printf("rowProductedIn_2_6[%d] = %e + %ei\n", i, creals(rowProductedIn_2_6[i]), cimags(rowProductedIn_2_6[i]));
  }
  
  assert(fabs(creals(rowProductedIn_2_6[0]) + 1.1000000000000000888178f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_2_6[0]) - 0.11f) < 1e-06);
  assert(fabs(creals(rowProductedIn_2_6[1]) + 0.6600000000000001421086f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_2_6[1]) - 0.4300000000000000488498 ) < 1e-06);
  assert(fabs(creals(rowProductedIn_2_6[2]) + 0.2199999999999999733547f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_2_6[2]) - 0.5899999999999999689138f) < 1e-06);
  assert(fabs(creals(rowProductedIn_2_6[3]) - 0.2199999999999999733547f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_2_6[3]) - 0.5899999999999999689138 ) < 1e-06);
  assert(fabs(creals(rowProductedIn_2_6[4]) - 0.6600000000000001421086f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_2_6[4]) - 0.4300000000000000488498f) < 1e-06);
  assert(fabs(creals(rowProductedIn_2_6[5]) - 1.1000000000000000888178f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_2_6[5]) - 0.11 ) < 1e-06);


  /*
   [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]   
    =>  [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]   
  */
  crowproda(in, 1, 12, rowProductedIn_1_12);
  for (i = 0 ; i < 12 ; ++i) {
    printf("rowProductedIn_1_12[%d] = %e + %ei\n", i, creals(rowProductedIn_1_12[i]), cimags(rowProductedIn_1_12[i]));
    assert(creals(rowProductedIn_1_12[i]) == creals(in[i]) && cimags(rowProductedIn_1_12[i]) == cimags(in[i]));
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
  crowproda(in, 12, 1, rowProductedIn_12_1);
  printf("rowProductedIn_12_1[%d] = %e + %ei\n", 0, creals(rowProductedIn_12_1[0]), cimags(rowProductedIn_12_1[0]));
  assert(fabs(creals(rowProductedIn_12_1[0]) + 0.3006711243250001497351f) < 1e-06);
  assert(fabs(cimags(rowProductedIn_12_1[0]) - 0.0000000000000000038164f) < 1e-06);

  return 0;
}

static int ccolumnprodaTest(void) {
  int i = 0;
  floatComplex in[12];
  floatComplex columnProductedIn_4_3[4];
  floatComplex columnProductedIn_3_4[3];
  floatComplex columnProductedIn_6_2[6];
  floatComplex columnProductedIn_2_6[2];
  floatComplex columnProductedIn_1_12[1];
  floatComplex columnProductedIn_12_1[12];

  printf("\n>>>> ColumnProd Float Complex Array Test\n");
  /* Init input var */
  for (i = 0 ; i < 12 ; ++i)
    {
      in[i] = FloatComplex((float) i / 10.0f, (11.0f - (float) i) / 10.0f);
    }

  /*
    [ 1.1i     0.4+0.7i 0.8+0.3i ]    [ - 0.7480000000000001092460 + 0.1210000000000001074696i ]
    [ 0.1+i    0.5+0.6i 0.9+0.2i ] => [ - 0.6069999999999999840128 + 0.3940000000000001278977i ] 
    [ 0.2+0.9i 0.6+0.5i 1+0.1i   ]    [ - 0.3940000000000000168754 + 0.6069999999999999840128i ]
    [ 0.3+0.8i 0.7+0.4i 1.1      ]    [ - 0.1210000000000000935918 + 0.7479999999999999982236i ]
  */
  ccolumnproda(in, 4, 3, columnProductedIn_4_3);
  for (i = 0 ; i < 4 ; ++i) {
    printf("columnProductedIn_4_3[%d] = %e + %ei\n", i, creals(columnProductedIn_4_3[i]), cimags(columnProductedIn_4_3[i]));
  }
  assert(fabs(creals(columnProductedIn_4_3[0]) + 0.7480000000000001092460f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_4_3[0]) - 0.1210000000000001074696f) < 1e-06);
  assert(fabs(creals(columnProductedIn_4_3[1]) + 0.6069999999999999840128f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_4_3[1]) - 0.394000000000000127897f) < 1e-06);
  assert(fabs(creals(columnProductedIn_4_3[2]) + 0.3940000000000000168754f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_4_3[2]) - 0.606999999999999984012f) < 1e-06);
  assert(fabs(creals(columnProductedIn_4_3[3]) + 0.1210000000000000935918f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_4_3[3]) - 0.7479999999999999982236f) < 1e-06);

  /*
    [ 1.1i     0.3+0.8i  0.6+0.5i 0.9+0.2i ]    [ - 0.5753000000000000335731 - 0.3564000000000000500933i ]  
    [ 0.1+i    0.4+0.7i  0.7+0.4i 1+0.1i   ] => [ - 0.6564999999999998614442 + 0.0000000000000000693889i ] 
    [ 0.2+0.9i 0.5+0.6i  0.8+0.3i 1.1      ]    [ - 0.5753000000000001445955 + 0.3564000000000001056044i ]
  */
  ccolumnproda(in, 3, 4, columnProductedIn_3_4);
  for (i = 0 ; i < 3 ; ++i) {
    printf("columnProductedIn_3_4[%d] = %e + %ei\n", i, creals(columnProductedIn_3_4[i]), cimags(columnProductedIn_3_4[i]));
  }
  assert(fabs(creals(columnProductedIn_3_4[0]) + 0.5753000000000000335731f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_3_4[0]) + 0.3564000000000000500933f) < 1e-06);
  assert(fabs(creals(columnProductedIn_3_4[1]) + 0.6564999999999998614442f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_3_4[1]) - 0.0000000000000000693889f) < 1e-06);
  assert(fabs(creals(columnProductedIn_3_4[2]) + 0.5753000000000001445955f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_3_4[2]) - 0.3564000000000001056044f) < 1e-06);

  /*
    [ 1.1i     0.6+0.5i ]    [ - 0.5500000000000000444089 + 0.6600000000000000310862i ]
    [ 0.1+i    0.7+0.4i ]    [ - 0.3300000000000000155431 + 0.7399999999999999911182i ]
    [ 0.2+0.9i 0.8+0.3i ] => [ - 0.1099999999999999866773 + 0.7800000000000000266454i ]
    [ 0.3+0.8i 0.9+0.2i ]    [   0.1099999999999999866773 + 0.7800000000000000266454i ]
    [ 0.4+0.7i 1+0.1i   ]    [   0.3300000000000000155431 + 0.7399999999999999911182i ]
    [ 0.5+0.6i 1.1      ]    [   0.5500000000000000444089 + 0.6600000000000000310862i ] 
  */
  ccolumnproda(in, 6, 2, columnProductedIn_6_2);
  for (i = 0 ; i < 6 ; ++i) {
    printf("columnProductedIn_6_2[%d] = %e + %ei\n", i, creals(columnProductedIn_6_2[i]), cimags(columnProductedIn_6_2[i]));
  }
  assert(fabs(creals(columnProductedIn_6_2[0]) + 0.5500000000000000444089f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_6_2[0]) - 0.6600000000000000310862f) < 1e-06);
  assert(fabs(creals(columnProductedIn_6_2[1]) + 0.3300000000000000155431f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_6_2[1]) - 0.7399999999999999911182f) < 1e-06);
  assert(fabs(creals(columnProductedIn_6_2[2]) + 0.1099999999999999866773f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_6_2[2]) - 0.7800000000000000266454f) < 1e-06);
  assert(fabs(creals(columnProductedIn_6_2[3]) - 0.1099999999999999866773f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_6_2[3]) - 0.7800000000000000266454f) < 1e-06);
  assert(fabs(creals(columnProductedIn_6_2[4]) - 0.3300000000000000155431f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_6_2[4]) - 0.7399999999999999911182f) < 1e-06);
  assert(fabs(creals(columnProductedIn_6_2[5]) - 0.5500000000000000444089f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_6_2[5]) - 0.6600000000000000310862f) < 1e-06);
  
  /*
    [ 1.1i  0.2+0.9i 0.4+0.7i 0.6+0.5i 0.8+0.3i 1+0.1i ]    [   0.2212649999999999894662 - 0.5017100000000001003286i ]
    [ 0.1+i 0.3+0.8i 0.5+0.6i 0.7+0.4i 0.9+0.2i 1.1    ] => [ - 0.2212649999999999894662 - 0.5017100000000001003286i ] 
  */
  ccolumnproda(in, 2, 6, columnProductedIn_2_6);
  for (i = 0 ; i < 2 ; ++i) {
    printf("columnProductedIn_2_6[%d] = %e + %ei\n", i, creals(columnProductedIn_2_6[i]), cimags(columnProductedIn_2_6[i]));
  }
  assert(fabs(creals(columnProductedIn_2_6[0]) - 0.2212649999999999894662f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_2_6[0]) + 0.5017100000000001003286f) < 1e-06);
  assert(fabs(creals(columnProductedIn_2_6[1]) + 0.2212649999999999894662f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_2_6[1]) + 0.5017100000000001003286f) < 1e-06);

  /* 
  [ 1.1i 0.1+i 0.2+0.9i  0.3+0.8i 0.4+0.7i 0.5+0.6i 0.6+0.5i 0.7+0.4i 0.8+0.3i 0.9+0.2i 1+0.1i 1.1 ]
    =>
    [  - 0.3006711243250001497351 + 0.0000000000000000038164i ]

  */
  ccolumnproda(in, 1, 12, columnProductedIn_1_12);
  printf("columnProductedIn_1_12[%d] = %e + %ei\n", 0, creals(columnProductedIn_1_12[0]), cimags(columnProductedIn_1_12[0]));
  assert(fabs(creals(columnProductedIn_1_12[0]) + 0.3006711243250001497351f) < 1e-06);
  assert(fabs(cimags(columnProductedIn_1_12[0]) - 0.0000000000000000038164f) < 1e-06);


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
  ccolumnproda(in, 12, 1, columnProductedIn_12_1);
 for (i = 0 ; i < 12 ; ++i) {
   printf("columnProductedIn_12_1[%d] = %e + %ei\n", i, creals(columnProductedIn_12_1[i]), cimags(columnProductedIn_12_1[i]));
   assert(creals(columnProductedIn_12_1[i]) == creals(in[i]) && cimags(columnProductedIn_12_1[i]) == cimags(in[i]));
 }

  return 0;
}

static int testProd(void) {

  sprodsTest();
  sprodaTest();
  srowprodaTest();
  scolumnprodaTest();
  cprodsTest();
  cprodaTest();
  crowprodaTest();
  ccolumnprodaTest();

  return 0;
}

int main(void) {
  assert(testProd() == 0);
  return 0;
}
