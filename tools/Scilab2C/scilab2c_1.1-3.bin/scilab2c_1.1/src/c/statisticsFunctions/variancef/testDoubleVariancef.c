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
#include "variancef.h"


/* #define LOCAL_DEBUG */

#define ERROR(x) printf("diff = %e\n", x)

static int dvariancefsTest(void) {


  printf("\n>>>> variancef Double Scalar Test\n");
  printf("result : %e " ,dvariancefs(value1,coef1)) ;
  
  assert( (  dvariancefs(3.0,56.0)  ) ==  0.0 );
  assert( ( dvariancefs( 1.123456789 ,2.0)  ) ==  0.0 );


  return 0;
}

static int dvariancefaTest(void) {
  double table1[3] = {3.0, 6.0, 9.0};
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
 
  printf("\n>>>> variancef Double Array Test\n");
  printf("result1 : %e " ,dvariancefa(table1, 3, coef1)) ;
  printf("result2 : %e " ,dvariancefa(table2, 5, coef2)) ;
  assert((dvariancefa(table1, 3, coef1) - 8.000000000 )/ dvariancefa(table1, 3 ,coef1) < 1e-6);
  assert((dvariancefa(table2, 5, coef2) - 1.916666666 )/ dvariancefa(table2, 5 ,coef2) < 1e-6);
  return 0;
}

