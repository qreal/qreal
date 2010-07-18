/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan SIMON
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
#include "meanf.h"

/* #define LOCAL_DEBUG */

#define ERROR(x) printf("diff = %e\n", x)

static int dmeanfsTest(void) {
  double value1 = 3.0;
  double value2 = 1.123456789;
  double coef1  = 56 ;
  double coef2  = 2 ;

  coef1  = 56 ;
  coef2  = 2 ;

  printf("\n>>>> Meanf Double Scalar Test\n");
  assert( ( fabs(dmeanfs(value1,coef1) ) - (  3.0 ) ) / fabs ( dmeanfs(value1,coef1)  ) <  3e-16 );
  assert( ( fabs(dmeanfs(value2,coef2) ) - (  1.123456789 ) ) / fabs ( dmeanfs(value2,coef2)  ) <  3e-16 );

  return 0;
}

static int dmeanfaTest(void) {
  double table1[3] = {3.0, 3.0, 3.0};
  double coef1[3] = {10.0, 2.0, 6.0}; 

  double table2[5] = {3.186784563,
		     4.186784563,
		     5.186784563,
		     6.186784563,
		     7.186784563};

  double coef2[5] = {3.0,
		     4.0,
		     5.0,
		     6.0,
		     7.0};

  printf("\n>>>> Meanf Float Array Test\n");
  assert(dmeanfa(table1, 3, coef1) == 54.0 / 18.0);
  assert((dmeanfa(table2, 5, coef2) - (3.186784563*3.0 + 4.186784563*4.0 + 5.186784563*5.0 + 6.186784563*6.0  + 7.186784563*7.0) / 25.0) / dmeanfa(table2, 5 ,coef2) < 3e-16);
  return 0;
}


static int dcolumnmeanfaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double coef1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double table2[10] = {1.0,   2.0,  3.0,  4.0 ,  5.0,  6.0,  7.0,  8.0,  9.0, 10.0};
  double coef2[10] =  {11.0, 12.0, 13.0, 14.0 , 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
  double columnMeanmedTable1_3_3[3] = {0};
  double columnMeanmedTable1_1_9[1] = {0};
  double columnMeanmedTable1_9_1[9] = {0};
  double columnMeanmedTable2_2_5[2] = {0};
  double columnMeanmedTable2_5_2[5] = {0};

  printf("\n>>>> Column Mean Double Array Test\n");
  /*
    [ 1 2 3 ]  [10 11 12 ]
    [ 4 5 6 ].*[ 1  2  3 ]  => [ 68 32 146 ]
    [ 7 8 9 ]  [ 5  6  7 ]
  */
  dcolumnmeanfa(table1, 3, 3, coef1 ,columnMeanmedTable1_3_3);
  assert( ( fabs(columnMeanmedTable1_3_3[0] ) - (  68.0 /  33.0 ) ) / fabs ( columnMeanmedTable1_3_3[0]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable1_3_3[1] ) - (  32.0 /   6.0 ) ) / fabs ( columnMeanmedTable1_3_3[1]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable1_3_3[2] ) - (  146.0 / 18.0 ) ) / fabs ( columnMeanmedTable1_3_3[2]  ) <  3e-16 );

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  dcolumnmeanfa(table1, 1, 9, coef1 ,columnMeanmedTable1_1_9);
  assert( ( fabs(columnMeanmedTable1_1_9[0] ) - (  246.0 / 57.0 ) ) / fabs ( columnMeanmedTable1_1_9[0]  ) <  3e-16 );

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
  dcolumnmeanfa(table1, 9, 1, coef1, columnMeanmedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnMeanmedTable1_9_1[%d] = %e\n", i, columnMeanmedTable1_9_1[i]);
    assert(columnMeanmedTable1_9_1[i] == table1[i]);
  }

  /*
    [ 1 3 5 7  9 ] .* [ 11 13 15 17 19 ]
    [ 2 4 6 8 10 ]    [ 12 14 16 18 20 ]=> [ 415 520 ]
  */
  dcolumnmeanfa(table2, 2, 5, coef2 , columnMeanmedTable2_2_5);
  assert( ( fabs(columnMeanmedTable2_2_5[0] ) - (  415.0 / 75.0 ) ) / fabs ( columnMeanmedTable2_2_5[0]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_2_5[1] ) - (  520.0 / 80.0 ) ) / fabs ( columnMeanmedTable2_2_5[1]  ) <  3e-16 );
  for ( i = 0 ; i < 2 ; ++i) {
    printf("columnMeanmedTable2_2_5[%d] = %e\n", i, columnMeanmedTable2_2_5[i]);
  }

  /*
    [ 1  6 ]  [ 11  16 ]
    [ 2  7 ]  [ 12  17 ] => [ 107 143 183 227 275 ]
    [ 3  8 ].*[ 13  18 ]
    [ 4  9 ]  [ 14  19 ]
    [ 5 10 ]  [ 15  20 ]
  */
  dcolumnmeanfa(table2, 5, 2, coef2 ,columnMeanmedTable2_5_2);
  assert( ( fabs(columnMeanmedTable2_5_2[0] ) - (  107.0 / 27.0 ) ) / fabs ( columnMeanmedTable2_5_2[0]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_5_2[1] ) - (  143.0 / 29.0 ) ) / fabs ( columnMeanmedTable2_5_2[1]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_5_2[2] ) - (  183.0 / 31.0 ) ) / fabs ( columnMeanmedTable2_5_2[2]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_5_2[3] ) - (  227.0 / 33.0 ) ) / fabs ( columnMeanmedTable2_5_2[3]  ) <  3e-16 );
  assert( ( fabs(columnMeanmedTable2_5_2[4] ) - (  275.0 / 35.0 ) ) / fabs ( columnMeanmedTable2_5_2[4]  ) <  3e-16 );
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnMeanmedTable2_5_2[%d] = %e\n", i, columnMeanmedTable2_5_2[i]);
  }

  return 0;
}


