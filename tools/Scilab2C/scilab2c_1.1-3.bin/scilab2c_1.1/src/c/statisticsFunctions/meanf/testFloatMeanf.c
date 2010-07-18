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

static int smeanfsTest(void) {
  float value1 ;
  float value2 ;
  float coef1  ;
  float coef2  ;

  value1 = 3.0f;
  value2 = 1.123456789f;
  coef1  = 56.0f ;
  coef2  = 2.0f ;

  printf("\n>>>> Meanf Float Scalar Test\n");
  assert( ( fabs(smeanfs(value1,coef1) ) - (  3.0f ) ) / fabs ( smeanfs(value1,coef1)  ) <  1e-6 );
  assert( ( fabs(smeanfs(value2,coef2) ) - (  1.123456789f ) ) / fabs ( smeanfs(value2,coef2)  ) <  1e-6 );

  return 0;
}

static int smeanfaTest(void) {
  float table1[3] = {3.0f, 3.0f, 3.0f};
  float coef1[3] = {10.0f, 2.0f, 6.0f}; 

  float table2[5] = {3.186784563f,
		     4.186784563f,
		     5.186784563f,
		     6.186784563f,
		     7.186784563f};

  float coef2[5] = {3.0f,
		     4.0f,
		     5.0f,
		     6.0f,
		     7.0f};

  printf("\n>>>> Meanf Float Array Test\n");
  assert(smeanfa(table1, 3, coef1) == 54.0f / 18.0f);
  assert((smeanfa(table2, 5, coef2) - (3.186784563f*3.0f + 4.186784563f*4.0f + 5.186784563f*5.0f + 6.186784563f*6.0f  + 7.186784563f*7.0f) / 25.0f) / smeanfa(table2, 5 ,coef2) < 1e-6);
  return 0;
}


static int scolumnmeanfaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float coef1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 
  float table2[10] = {1.0f,   2.0f,  3.0f,  4.0f ,  5.0f,  6.0f,  7.0f,  8.0f,  9.0f, 10.0f};
  float coef2[10] =  {11.0f, 12.0f, 13.0f, 14.0f , 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f};
  float columnMeanmedTable1_3_3[3] = {0};
  float columnMeanmedTable1_1_9[1] = {0};
  float columnMeanmedTable1_9_1[9] = {0};
  float columnMeanmedTable2_2_5[2] = {0};
  float columnMeanmedTable2_5_2[5] = {0};

  printf("\n>>>> Column Mean Float Array Test\n");
  /*
    [ 1 2 3 ]  [10 11 12 ]
    [ 4 5 6 ].*[ 1  2  3 ]  => [ 68 32 146 ]
    [ 7 8 9 ]  [ 5  6  7 ]
  */
  scolumnmeanfa(table1, 3, 3, coef1 ,columnMeanmedTable1_3_3);
  assert( fabs(columnMeanmedTable1_3_3[0]  - (  68.0f /  33.0f ) ) / fabs ( columnMeanmedTable1_3_3[0]  ) <  1e-6 );
  assert( fabs(columnMeanmedTable1_3_3[1]  - (  32.0f /   6.0f ) ) / fabs ( columnMeanmedTable1_3_3[1]  ) <  1e-6 );
  assert( fabs(columnMeanmedTable1_3_3[2]  - (  146.0f / 18.0f ) ) / fabs ( columnMeanmedTable1_3_3[2]  ) <  1e-6 );

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]
  */
  scolumnmeanfa(table1, 1, 9, coef1 ,columnMeanmedTable1_1_9);
  assert( ( fabs(columnMeanmedTable1_1_9[0] ) - (  246.0f / 57.0f ) ) / fabs ( columnMeanmedTable1_1_9[0]  ) <  1e-6 );

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
  scolumnmeanfa(table1, 9, 1, coef1, columnMeanmedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnMeanmedTable1_9_1[%d] = %e\n", i, columnMeanmedTable1_9_1[i]);
    assert(columnMeanmedTable1_9_1[i] == table1[i]);
  }

  /*
    [ 1 3 5 7  9 ] .* [ 11 13 15 17 19 ]
    [ 2 4 6 8 10 ]    [ 12 14 16 18 20 ]=> [ 415 520 ]
  */
  scolumnmeanfa(table2, 2, 5, coef2 , columnMeanmedTable2_2_5);
  assert( ( fabs(columnMeanmedTable2_2_5[0] ) - (  415.0f / 75.0f ) ) / fabs ( columnMeanmedTable2_2_5[0]  ) <  1e-6 );
  assert( ( fabs(columnMeanmedTable2_2_5[1] ) - (  520.0f / 80.0f ) ) / fabs ( columnMeanmedTable2_2_5[1]  ) <  1e-6 );
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
  scolumnmeanfa(table2, 5, 2, coef2 ,columnMeanmedTable2_5_2);
  assert( fabs(columnMeanmedTable2_5_2[0]  - (  107.0f / 27.0f ) ) / fabs ( columnMeanmedTable2_5_2[0]  ) <  1e-6 );
  assert( fabs(columnMeanmedTable2_5_2[1]  - (  143.0f / 29.0f ) ) / fabs ( columnMeanmedTable2_5_2[1]  ) <  1e-6 );
  assert( fabs(columnMeanmedTable2_5_2[2]  - (  183.0f / 31.0f ) ) / fabs ( columnMeanmedTable2_5_2[2]  ) <  1e-6 );
  assert( fabs(columnMeanmedTable2_5_2[3]  - (  227.0f / 33.0f ) ) / fabs ( columnMeanmedTable2_5_2[3]  ) <  1e-6 );
  assert( fabs(columnMeanmedTable2_5_2[4]  - (  275.0f / 35.0f ) ) / fabs ( columnMeanmedTable2_5_2[4]  ) <  1e-6 );
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnMeanmedTable2_5_2[%d] = %e\n", i, columnMeanmedTable2_5_2[i]);
  }

  return 0;
}