/*
static int dcolumnvariancefaTest(void) {
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

  printf("\n>>>> Column Variancef Double Array Test\n");
 
    [ 1 2 3 ]  [10 11 12 ]
    [ 4 5 6 ].*[ 1  2  3 ]  => [ 68 32 146 ]
    [ 7 8 9 ]  [ 5  6  7 ]
  
  dcolumnvariancefa(table1, 3, 3, coef1 ,columnMeanmedTable1_3_3);
  assert( ( fabs(columnMeanmedTable1_3_3[0] ) - (  68.0 /  33.0 ) ) / fabs ( columnMeanmedTable1_3_3[0]  ) <  1e-6 );
  assert( ( fabs(columnMeanmedTable1_3_3[1] ) - (  32.0 /   6.0 ) ) / fabs ( columnMeanmedTable1_3_3[1]  ) <  1e-6 );
  assert( ( fabs(columnMeanmedTable1_3_3[2] ) - (  146.0 / 18.0 ) ) / fabs ( columnMeanmedTable1_3_3[2]  ) <  1e-6 );


    [ 1 2 3 4 5 6 7 8 9 ] => [ 45 ]

  dcolumnvariancefa(table1, 1, 9, coef1 ,columnMeanmedTable1_1_9);
  printf("result qui foire : %e\n " ,columnMeanmedTable1_1_9[0]) ;
  assert( ( fabs(columnMeanmedTable1_1_9[0] ) - (  1.146666666f ) ) / fabs ( columnMeanmedTable1_1_9[0]  ) <  1e-6 );

  
    [ 1 ]
    [ 2 ]
    [ 3 ]
    [ 4 ]
    [ 5 ] =>  [ 1 2 3 4 5 6 7 8 9 ]
    [ 6 ]
    [ 7 ]
    [ 8 ]
    [ 9 ]
  
  dcolumnvariancefa(table1, 9, 1, coef1, columnMeanmedTable1_9_1);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("columnMeanmedTable1_9_1[%d] = %e\n", i, columnMeanmedTable1_9_1[i]);
    assert(columnMeanmedTable1_9_1[i] == table1[i]);
  }
  
  
    [ 1 3 5 7  9 ] .* [ 11 13 15 17 19 ]
    [ 2 4 6 8 10 ]    [ 12 14 16 18 20 ]=> [ 415 520 ]
  
  dcolumnvariancefa(table2, 2, 5, coef2 , columnMeanmedTable2_2_5);
  assert( ( fabs(columnMeanmedTable2_2_5[0] ) - (  415.0 / 75.0 ) ) / fabs ( columnMeanmedTable2_2_5[0]  ) <  1e-6 );
  assert( ( fabs(columnMeanmedTable2_2_5[1] ) - (  520.0 / 80.0 ) ) / fabs ( columnMeanmedTable2_2_5[1]  ) <  1e-6 );
  for ( i = 0 ; i < 2 ; ++i) {
    printf("columnMeanmedTable2_2_5[%d] = %e\n", i, columnMeanmedTable2_2_5[i]);
  }

  
    [ 1  6 ]  [ 11  16 ]
    [ 2  7 ]  [ 12  17 ] => [ 107 143 183 227 275 ]
    [ 3  8 ].*[ 13  18 ]
    [ 4  9 ]  [ 14  19 ]
    [ 5 10 ]  [ 15  20 ]
   

  dcolumnvariancefa(table2, 5, 2, coef2 ,columnMeanmedTable2_5_2);
  assert( ( fabs(columnMeanmedTable2_5_2[0] ) - (  107.0 / 27.0 ) ) / fabs ( columnMeanmedTable2_5_2[0]  ) <  1e-6 );
  assert( ( fabs(columnMeanmedTable2_5_2[1] ) - (  143.0 / 29.0 ) ) / fabs ( columnMeanmedTable2_5_2[1]  ) <  1e-6 );
  assert( ( fabs(columnMeanmedTable2_5_2[2] ) - (  183.0 / 31.0 ) ) / fabs ( columnMeanmedTable2_5_2[2]  ) <  1e-6 );
  assert( ( fabs(columnMeanmedTable2_5_2[3] ) - (  227.0 / 33.0 ) ) / fabs ( columnMeanmedTable2_5_2[3]  ) <  1e-6 );
  assert( ( fabs(columnMeanmedTable2_5_2[4] ) - (  275.0 / 35.0 ) ) / fabs ( columnMeanmedTable2_5_2[4]  ) <  1e-6 );
  for ( i = 0 ; i < 5 ; ++i) {
    printf("columnMeanmedTable2_5_2[%d] = %e\n", i, columnMeanmedTable2_5_2[i]);
  }

  return 0;
}
*/
/*
static int srowvariancefaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double coef1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double table2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
  double coef2[10] =  {11.0, 12.0, 13.0, 14.0 , 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
  double rowMeanmedTable1_3_3[3] = {0};
  double rowMeanmedTable1_1_9[9] = {0};
  double rowMeanmedTable1_9_1[1] = {0};
  double rowMeanmedTable2_2_5[5] = {0};
  double rowMeanmedTable2_5_2[2] = {0};

  printf("\n>>>> Row Mean Double Array Test\n");
  
    [ 1 2 3 ]  [10 11 12 ]
    [ 4 5 6 ].*[ 1  2  3 ]  => [ 49 80 117 ]
    [ 7 8 9 ]  [ 5  6  7 ]
  
  srowvariancefa(table1, 3, 3, coef1 , rowMeanmedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowMeanmedTable1_3_3[%d] = %e\n", i, rowMeanmedTable1_3_3[i]);
  }
  assert( ( fabs(rowMeanmedTable1_3_3[0] ) - (  49.0 / 16.0 ) ) / fabs ( rowMeanmedTable1_3_3[0]  ) <  1e-6 );
  assert( ( fabs(rowMeanmedTable1_3_3[1] ) - (  80.0 / 19.0 ) ) / fabs ( rowMeanmedTable1_3_3[1]  ) <  1e-6 );
  assert( ( fabs(rowMeanmedTable1_3_3[2] ) - ( 117.0 / 22.0 ) ) / fabs ( rowMeanmedTable1_3_3[2]  ) <  1e-6 );

  
    [ 1 ]
    [ 2 ]
    [ 3 ]
    [ 4 ]
    [ 5 ] =>  [ 1 2 3 4 5 6 7 8 9 ]
    [ 6 ]
    [ 7 ]
    [ 8 ]
    [ 9 ]
  
  srowvariancefa(table1, 1, 9, coef1, rowMeanmedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowMeanmedTable1_1_9[%d] = %e\n", i, rowMeanmedTable1_1_9[i]);
    assert(rowMeanmedTable1_1_9[i] == table1[i]);
  }

  
    [ 1 2 3 4 5 6 7 8 9 ] => [ 246 ]
  
  srowvariancefa(table1, 9, 1, coef1, rowMeanmedTable1_9_1);
  assert( ( fabs(rowMeanmedTable1_9_1[0] ) - (  246.0 / 57.0 ) ) / fabs ( rowMeanmedTable1_9_1[0]  ) <  1e-6 );

  
    [ 1 3 5 7  9 ] .* [ 11 13 15 17 19 ]
    [ 2 4 6 8 10 ]    [ 12 14 16 18 20 ] => [ 3 7 11 15 19 ]
  
  srowvariancefa(table2, 2, 5, coef2, rowMeanmedTable2_2_5);
  for ( i = 0 ; i < 5 ; ++i) {
    printf("rowMeanmedTable2_2_5[%d] = %e\n", i, rowMeanmedTable2_2_5[i]);
  }
  assert( ( fabs(rowMeanmedTable2_2_5[0] ) - (  35.0 / 23.0 ) ) / fabs ( rowMeanmedTable2_2_5[0]  ) <  1e-6 );
  assert( ( fabs(rowMeanmedTable2_2_5[1] ) - (  95.0 / 27.0 ) ) / fabs ( rowMeanmedTable2_2_5[1]  ) <  1e-6 );
  assert( ( fabs(rowMeanmedTable2_2_5[2] ) - ( 171.0 / 31.0 ) ) / fabs ( rowMeanmedTable2_2_5[2]  ) <  1e-6 );
  assert( ( fabs(rowMeanmedTable2_2_5[3] ) - ( 263.0 / 35.0 ) ) / fabs ( rowMeanmedTable2_2_5[3]  ) <  1e-6 );
  assert( ( fabs(rowMeanmedTable2_2_5[4] ) - ( 371.0 / 39.0 ) ) / fabs ( rowMeanmedTable2_2_5[4]  ) <  1e-6 );


  return 0;
}
*/

