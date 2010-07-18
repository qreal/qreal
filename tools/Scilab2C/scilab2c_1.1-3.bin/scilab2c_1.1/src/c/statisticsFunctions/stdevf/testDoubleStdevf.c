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
#include "stdevf.h"


/* #define LOCAL_DEBUG */

#define ERROR(x) printf("diff = %e\n", x)

static int dstdevfsTest(void) {
  double value1 = 3.0;
  double value2 = 1.123456789f;
  double coef1  = 56.0 ;
  double coef2  = 2.0 ;

  printf("\n>>>> stdevf Double Scalar Test\n");
  printf("result : %e " ,dstdevfs(value1,coef1)) ;
  printf("result : %e " ,dstdevfs(value2,coef)) ;
  assert( (  dstdevfs(value1,coef1)  ) ==  0.0 );
  assert( ( dstdevfs(value2,coef2)  ) ==  0.0 );

  value1 = 3.0;
  value2 = 1.123456789f;
  coef1  = 56.0 ;
  coef2  = 2.0 ;


  return 0;
}

static int dstdevfaTest(void) {
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
 
  double table3[10] = {3.0, 6.0, 9.0,10.0, 5.0,
                      6.0,18.0, 7.0,14.0, 2.0};
  double coef3 [10] = {3.0, 8.0,14.0,13.0, 2.0,
                      5.0, 8.0, 2.0, 6.0, 8.0};

  double result_2_5 = dstdevfa(table3,2, 5, coef3);

  printf("\n>>>> stdevf Double Array Test\n");
  printf("\nresult : %e " ,dstdevfa(table1,1, 3, coef1)) ;
  printf("\nresult : %e " ,dstdevfa(table2,1, 5, coef2)) ;
  printf("\nresult : %e " ,result_2_5) ;

  assert(dstdevfa(table1,1, 3, coef1) == 0.0);
  assert(dstdevfa(table1,1, 3, coef1) == 0.0);
  assert( ( fabs(result_2_5 ) - (  4.6440201 ) ) / fabs ( result_2_5  ) <  1e-6 );
  return 0;
}


static int dcolumnstdevfaTest(void) {

  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double coef1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 

  double columnStdevfedTable1_3_3[3] = {0};
  double columnStdevfedTable1_1_9[1] = {0};


  printf("\n>>>> Column stdevf Double Array Test\n");
  
/**/
  
  dcolumnstdevfa(table1, 3, 3, coef1 ,columnStdevfedTable1_3_3);

  printf("\nresult1 : %e " ,columnStdevfedTable1_3_3[0]) ;
  printf("\nresult2 : %e " ,columnStdevfedTable1_3_3[1]) ;
  printf("\nresult3 : %e " ,columnStdevfedTable1_3_3[2]) ;

  assert( fabs(columnStdevfedTable1_3_3[0]  - 0.8268689) / fabs ( columnStdevfedTable1_3_3[0]  ) <  1e-6 );
  assert( fabs(columnStdevfedTable1_3_3[1]  - 0.8164966) / fabs ( columnStdevfedTable1_3_3[1]  ) <  1e-6 );
  assert( fabs(columnStdevfedTable1_3_3[2]  - 0.8323524 ) / fabs ( columnStdevfedTable1_3_3[2]  ) <  1e-6 );


  
  dcolumnstdevfa(table1, 1, 9, coef1 ,columnStdevfedTable1_1_9);
  printf("\nresult_1_9: %e\n " ,columnStdevfedTable1_1_9[0]) ;
  assert( columnStdevfedTable1_1_9[0] == 0.0);


  return 0;
}