static int drowmeanfaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double coef1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double table2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  double coef2[10] =  {11.0, 12.0, 13.0, 14.0 , 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
  double rowMeanmedTable1_3_3[3] = {0};
  double rowMeanmedTable1_1_9[9] = {0};
  double rowMeanmedTable1_9_1[1] = {0};
  double rowMeanmedTable2_2_5[5] = {0};


  printf("\n>>>> Row Mean Double Array Test\n");
  /*
    [ 1 2 3 ]  [10 11 12 ]
    [ 4 5 6 ].*[ 1  2  3 ]  => [ 49 80 117 ]
    [ 7 8 9 ]  [ 5  6  7 ]
  */
  drowmeanfa(table1, 3, 3, coef1 , rowMeanmedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowMeanmedTable1_3_3[%d] = %e\n", i, rowMeanmedTable1_3_3[i]);
  }
  assert( ( fabs(rowMeanmedTable1_3_3[0] ) - (  49.0 / 16.0 ) ) / fabs ( rowMeanmedTable1_3_3[0]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable1_3_3[1] ) - (  80.0 / 19.0 ) ) / fabs ( rowMeanmedTable1_3_3[1]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable1_3_3[2] ) - ( 117.0 / 22.0 ) ) / fabs ( rowMeanmedTable1_3_3[2]  ) <  3e-16 );

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
  drowmeanfa(table1, 1, 9, coef1, rowMeanmedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowMeanmedTable1_1_9[%d] = %e\n", i, rowMeanmedTable1_1_9[i]);
    assert(rowMeanmedTable1_1_9[i] == table1[i]);
  }

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 246 ]
  */
  drowmeanfa(table1, 9, 1, coef1, rowMeanmedTable1_9_1);
  assert( ( fabs(rowMeanmedTable1_9_1[0] ) - (  246.0 / 57.0 ) ) / fabs ( rowMeanmedTable1_9_1[0]  ) <  3e-16 );

  /*
    [ 1 3 5 7  9 ] .* [ 11 13 15 17 19 ]
    [ 2 4 6 8 10 ]    [ 12 14 16 18 20 ] => [ 3 7 11 15 19 ]
  */
  drowmeanfa(table2, 2, 5, coef2, rowMeanmedTable2_2_5);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("rowMeanmedTable2_2_5[%d] = %e\n", i, rowMeanmedTable2_2_5[i]);
  }
  assert( ( fabs(rowMeanmedTable2_2_5[0] ) - (  35.0 / 23.0 ) ) / fabs ( rowMeanmedTable2_2_5[0]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_2_5[1] ) - (  95.0 / 27.0 ) ) / fabs ( rowMeanmedTable2_2_5[1]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_2_5[2] ) - ( 171.0 / 31.0 ) ) / fabs ( rowMeanmedTable2_2_5[2]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_2_5[3] ) - ( 263.0 / 35.0 ) ) / fabs ( rowMeanmedTable2_2_5[3]  ) <  3e-16 );
  assert( ( fabs(rowMeanmedTable2_2_5[4] ) - ( 371.0 / 39.0 ) ) / fabs ( rowMeanmedTable2_2_5[4]  ) <  3e-16 );


  return 0;
}