static int zvariancefsTest(void) {

  printf("\n>>>> Mean Double Complex Scalar Test\n");
  assert(  zreals(zvariancefs(DoubleComplex(3.0, 0.0),DoubleComplex(3.0, 0.0))) ==  0.0 );
  assert(  zimags(zvariancefs(DoubleComplex(3.0, 0.0),DoubleComplex(3.0, 0.0))) ==  0.0 );
  assert(  zreals(zvariancefs(DoubleComplex(1.123456789, 1.123456789),DoubleComplex(9.0, 0.0))) ==  0.0 );
  assert(  zimags(zvariancefs(DoubleComplex(1.123456789, 1.123456789),DoubleComplex(9.0, 0.0))) ==  0.0 );

  return 0;
}



static int zvariancefaTest(void) {

  double tableR1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI1[9] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0};
  double coefR1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double coefI1[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 

  doubleComplex* table1 = DoubleComplexMatrix (tableR1, tableI1, 9);
  doubleComplex* coef1 = DoubleComplexMatrix (coefR1, coefI1, 9);
  doubleComplex result =DoubleComplex(0.0 , 0.0); 
  
  printf("\n>>>> Mean Double Complex Array Test\n");
  result = zvariancefa(table1, 9, coef1);
  /*printf("\nresult : %e \t+ %e i " ,zreals(result) ,zimags(result)) ;*/

  assert(  fabs(zreals(result) - (  1.3834586f ) ) / fabs ( zreals(result)  ) <  1e-6 );
  assert(  fabs(zimags(result) - (  9.6090226f ) ) / fabs ( zimags(result)  ) <  1e-6 );

  return 0;
}

/*
static int crowvariancefaTest(void) {

  int i = 0 ;

  double tableR1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI1[9] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0};
  double coefR1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double coefI1[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 

  double tableR2[10] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0, 10.0};
  double tableI2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 15.0};
  double coefR2[10]  = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0, 19.0}; 
  double coefI2[10]   = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 

  doubleComplex* table1 = DoubleComplexMatrix (tableR1, tableI1, 9);
  doubleComplex* coef1 = DoubleComplexMatrix (coefR1, coefI1, 9);

  doubleComplex* table2 = DoubleComplexMatrix (tableR2, tableI2, 9);
  doubleComplex* coef2 = DoubleComplexMatrix (coefR2, coefI2, 9);

  doubleComplex rowMeanmedTable1_3_3[3] = {DoubleComplex(0.0, 0.0)};
 
  doubleComplex rowMeanmedTable1_1_9[9] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable1_9_1[1] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable2_2_5[5] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable2_5_2[2] = {DoubleComplex(0.0, 0.0)};

  printf("\n>>>> Row variancef Double Complex Array Test\n");

 

  crowvariancefa(table1 , 3 , 3 , coef1 , rowMeanmedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {

      printf("rowMeanmedTable_3_3[%d] = %e + %ei\n", i, zreals(rowMeanmedTable1_3_3[i]), zimags(rowMeanmedTable1_3_3[i]));
    }
  assert( ( fabs(zimags(rowMeanmedTable1_3_3[0]) ) - ( 27.0 / 16.0 ) ) / fabs ( zimags(rowMeanmedTable1_3_3[0])  ) <  1e-6 );
  assert( ( fabs(zreals(rowMeanmedTable1_3_3[0]) ) - ( 49.0 / 16.0 ) ) / fabs ( zreals(rowMeanmedTable1_3_3[0])  ) <  1e-6 );
  
  assert( ( fabs(zimags(rowMeanmedTable1_3_3[1]) ) - ( 90.0 / 19.0 ) ) / fabs ( zimags(rowMeanmedTable1_3_3[1])  ) <  1e-6 );
  assert( ( fabs(zreals(rowMeanmedTable1_3_3[1]) ) - ( 80.0 / 19.0 ) ) / fabs ( zreals(rowMeanmedTable1_3_3[1])  ) <  1e-6 );

  assert( ( fabs(zimags(rowMeanmedTable1_3_3[2]) ) - (171.0 / 22.0 ) ) / fabs ( zimags(rowMeanmedTable1_3_3[2])  ) <  1e-6 );
  assert( ( fabs(zreals(rowMeanmedTable1_3_3[2]) ) - (117.0 / 22.0 ) ) / fabs ( zreals(rowMeanmedTable1_3_3[2])  ) <  1e-6 );




  return 0;
}
*/