static int drowstdevfaTest(void) {
  int i = 0;
  double table1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double coef1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 

  double table2[10] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0, 10.0};
  double coef2[10]  = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0, 19.0}; 



  double rowStdevfedTable1_3_3[3] = {0};
  double rowStdevfedTable1_1_9[9] = {0};
  double rowStdevfedTable2_2_5[5] = {0};
  printf("\n>>>> Row Mean Double Array Test\n");

  
  drowstdevfa(table1, 3, 3, coef1 , rowStdevfedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowStdevfedTable1_3_3[%d] = %e\n", i, rowStdevfedTable1_3_3[i]);
  }
  assert( fabs(rowStdevfedTable1_3_3[0]  - 2.8394542 ) / fabs ( rowStdevfedTable1_3_3[0]  ) <  1e-6 );
  assert( fabs(rowStdevfedTable1_3_3[1]  - 2.8003759) / fabs ( rowStdevfedTable1_3_3[1]  ) <  1e-6 );
  assert( fabs(rowStdevfedTable1_3_3[2]  - 2.7669196) / fabs ( rowStdevfedTable1_3_3[2]  ) <  1e-6 );

/**/
  drowstdevfa(table1, 1, 9, coef1, rowStdevfedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowStdevfedTable1_1_9[%d] = %e\n", i, rowStdevfedTable1_1_9[i]);
    assert(rowStdevfedTable1_1_9[i] == 0.0);
  }

  
/**/
  /**/
  drowstdevfa(table2 , 2 , 5 , coef2 , rowStdevfedTable2_2_5);
  for (i = 0 ; i < 5 ; ++i )
    {
      printf("rowStdevfedTable_2_5[%d] = %e \n", i, rowStdevfedTable2_2_5[i]);
    }
  assert( fabs(rowStdevfedTable2_2_5[0] - 0.9045340 ) / fabs ( rowStdevfedTable2_2_5[0] ) <  1e-6 );
  assert( fabs(rowStdevfedTable2_2_5[1] - 2.3935678 ) / fabs ( rowStdevfedTable2_2_5[1] ) <  1e-6 );
  assert( fabs(rowStdevfedTable2_2_5[2] - 1.3887301 ) / fabs ( rowStdevfedTable2_2_5[2] ) <  1e-6 );
  assert( fabs(rowStdevfedTable2_2_5[3] - 1.2421180 ) / fabs ( rowStdevfedTable2_2_5[3] ) <  1e-6 );
  assert( fabs(rowStdevfedTable2_2_5[4] - 0.4523443 ) / fabs ( rowStdevfedTable2_2_5[4] ) <  1e-6 );
  return 0;
}


static int zstdevfsTest(void) {
  doubleComplex value1;
  doubleComplex coef1 ;
  doubleComplex value2;
  doubleComplex coef2 ;

  value1 = DoubleComplex(3.0, 3.0);
  coef1 = DoubleComplex(3.0, 0.0);
  value2 = DoubleComplex(1.123456789f, 1.123456789f);
  coef2 = DoubleComplex(9.0, 0.0);

  printf("\n>>>> Mean Double Complex Scalar Test\n");
  assert(  zreals(zstdevfs(value1,coef1)) ==  0.0 );
  assert(  zimags(zstdevfs(value1,coef1)) ==  0.0 );
  assert(  zreals(zstdevfs(value2,coef2)) ==  0.0 );
  assert(  zimags(zstdevfs(value2,coef2)) ==  0.0 );

  return 0;
}