static int zmeanfsTest(void) {
  doubleComplex value1 = DoubleComplex(3.0, 3.0);
  doubleComplex coef1 = DoubleComplex(3.0, 0.0);
  doubleComplex value2 = DoubleComplex(1.123456789, 1.123456789);
  doubleComplex coef2 = DoubleComplex(9.0, 0.0);

  coef1 = DoubleComplex(3.0, 0.0);
  coef2 = DoubleComplex(9.0, 0.0);

  printf("\n>>>> Mean Double Complex Scalar Test\n");
  assert( ( fabs(zreals(zmeanfs(value1,coef1)) ) - (  3.0 ) ) / fabs ( zreals(zmeanfs(value1,coef1))  ) <  3e-16 );
  assert( ( fabs(zimags(zmeanfs(value1,coef1)) ) - (  3.0 ) ) / fabs ( zimags(zmeanfs(value1,coef1))  ) <  3e-16 );
  assert( ( fabs(zreals(zmeanfs(value2,coef1)) ) - (  1.123456789 ) ) / fabs ( zreals(zmeanfs(value2,coef1))  ) <  3e-16 );
  assert( ( fabs(zimags(zmeanfs(value2,coef1)) ) - (  1.123456789 ) ) / fabs ( zimags(zmeanfs(value2,coef1))  ) <  3e-16 );
  return 0;
}



static int zmeanfaTest(void) {

  double tableR1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI1[9] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0};
  double coefR1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double coefI1[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 

  doubleComplex* table1 = DoubleComplexMatrix (tableR1, tableI1, 9);
  doubleComplex* coef1 = DoubleComplexMatrix (coefR1, coefI1, 9);


  printf("\n>>>> Mean Double Complex Array Test\n");
  

  assert( ( fabs(zreals(zmeanfa(table1, 3, coef1)) ) - (  246.0 /  57.0 ) ) / fabs ( zreals(zmeanfa(table1, 3, coef1))  ) <  3e-16 );
  assert( ( fabs(zimags(zmeanfa(table1, 3, coef1)) ) - (  288.0 /  57.0 ) ) / fabs ( zimags(zmeanfa(table1, 3, coef1))  ) <  3e-16 );/*
*/
  return 0;
}