static int srowmeanfaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float coef1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 
  float table2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  float coef2[10] =  {11.0f, 12.0f, 13.0f, 14.0f , 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f};
  float rowMeanmedTable1_3_3[3] = {0};
  float rowMeanmedTable1_1_9[9] = {0};
  float rowMeanmedTable1_9_1[1] = {0};
  float rowMeanmedTable2_2_5[5] = {0};
  /*float rowMeanmedTable2_5_2[2] = {0};*/

  printf("\n>>>> Row Mean Float Array Test\n");
  /*
    [ 1 2 3 ]  [10 11 12 ]
    [ 4 5 6 ].*[ 1  2  3 ]  => [ 49 80 117 ]
    [ 7 8 9 ]  [ 5  6  7 ]
  */
  srowmeanfa(table1, 3, 3, coef1 , rowMeanmedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowMeanmedTable1_3_3[%d] = %e\n", i, rowMeanmedTable1_3_3[i]);
  }
  assert( fabs(rowMeanmedTable1_3_3[0]  - (  49.0f / 16.0f ) ) / fabs ( rowMeanmedTable1_3_3[0]  ) <  1e-6 );
  assert( fabs(rowMeanmedTable1_3_3[1]  - (  80.0f / 19.0f ) ) / fabs ( rowMeanmedTable1_3_3[1]  ) <  1e-6 );
  assert( fabs(rowMeanmedTable1_3_3[2]  - ( 117.0f / 22.0f ) ) / fabs ( rowMeanmedTable1_3_3[2]  ) <  1e-6 );

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
  srowmeanfa(table1, 1, 9, coef1, rowMeanmedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowMeanmedTable1_1_9[%d] = %e\n", i, rowMeanmedTable1_1_9[i]);
    assert(rowMeanmedTable1_1_9[i] == table1[i]);
  }

  /*
    [ 1 2 3 4 5 6 7 8 9 ] => [ 246 ]
  */
  srowmeanfa(table1, 9, 1, coef1, rowMeanmedTable1_9_1);
  assert( ( fabs(rowMeanmedTable1_9_1[0] ) - (  246.0f / 57.0f ) ) / fabs ( rowMeanmedTable1_9_1[0]  ) <  1e-6 );

  /*
    [ 1 3 5 7  9 ] .* [ 11 13 15 17 19 ]
    [ 2 4 6 8 10 ]    [ 12 14 16 18 20 ] => [ 3 7 11 15 19 ]
  */
  srowmeanfa(table2, 2, 5, coef2, rowMeanmedTable2_2_5);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("rowMeanmedTable2_2_5[%d] = %e\n", i, rowMeanmedTable2_2_5[i]);
  }
  assert( fabs(rowMeanmedTable2_2_5[0]  - (  35.0f / 23.0f ) ) / fabs ( rowMeanmedTable2_2_5[0]  ) <  1e-6 );
  assert( fabs(rowMeanmedTable2_2_5[1]  - (  95.0f / 27.0f ) ) / fabs ( rowMeanmedTable2_2_5[1]  ) <  1e-6 );
  assert( fabs(rowMeanmedTable2_2_5[2]  - ( 171.0f / 31.0f ) ) / fabs ( rowMeanmedTable2_2_5[2]  ) <  1e-6 );
  assert( fabs(rowMeanmedTable2_2_5[3]  - ( 263.0f / 35.0f ) ) / fabs ( rowMeanmedTable2_2_5[3]  ) <  1e-6 );
  assert( fabs(rowMeanmedTable2_2_5[4]  - ( 371.0f / 39.0f ) ) / fabs ( rowMeanmedTable2_2_5[4]  ) <  1e-6 );


  return 0;
}