static int zstdevfaTest(void) {

  double tableR1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI1[9] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0};
  double coefR1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double coefI1[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 


  double tableR2[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI2[9] = {0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0};
  double coefR2[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 
  double coefI2[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 


  double tableR3[10] = {3.0, 6.0, 9.0,10.0, 5.0,
                       6.0,18.0, 7.0,14.0, 2.0};
  double tableI3[10] = {3.0,12.0,25.0, 1.0, 2.0,
                       5.0,18.0, 7.0, 4.0, 1.0};


  double coefR3 [10] = {3.0, 8.0,14.0,13.0, 2.0,
                       5.0, 8.0, 2.0, 6.0, 8.0};
  double coefI3 [10] = {0.0, 0.0, 0.0, 0.0, 0.0,
                       0.0, 0.0, 0.0, 0.0, 0.0}; 


  doubleComplex* table1 = DoubleComplexMatrix (tableR1, tableI1, 9);
  doubleComplex* coef1 = DoubleComplexMatrix (coefR1, coefI1, 9);

  doubleComplex* table2 = DoubleComplexMatrix (tableR2, tableI2, 9);
  doubleComplex* coef2 = DoubleComplexMatrix (coefR2, coefI2, 9);

  doubleComplex* table3 = DoubleComplexMatrix (tableR3, tableI3, 10);
  doubleComplex* coef3 = DoubleComplexMatrix (coefR3, coefI3, 10);

  doubleComplex result =DoubleComplex(0.0 , 0.0); 
  
  printf("\n>>>> Mean Double Complex Array Test\n");
  result = zstdevfa(table1,1, 9, coef1);
  printf("\nresult_1_9 : %e \t+ %e i " ,zreals(result) ,zimags(result)) ;
  assert(  zreals(result) == 0.0 );
  assert(  zimags(result) == 0.0 );

  result = zstdevfa(table2,1, 9, coef2);
  printf("\nresult2_1_9 : %e \t+ %e i " ,zreals(result) ,zimags(result)) ;
  assert(  zreals(result) == 0.0 );
  assert(  zimags(result) == 0.0 );


  result = zstdevfa(table3,2, 5, coef3);
  printf("\nresult_2_5 : %e \t+ %e i " ,zreals(result) ,zimags(result)) ;
  assert( fabs(zreals(result)  - 1.7749350 ) / fabs ( zimags(result)  ) <  1e-6 );
  assert( fabs(zimags(result)  - 8.3811287) / fabs ( zreals(result)  ) <  1e-6 );

  return 0;
}


static int zrowstdevfaTest(void) {

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

  doubleComplex* table2 = DoubleComplexMatrix (tableR2, tableI2, 10);
  doubleComplex* coef2 = DoubleComplexMatrix (coefR2, coefI2, 10);
 
  doubleComplex rowStdevfedTable1_3_3[3];
  doubleComplex rowStdevfedTable2_2_5[5];
  /*test when the matrix is only imaginary*/
  double tableR3[10] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
  double tableI3[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 15.0};
  double coefR3[10]  = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0, 19.0}; 
  double coefI3[10]  = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 


  doubleComplex* table3 = DoubleComplexMatrix (tableR3, tableI3, 10);
  doubleComplex* coef3  = DoubleComplexMatrix (coefR3, coefI3, 10);
 
  doubleComplex rowStdevfedTable3_2_5[5];
 /**/
  /*test when the matrix is only real and the coef matrix is only imaginary*/
  double tableR4[10] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0, 10.0};
  double tableI4[10] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0};
  double coefR4[10]  = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0}; 
  double coefI4[10]   ={10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0, 19.0};

 


  doubleComplex* table4 = DoubleComplexMatrix (tableR4, tableI4, 10);
  doubleComplex* coef4  = DoubleComplexMatrix (coefR4, coefI4, 10);
 
  doubleComplex rowStdevfedTable4_2_5[5];
 /**/


  printf("\n>>>> Row stdevf Double Complex Array Test\n");

  rowStdevfedTable1_3_3[0] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable1_3_3[1] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable1_3_3[2] = DoubleComplex(0.0, 0.0);

  rowStdevfedTable2_2_5[0] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable2_2_5[1] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable2_2_5[2] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable2_2_5[3] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable2_2_5[4] = DoubleComplex(0.0, 0.0);

  rowStdevfedTable3_2_5[0] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable3_2_5[1] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable3_2_5[2] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable3_2_5[3] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable3_2_5[4] = DoubleComplex(0.0, 0.0);


  rowStdevfedTable4_2_5[0] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable4_2_5[1] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable4_2_5[2] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable4_2_5[3] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable4_2_5[4] = DoubleComplex(0.0, 0.0);


  zrowstdevfa(table1 , 3 , 3 , coef1 , rowStdevfedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {
      printf("rowStdevfedTable_3_3[%d] = %e + %ei\n", i, zreals(rowStdevfedTable1_3_3[i]), zimags(rowStdevfedTable1_3_3[i]));
    }
  assert( fabs(zreals(rowStdevfedTable1_3_3[0])  - 2.8394542 ) / fabs ( zreals(rowStdevfedTable1_3_3[0])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable1_3_3[0])  - 0.9464847) / fabs ( zimags(rowStdevfedTable1_3_3[0])  ) <  1e-6 );
  
  assert( fabs(zreals(rowStdevfedTable1_3_3[1])  - 2.8003759) / fabs ( zreals(rowStdevfedTable1_3_3[1])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable1_3_3[1])  - 0.9334586) / fabs ( zimags(rowStdevfedTable1_3_3[1])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable1_3_3[2])  - 2.7669196) / fabs ( zreals(rowStdevfedTable1_3_3[2])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable1_3_3[2])  - 0.9223065) / fabs ( zimags(rowStdevfedTable1_3_3[2])  ) <  1e-6 );