static int zrowmeanfaTest(void) {

  int i = 0 ;

  double tableR1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI1[9] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0};
  double coefR1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double coefI1[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 

  double tableR2[10] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0, 10.0};
  double tableI2[10] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0};
  double coefR2[10]  = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0}; 
  double coefI2[10]   ={10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0, 19.0};

  doubleComplex* table1 = DoubleComplexMatrix (tableR1, tableI1, 9);
  doubleComplex* coef1 = DoubleComplexMatrix (coefR1, coefI1, 9);

  doubleComplex* table2 = DoubleComplexMatrix (tableR2, tableI2, 10);
  doubleComplex* coef2 = DoubleComplexMatrix (coefR2, coefI2, 10);

  doubleComplex rowMeanmedTable1_3_3[3];
  doubleComplex rowMeanmedTable2_2_5[5];

  rowMeanmedTable1_3_3[0] = DoubleComplex(0.0, 0.0);
  rowMeanmedTable1_3_3[1] = DoubleComplex(0.0, 0.0);
  rowMeanmedTable1_3_3[2] = DoubleComplex(0.0, 0.0);

  rowMeanmedTable2_2_5[0] = DoubleComplex(0.0, 0.0);
  rowMeanmedTable2_2_5[1] = DoubleComplex(0.0, 0.0);
  rowMeanmedTable2_2_5[2] = DoubleComplex(0.0, 0.0);
  rowMeanmedTable2_2_5[3] = DoubleComplex(0.0, 0.0);
  rowMeanmedTable2_2_5[4] = DoubleComplex(0.0, 0.0);

 /*
  doubleComplex rowMeanmedTable1_1_9[9] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable1_9_1[1] = {DoubleComplex(0.0, 0.0)};

  doubleComplex rowMeanmedTable2_5_2[2] = {DoubleComplex(0.0, 0.0)};
*/

  printf("\n>>>> Row Meanf Double Complex Array Test\n");

  /**/

  zrowmeanfa(table1 , 3 , 3 , coef1 , rowMeanmedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {

      printf("rowMeanmedTable_3_3[%d] = %e + %ei\n", i, zreals(rowMeanmedTable1_3_3[i]), zimags(rowMeanmedTable1_3_3[i]));
    }
  assert(  fabs(zimags(rowMeanmedTable1_3_3[0])  - ( 27.0 / 16.0 ) ) / fabs ( zimags(rowMeanmedTable1_3_3[0])  ) <  3e-16 );
  assert(  fabs(zreals(rowMeanmedTable1_3_3[0])  - ( 49.0 / 16.0 ) ) / fabs ( zreals(rowMeanmedTable1_3_3[0])  ) <  3e-16 );
  
  assert(  fabs(zimags(rowMeanmedTable1_3_3[1])  - ( 90.0 / 19.0 ) ) / fabs ( zimags(rowMeanmedTable1_3_3[1])  ) <  3e-16 );
  assert(  fabs(zreals(rowMeanmedTable1_3_3[1])  - ( 80.0 / 19.0 ) ) / fabs ( zreals(rowMeanmedTable1_3_3[1])  ) <  3e-16 );

  assert(  fabs(zimags(rowMeanmedTable1_3_3[2])  - (171.0 / 22.0 ) ) / fabs ( zimags(rowMeanmedTable1_3_3[2])  ) <  3e-16 );
  assert(  fabs(zreals(rowMeanmedTable1_3_3[2])  - (117.0 / 22.0 ) ) / fabs ( zreals(rowMeanmedTable1_3_3[2])  ) <  3e-16 );
/**/

  printf("\n\n");
  zrowmeanfa(table2 , 2 , 5 , coef2 , rowMeanmedTable2_2_5);

  for (i = 0 ; i < 5 ; ++i )
    {
      printf("rowMeanmedTable_2_5[%d] = %e + %ei\n", i, zreals(rowMeanmedTable2_2_5[i]), zimags(rowMeanmedTable2_2_5[i]));
    }
  assert( fabs(zreals(rowMeanmedTable2_2_5[0])  - 1.2727273 ) / fabs ( zreals(rowMeanmedTable2_2_5[0])  ) <  1e-6 );
  assert( zimags(rowMeanmedTable2_2_5[0])  == 0);

  assert( fabs(zreals(rowMeanmedTable2_2_5[1])  - 3.5625000 ) / fabs ( zreals(rowMeanmedTable2_2_5[1])  ) <  1e-6 );
  assert( zimags(rowMeanmedTable2_2_5[1])  == 0);

  assert( fabs(zreals(rowMeanmedTable2_2_5[2])  - 7.2500000 ) / fabs ( zreals(rowMeanmedTable2_2_5[2])  ) <  1e-6 );
  assert( zimags(rowMeanmedTable2_2_5[2])  == 0);

  assert( fabs(zreals(rowMeanmedTable2_2_5[3])  - 3.6000000 ) / fabs ( zreals(rowMeanmedTable2_2_5[3])  ) <  1e-6 );
  assert( zimags(rowMeanmedTable2_2_5[3])  == 0);

  assert( fabs(zreals(rowMeanmedTable2_2_5[4])  - 9.7307692 ) / fabs ( zreals(rowMeanmedTable2_2_5[4])  ) <  1e-6 );
 assert( zimags(rowMeanmedTable2_2_5[4])  == 0);


  return 0;
}