/*
static int scolumnvariancefaTest(void) {

  int i = 0 ;

  double tableR1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI1[9] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0};
  double coefR1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double coefI1[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 

  double tableR2[10] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0, 10.0};
  double tableI2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 15.0};
  double coefR2[10]  = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0, 19.0}; 
  double coefI2[10]   = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 

  doubleComplex* table1 = DoubleComplexMatrix (tableR1, tableI1, 9);
  doubleComplex* coef1 = DoubleComplexMatrix (coefR1, coefI1, 9);

  doubleComplex* table2 = DoubleComplexMatrix (tableR2, tableI2, 9);
  doubleComplex* coef2 = DoubleComplexMatrix (coefR2, coefI2, 9);

  doubleComplex columnMeanmedTable1_3_3[3] = {DoubleComplex(0.0, 0.0)};
 
  doubleComplex rowMeanmedTable1_1_9[9] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable1_9_1[1] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable2_2_5[5] = {DoubleComplex(0.0, 0.0)};
  doubleComplex rowMeanmedTable2_5_2[2] = {DoubleComplex(0.0, 0.0)};


  printf("\n>>>> Column variancef Double Complex Array Test\n");

  

  scolumnvariancefa(table1 , 3 , 3 , coef1 , columnMeanmedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {
      printf("columnMeanmedTable_3_3[%d] = %e + %ei\n", i, zreals(columnMeanmedTable1_3_3[i]), zimags(columnMeanmedTable1_3_3[i]));
    }
  assert( ( fabs(zimags(columnMeanmedTable1_3_3[0]) ) - (138.0 / 33.0 ) ) / fabs ( zimags(columnMeanmedTable1_3_3[0])  ) <  1e-6 );
  assert( ( fabs(zreals(columnMeanmedTable1_3_3[0]) ) - ( 68.0 / 33.0 ) ) / fabs ( zreals(columnMeanmedTable1_3_3[0])  ) <  1e-6 );
  
  assert( ( fabs(zimags(columnMeanmedTable1_3_3[1]) ) - ( 36.0 /  6.0 ) ) / fabs ( zimags(columnMeanmedTable1_3_3[1])  ) <  1e-6 );
  assert( ( fabs(zreals(columnMeanmedTable1_3_3[1]) ) - ( 32.0 /  6.0 ) ) / fabs ( zreals(columnMeanmedTable1_3_3[1])  ) <  1e-6 );

  assert( ( fabs(zimags(columnMeanmedTable1_3_3[2]) ) - (114.0 / 18.0 ) ) / fabs ( zimags(columnMeanmedTable1_3_3[2])  ) <  1e-6 );
  assert( ( fabs(zreals(columnMeanmedTable1_3_3[2]) ) - (146.0 / 18.0 ) ) / fabs ( zreals(columnMeanmedTable1_3_3[2])  ) <  1e-6 );




  return 0;
}

*/

static int testvariancef(void) {

  dvariancefsTest();
  dvariancefaTest();
  /*dcolumnvariancefaTest();
  srowvariancefaTest();*/
  zvariancefsTest();
  zvariancefaTest();
  /*crowvariancefaTest();
  scolumnvariancefaTest();
*/
  return 0;
}

int main(void) {
  assert(testvariancef() == 0);
  return 0;
}