/**/
  zrowstdevfa(table2 , 2 , 5 , coef2 , rowStdevfedTable2_2_5);
  for (i = 0 ; i < 5 ; ++i )
    {
      printf("rowStdevfedTable_2_5[%d] = %e + %ei\n", i, zreals(rowStdevfedTable2_2_5[i]), zimags(rowStdevfedTable2_2_5[i]));
    }
  assert( fabs(zreals(rowStdevfedTable2_2_5[0])  - 0.9045340 ) / fabs ( zreals(rowStdevfedTable2_2_5[0])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable2_2_5[0])  - 0.3015113 ) / fabs ( zimags(rowStdevfedTable2_2_5[0])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable2_2_5[1])  - 2.3935678 ) / fabs ( zreals(rowStdevfedTable2_2_5[1])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable2_2_5[1])  + 0.4787136) / fabs ( zimags(rowStdevfedTable2_2_5[1])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable2_2_5[2])  - 1.3887301 ) / fabs ( zreals(rowStdevfedTable2_2_5[2])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable2_2_5[2])  - 0.4629100 ) / fabs ( zimags(rowStdevfedTable2_2_5[2])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable2_2_5[3])  - 1.2421180 ) / fabs ( zreals(rowStdevfedTable2_2_5[3])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable2_2_5[3])  - 0.4140393 ) / fabs ( zimags(rowStdevfedTable2_2_5[3])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable2_2_5[4])  - 0.4523443 ) / fabs ( zreals(rowStdevfedTable2_2_5[4])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable2_2_5[4])  - 2.7140659) / fabs ( zimags(rowStdevfedTable2_2_5[4])  ) <  1e-6 );
/**/
  zrowstdevfa(table3, 2 , 5 , coef3 , rowStdevfedTable3_2_5);
  for (i = 0 ; i < 5 ; ++i )
    {
      printf("rowStdevfedTable3_2_5[%d] = %e + %ei\n", i, zreals(rowStdevfedTable3_2_5[i]), zimags(rowStdevfedTable3_2_5[i]));
    }
  
  assert( fabs(zimags(rowStdevfedTable3_2_5[0])  - 0.3015113 ) / fabs (zimags(rowStdevfedTable3_2_5[0])  ) <  1e-6 );


  assert( fabs(zimags(rowStdevfedTable3_2_5[1])  - 0.4787136) / fabs  ( zimags(rowStdevfedTable3_2_5[1])  ) <  1e-6 );


  assert( fabs(zimags(rowStdevfedTable3_2_5[2])  - 0.4629100 ) / fabs (zimags(rowStdevfedTable3_2_5[2])  ) <  1e-6 );


  assert( fabs(zimags(rowStdevfedTable3_2_5[3])  - 0.4140393 ) / fabs ( zimags(rowStdevfedTable3_2_5[3])  ) <  1e-6 );


  assert( fabs(zimags(rowStdevfedTable3_2_5[4])  - 2.7140659) / fabs  ( zimags(rowStdevfedTable3_2_5[4])  ) <  1e-6 );