static int cmeanfsTest(void) {
  floatComplex value1 ;
  floatComplex coef1 ;
  floatComplex value2 ;
  floatComplex coef2 ;

  value1 = FloatComplex(3.0f, 3.0f);
  coef1  = FloatComplex(3.0f, 0.0f);
  value2 = FloatComplex(1.123456789f, 1.123456789f);
  coef2  = FloatComplex(9.0f, 0.0f);

  printf("\n>>>> Mean Float Complex Scalar Test\n");
  assert(  fabs(creals(cmeanfs(value1,coef1))  - (  3.0f ) ) / fabs ( creals(cmeanfs(value1,coef1))  ) <  1e-6 );
  assert(  fabs(cimags(cmeanfs(value1,coef1))  - (  3.0f ) ) / fabs ( cimags(cmeanfs(value1,coef1))  ) <  1e-6 );
  assert(  fabs(creals(cmeanfs(value2,coef1))  - (  1.123456789f ) ) / fabs ( creals(cmeanfs(value2,coef1))  ) <  1e-6 );
  assert(  fabs(cimags(cmeanfs(value2,coef1))  - (  1.123456789f ) ) / fabs ( cimags(cmeanfs(value2,coef1))  ) <  1e-6 );
  return 0;
}



static int cmeanfaTest(void) {

  float tableR1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float tableI1[9] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  float coefR1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 
  float coefI1[9] = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 

  floatComplex* table1 = FloatComplexMatrix (tableR1, tableI1, 9);
  floatComplex* coef1 = FloatComplexMatrix (coefR1, coefI1, 9);


  printf("\n>>>> Mean Float Complex Array Test\n");
  printf("\tMeanmedTable_1_1[0] = %e + %ei\n", creals(cmeanfa(table1, 9, coef1)), cimags(cmeanfa(table1, 9, coef1)));

  assert( fabs( creals(cmeanfa(table1, 9, coef1))  - (  246.0f /  57.0f ) ) / fabs ( creals(cmeanfa(table1, 9, coef1))  ) <  1e-6 );
  assert( fabs( cimags(cmeanfa(table1, 9, coef1))  - (  288.0f /  57.0f ) ) / fabs ( cimags(cmeanfa(table1, 9, coef1))  ) <  1e-6 );/*
*/
  return 0;
}