static int zcolumnmeanfaTest(void) {

  int i = 0 ;

  double tableR1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI1[9] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0};
  double coefR1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double coefI1[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 
/*
  double tableR2[10] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0, 10.0};
  double tableI2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 15.0};
  double coefR2[10]  = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0, 19.0}; 
  double coefI2[10]   = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
*/
  doubleComplex* table1 = DoubleComplexMatrix (tableR1, tableI1, 9);
  doubleComplex* coef1 = DoubleComplexMatrix (coefR1, coefI1, 9);
/*
  doubleComplex* table2 = DoubleComplexMatrix (tableR2, tableI2, 9);
  doubleComplex* coef2 = DoubleComplexMatrix (coefR2, coefI2, 9);
*/
  doubleComplex columnMeanmedTable1_3_3[3] ;
 /*
  doubleComplex rowMeanmedTable1_1_9[9] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable1_9_1[1] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable2_2_5[5] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable2_5_2[2] = {DoubleComplex(0.0, 0.0)};
*/

  columnMeanmedTable1_3_3[0] = DoubleComplex(0.0, 0.0);
  columnMeanmedTable1_3_3[1] = DoubleComplex(0.0, 0.0);
  columnMeanmedTable1_3_3[2] = DoubleComplex(0.0, 0.0);

  printf("\n>>>> Column Meanf Double Complex Array Test\n");

  /**/

  zcolumnmeanfa(table1 , 3 , 3 , coef1 , columnMeanmedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {
      printf("columnMeanmedTable_3_3[%d] = %e + %ei\n", i, zreals(columnMeanmedTable1_3_3[i]), zimags(columnMeanmedTable1_3_3[i]));
    }
  assert( ( fabs(zimags(columnMeanmedTable1_3_3[0]) ) - (138.0 / 33.0 ) ) / fabs ( zimags(columnMeanmedTable1_3_3[0])  ) <  3e-16 );
  assert( ( fabs(zreals(columnMeanmedTable1_3_3[0]) ) - ( 68.0 / 33.0 ) ) / fabs ( zreals(columnMeanmedTable1_3_3[0])  ) <  3e-16 );
  
  assert( ( fabs(zimags(columnMeanmedTable1_3_3[1]) ) - ( 36.0 /  6.0 ) ) / fabs ( zimags(columnMeanmedTable1_3_3[1])  ) <  3e-16 );
  assert( ( fabs(zreals(columnMeanmedTable1_3_3[1]) ) - ( 32.0 /  6.0 ) ) / fabs ( zreals(columnMeanmedTable1_3_3[1])  ) <  3e-16 );

  assert( ( fabs(zimags(columnMeanmedTable1_3_3[2]) ) - (114.0 / 18.0 ) ) / fabs ( zimags(columnMeanmedTable1_3_3[2])  ) <  3e-16 );
  assert( ( fabs(zreals(columnMeanmedTable1_3_3[2]) ) - (146.0 / 18.0 ) ) / fabs ( zreals(columnMeanmedTable1_3_3[2])  ) <  3e-16 );




  return 0;
}



static int testMeanf(void) {

  dmeanfsTest();
  dmeanfaTest();
  dcolumnmeanfaTest();
  drowmeanfaTest();
  zmeanfsTest();
  zmeanfaTest();
  zrowmeanfaTest();
  zcolumnmeanfaTest();

  return 0;
}

int main(void) {
  assert(testMeanf() == 0);
  return 0;
}