/**/
  zrowstdevfa(table4 , 2 , 5 , coef4 , rowStdevfedTable4_2_5);
  for (i = 0 ; i < 5 ; ++i )
    {
      printf("rowStdevfedTable4_2_5[%d] = %e + %ei\n", i, zreals(rowStdevfedTable4_2_5[i]), zimags(rowStdevfedTable4_2_5[i]));
    }
  assert( fabs(zreals(rowStdevfedTable4_2_5[0])  - 0.8597825 ) / fabs ( zreals(rowStdevfedTable4_2_5[0])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable4_2_5[0])  + 0.0390006 ) / fabs ( zimags(rowStdevfedTable4_2_5[0])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable4_2_5[1])  - 2.3141768 ) / fabs ( zreals(rowStdevfedTable4_2_5[1])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable4_2_5[1])  + 0.0722475) / fabs ( zimags(rowStdevfedTable4_2_5[1])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable4_2_5[2])  - 1.2915122 ) / fabs ( zreals(rowStdevfedTable4_2_5[2])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable4_2_5[2])  + 0.0804066 ) / fabs ( zimags(rowStdevfedTable4_2_5[2])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable4_2_5[3])  - 1.1980065 ) / fabs ( zreals(rowStdevfedTable4_2_5[3])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable4_2_5[3])  + 0.0398893 ) / fabs ( zimags(rowStdevfedTable4_2_5[3])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable4_2_5[4])  - 0.4433143 ) / fabs ( zreals(rowStdevfedTable4_2_5[4])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable4_2_5[4])  + 0.00852212) / fabs ( zimags(rowStdevfedTable4_2_5[4])  ) <  1e-6 );

  return 0;
}




static int zcolumnstdevfaTest(void) {

  int i = 0 ;

  double tableR1[9] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0};
  double tableI1[9] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0};
  double coefR1[9] = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0}; 
  double coefI1[9] = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0}; 

  double tableR2[10] = {1.0, 4.0, 7.0, 2.0 , 5.0, 8.0, 3.0, 6.0, 9.0, 10.0};
  double tableI2[10] = {1.0, 2.0, 3.0, 4.0 , 5.0, 6.0, 7.0, 8.0, 9.0, 15.0};
  double coefR2[10]  = {10.0, 1.0, 5.0,11.0 , 2.0, 6.0,12.0, 3.0, 7.0, 19.0}; 
  double coefI2[10]   = { 0.0, 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 

  double tableR3[6] = {4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
  double tableI3[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  double coefR3[6]  = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};  
  double coefI3[6]  = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}; 

  doubleComplex* table1 = DoubleComplexMatrix (tableR1, tableI1, 9);
  doubleComplex* coef1 = DoubleComplexMatrix (coefR1, coefI1, 9);

  doubleComplex* table2 = DoubleComplexMatrix (tableR2, tableI2, 10);
  doubleComplex* coef2 = DoubleComplexMatrix (coefR2, coefI2, 10);

  doubleComplex* table3 = DoubleComplexMatrix (tableR3, tableI3, 10);
  doubleComplex* coef3 = DoubleComplexMatrix (coefR3, coefI3, 10);

  doubleComplex rowStdevfedTable1_3_3[3];
  doubleComplex rowStdevfedTable2_2_5[2];
  doubleComplex rowStdevfedTable3_3_2[3];
 
  rowStdevfedTable1_3_3[0] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable1_3_3[1] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable1_3_3[2] = DoubleComplex(0.0, 0.0);

  rowStdevfedTable2_2_5[0] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable2_2_5[1] = DoubleComplex(0.0, 0.0);

  rowStdevfedTable3_3_2[0] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable3_3_2[1] = DoubleComplex(0.0, 0.0);
  rowStdevfedTable3_3_2[2] = DoubleComplex(0.0, 0.0);


  printf("\n>>>> Column stdevf Double Complex Array Test\n");

  

  zcolumnstdevfa(table1 , 3 , 3 , coef1 , rowStdevfedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {
      printf("columnStdevfedTable_3_3[%d] = %e + %ei\n", i, zreals(rowStdevfedTable1_3_3[i]), zimags(rowStdevfedTable1_3_3[i]));
    }
  assert( fabs(zreals(rowStdevfedTable1_3_3[0])  - 0.8268689) / fabs ( zimags(rowStdevfedTable1_3_3[0])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable1_3_3[0])  - 2.4806066) / fabs ( zreals(rowStdevfedTable1_3_3[0])  ) <  1e-6 );
  
  assert( fabs(zreals(rowStdevfedTable1_3_3[1])  - 0.8164966) / fabs ( zimags(rowStdevfedTable1_3_3[1])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable1_3_3[1])  - 2.4494897) / fabs ( zreals(rowStdevfedTable1_3_3[1])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable1_3_3[2])  - 0.8323524 ) / fabs ( zimags(rowStdevfedTable1_3_3[2])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable1_3_3[2])  - 2.4970571 ) / fabs ( zreals(rowStdevfedTable1_3_3[2])  ) <  1e-6 );