static int crowmeanfaTest(void) {

  int i = 0 ;

  float tableR1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float tableI1[9] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  float coefR1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 
  float coefI1[9] = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 
/*
  float tableR2[10] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f, 10.0f};
  float tableI2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 15.0f};
  float coefR2[10]  = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f, 19.0f}; 
  float coefI2[10]   = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 
*/
  floatComplex* table1 = FloatComplexMatrix (tableR1, tableI1, 9);
  floatComplex* coef1 = FloatComplexMatrix (coefR1, coefI1, 9);
/*
  floatComplex* table2 = FloatComplexMatrix (tableR2, tableI2, 9);
  floatComplex* coef2 = FloatComplexMatrix (coefR2, coefI2, 9);
*/
  floatComplex rowMeanmedTable1_3_3[3] ;
  rowMeanmedTable1_3_3[0] = FloatComplex(0.0f, 0.0f);
  rowMeanmedTable1_3_3[1] = FloatComplex(0.0f, 0.0f);
  rowMeanmedTable1_3_3[2] = FloatComplex(0.0f, 0.0f);
 /*
  floatComplex rowMeanmedTable1_1_9[9] = {FloatComplex(0.0f, 0.0f)};
  floatComplex rowMeanmedTable1_9_1[1] = {FloatComplex(0.0f, 0.0f)};
  floatComplex rowMeanmedTable2_2_5[5] = {FloatComplex(0.0f, 0.0f)};
  floatComplex rowMeanmedTable2_5_2[2] = {FloatComplex(0.0f, 0.0f)};
*/

  printf("\n>>>> Row Meanf Float Complex Array Test\n");

  /**/

  crowmeanfa(table1 , 3 , 3 , coef1 , rowMeanmedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {

      printf("rowMeanmedTable_3_3[%d] = %e + %ei\n", i, creals(rowMeanmedTable1_3_3[i]), cimags(rowMeanmedTable1_3_3[i]));
    }
  assert( fabs(cimags(rowMeanmedTable1_3_3[0])  - ( 27.0f / 16.0f ) ) / fabs ( cimags(rowMeanmedTable1_3_3[0])  ) <  1e-6 );
  assert( fabs(creals(rowMeanmedTable1_3_3[0])  - ( 49.0f / 16.0f ) ) / fabs ( creals(rowMeanmedTable1_3_3[0])  ) <  1e-6 );
  
  assert( fabs(cimags(rowMeanmedTable1_3_3[1])  - ( 90.0f / 19.0f ) ) / fabs ( cimags(rowMeanmedTable1_3_3[1])  ) <  1e-6 );
  assert( fabs(creals(rowMeanmedTable1_3_3[1])  - ( 80.0f / 19.0f ) ) / fabs ( creals(rowMeanmedTable1_3_3[1])  ) <  1e-6 );

  assert( fabs(cimags(rowMeanmedTable1_3_3[2])  - (171.0f / 22.0f ) ) / fabs ( cimags(rowMeanmedTable1_3_3[2])  ) <  1e-6 );
  assert( fabs(creals(rowMeanmedTable1_3_3[2])  - (117.0f / 22.0f ) ) / fabs ( creals(rowMeanmedTable1_3_3[2])  ) <  1e-6 );




  return 0;
}


static int ccolumnmeanfaTest(void) {

  int i = 0 ;

  float tableR1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float tableI1[9] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  float coefR1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 
  float coefI1[9] = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 
/*
  float tableR2[10] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f, 10.0f};
  float tableI2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 15.0f};
  float coefR2[10]  = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f, 19.0f}; 
  float coefI2[10]   = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 
*/
  floatComplex* table1 = FloatComplexMatrix (tableR1, tableI1, 9);
  floatComplex* coef1 = FloatComplexMatrix (coefR1, coefI1, 9);
/*
  floatComplex* table2 = FloatComplexMatrix (tableR2, tableI2, 9);
  floatComplex* coef2 = FloatComplexMatrix (coefR2, coefI2, 9);
*/
  floatComplex columnMeanmedTable1_3_3[3] ;
  columnMeanmedTable1_3_3[0] = FloatComplex(0.0f, 0.0f);
  columnMeanmedTable1_3_3[1] = FloatComplex(0.0f, 0.0f);
  columnMeanmedTable1_3_3[2] = FloatComplex(0.0f, 0.0f);
 /*
  floatComplex rowMeanmedTable1_1_9[9] = {FloatComplex(0.0f, 0.0f)};
  floatComplex rowMeanmedTable1_9_1[1] = {FloatComplex(0.0f, 0.0f)};
  floatComplex rowMeanmedTable2_2_5[5] = {FloatComplex(0.0f, 0.0f)};
  floatComplex rowMeanmedTable2_5_2[2] = {FloatComplex(0.0f, 0.0f)};
*/

  printf("\n>>>> Column Meanf Float Complex Array Test\n");

  /**/

  ccolumnmeanfa(table1 , 3 , 3 , coef1 , columnMeanmedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {
      printf("columnMeanmedTable_3_3[%d] = %e + %ei\n", i, creals(columnMeanmedTable1_3_3[i]), cimags(columnMeanmedTable1_3_3[i]));
    }
  assert( fabs(cimags(columnMeanmedTable1_3_3[0])  - (138.0f / 33.0f ) ) / fabs ( cimags(columnMeanmedTable1_3_3[0])  ) <  1e-6 );
  assert( fabs(creals(columnMeanmedTable1_3_3[0])  - ( 68.0f / 33.0f ) ) / fabs ( creals(columnMeanmedTable1_3_3[0])  ) <  1e-6 );
  
  assert( fabs(cimags(columnMeanmedTable1_3_3[1])  - ( 36.0f /  6.0f ) ) / fabs ( cimags(columnMeanmedTable1_3_3[1])  ) <  1e-6 );
  assert( fabs(creals(columnMeanmedTable1_3_3[1])  - ( 32.0f /  6.0f ) ) / fabs ( creals(columnMeanmedTable1_3_3[1])  ) <  1e-6 );

  assert( fabs(cimags(columnMeanmedTable1_3_3[2])  - (114.0f / 18.0f ) ) / fabs ( cimags(columnMeanmedTable1_3_3[2])  ) <  1e-6 );
  assert( fabs(creals(columnMeanmedTable1_3_3[2])  - (146.0f / 18.0f ) ) / fabs ( creals(columnMeanmedTable1_3_3[2])  ) <  1e-6 );




  return 0;
}



static int testMeanf(void) {

  smeanfsTest();
  smeanfaTest();
  scolumnmeanfaTest();
  srowmeanfaTest();
  cmeanfsTest();
  cmeanfaTest();
  crowmeanfaTest();
  ccolumnmeanfaTest();

  return 0;
}

int main(void) {
  assert(testMeanf() == 0);
  return 0;
}