/**/
  zcolumnstdevfa(table2 , 2 , 5 , coef2 , rowStdevfedTable2_2_5);
  for (i = 0 ; i < 2 ; ++i )
    {
      printf("columnStdevfedTable_2_5[%d] = %e + %ei\n", i, zreals(rowStdevfedTable2_2_5[i]), zimags(rowStdevfedTable2_2_5[i]));
    }
  assert( fabs(zreals(rowStdevfedTable2_2_5[0])  - 2.3683811 ) / fabs ( zimags(rowStdevfedTable2_2_5[0])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable2_2_5[0])  - 2.4784193 ) / fabs ( zreals(rowStdevfedTable2_2_5[0])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable2_2_5[1])  - 3.1890422 ) / fabs ( zimags(rowStdevfedTable2_2_5[1])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable2_2_5[1])  - 4.9966774 ) / fabs ( zreals(rowStdevfedTable2_2_5[1])  ) <  1e-6 );

  zcolumnstdevfa(table3 , 3 , 2 , coef3 , rowStdevfedTable3_3_2);
  for (i = 0 ; i < 3 ; ++i )
    {
      printf("columnStdevfedTable3_3_2[%d] = %e + %ei\n", i, zreals(rowStdevfedTable3_3_2[i]), zimags(rowStdevfedTable3_3_2[i]));
    }
  assert( fabs(zreals(rowStdevfedTable3_3_2[0])  - 1.1825083) / fabs ( zimags(rowStdevfedTable3_3_2[0])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable3_3_2[0])  + 0.1170914) / fabs ( zreals(rowStdevfedTable3_3_2[0])  ) <  1e-6 );
  
  assert( fabs(zreals(rowStdevfedTable3_3_2[1])  - 1.3450417) / fabs ( zimags(rowStdevfedTable3_3_2[1])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable3_3_2[1])  + 0.0955892) / fabs ( zreals(rowStdevfedTable3_3_2[1])  ) <  1e-6 );

  assert( fabs(zreals(rowStdevfedTable3_3_2[2])  - 1.4077246 ) / fabs ( zimags(rowStdevfedTable3_3_2[2])  ) <  1e-6 );
  assert( fabs(zimags(rowStdevfedTable3_3_2[2])  + 0.0779670 ) / fabs ( zreals(rowStdevfedTable3_3_2[2])  ) <  1e-6 );
  return 0;
}


static int teststdevf(void) {

  dstdevfsTest();
  dstdevfaTest();
  dcolumnstdevfaTest();
  drowstdevfaTest();
  zstdevfsTest();
  zstdevfaTest();
  zrowstdevfaTest();
  zcolumnstdevfaTest();

  return 0;
}

int main(void) {
  assert(teststdevf() == 0);
  return 0;
}

