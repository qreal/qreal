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

static int sstdevfsTest(void) {
  float value1 = 3.0f;
  float value2 = 1.123456789f;
  float coef1  = 56.0f ;
  float coef2  = 2.0f ;

  printf("\n>>>> stdevf Float Scalar Test\n");
  printf("result : %f " ,sstdevfs(value1,coef1)) ;
  printf("result : %f " ,sstdevfs(value2,coef)) ;
  assert( (  sstdevfs(value1,coef1)  ) ==  0.0f );
  assert( ( sstdevfs(value2,coef2)  ) ==  0.0f );

  value1 = 3.0f;
  value2 = 1.123456789f;
  coef1  = 56.0f ;
  coef2  = 2.0f ;


  return 0;
}

static int sstdevfaTest(void) {
  float table1[3] = {3.0f, 6.0f, 9.0f};
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
 
  float table3[10] = {3.0f, 6.0f, 9.0f,10.0f, 5.0f,
                      6.0f,18.0f, 7.0f,14.0f, 2.0f};
  float coef3 [10] = {3.0f, 8.0f,14.0f,13.0f, 2.0f,
                      5.0f, 8.0f, 2.0f, 6.0f, 8.0f};

  float result_2_5 = sstdevfa(table3,2, 5, coef3);

  printf("\n>>>> stdevf Float Array Test\n");
  printf("\nresult : %f " ,sstdevfa(table1,1, 3, coef1)) ;
  printf("\nresult : %f " ,sstdevfa(table2,1, 5, coef2)) ;
  printf("\nresult : %f " ,result_2_5) ;

  assert(sstdevfa(table1,1, 3, coef1) == 0.0f);
  assert(sstdevfa(table1,1, 3, coef1) == 0.0f);
  assert( ( fabs(result_2_5 ) - (  4.6440201f ) ) / fabs ( result_2_5  ) <  1e-6 );
  return 0;
}


static int scolumnstdevfaTest(void) {

  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float coef1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 

  float columnStdevfedTable1_3_3[3] = {0};
  float columnStdevfedTable1_1_9[1] = {0};


  printf("\n>>>> Column stdevf Float Array Test\n");
  
/**/
  
  scolumnstdevfa(table1, 3, 3, coef1 ,columnStdevfedTable1_3_3);

  printf("\nresult1 : %f " ,columnStdevfedTable1_3_3[0]) ;
  printf("\nresult2 : %f " ,columnStdevfedTable1_3_3[1]) ;
  printf("\nresult3 : %f " ,columnStdevfedTable1_3_3[2]) ;

  assert( fabs(columnStdevfedTable1_3_3[0]  - 0.8268689f ) / fabs ( columnStdevfedTable1_3_3[0]  ) <  1e-6 );
  assert( fabs(columnStdevfedTable1_3_3[1]  - 0.8164966f ) / fabs ( columnStdevfedTable1_3_3[1]  ) <  1e-6 );
  assert( fabs(columnStdevfedTable1_3_3[2]  - 0.8323524f ) / fabs ( columnStdevfedTable1_3_3[2]  ) <  1e-6 );


  
  scolumnstdevfa(table1, 1, 9, coef1 ,columnStdevfedTable1_1_9);
  printf("\nresult_1_9: %f\n " ,columnStdevfedTable1_1_9[0]) ;
  assert( columnStdevfedTable1_1_9[0] == 0.0f);


  return 0;
}


static int srowstdevfaTest(void) {
  int i = 0;
  float table1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float coef1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 

  float table2[10] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f, 10.0f};
  float coef2[10]  = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f, 19.0f}; 



  float rowStdevfedTable1_3_3[3] = {0};
  float rowStdevfedTable1_1_9[9] = {0};
  float rowStdevfedTable2_2_5[5] = {0};
  printf("\n>>>> Row Mean Float Array Test\n");

  
  srowstdevfa(table1, 3, 3, coef1 , rowStdevfedTable1_3_3);
  for ( i = 0 ; i < 3 ; ++i) {
    printf("rowStdevfedTable1_3_3[%d] = %e\n", i, rowStdevfedTable1_3_3[i]);
  }
  assert( fabs(rowStdevfedTable1_3_3[0]  - 2.8394542f ) / fabs ( rowStdevfedTable1_3_3[0]  ) <  1e-6 );
  assert( fabs(rowStdevfedTable1_3_3[1]  - 2.8003759f ) / fabs ( rowStdevfedTable1_3_3[1]  ) <  1e-6 );
  assert( fabs(rowStdevfedTable1_3_3[2]  - 2.7669196f ) / fabs ( rowStdevfedTable1_3_3[2]  ) <  1e-6 );

/**/
  srowstdevfa(table1, 1, 9, coef1, rowStdevfedTable1_1_9);
  for ( i = 0 ; i < 9 ; ++i) {
    printf("rowStdevfedTable1_1_9[%d] = %e\n", i, rowStdevfedTable1_1_9[i]);
    assert(rowStdevfedTable1_1_9[i] == 0.0f);
  }

  
/**/
  /**/
  srowstdevfa(table2 , 2 , 5 , coef2 , rowStdevfedTable2_2_5);
  for (i = 0 ; i < 5 ; ++i )
    {
      printf("rowStdevfedTable_2_5[%d] = %e \n", i, rowStdevfedTable2_2_5[i]);
    }
  assert( fabs(rowStdevfedTable2_2_5[0] - 0.9045340f ) / fabs ( rowStdevfedTable2_2_5[0] ) <  1e-6 );
  assert( fabs(rowStdevfedTable2_2_5[1] - 2.3935678f ) / fabs ( rowStdevfedTable2_2_5[1] ) <  1e-6 );
  assert( fabs(rowStdevfedTable2_2_5[2] - 1.3887301f ) / fabs ( rowStdevfedTable2_2_5[2] ) <  1e-6 );
  assert( fabs(rowStdevfedTable2_2_5[3] - 1.2421180f ) / fabs ( rowStdevfedTable2_2_5[3] ) <  1e-6 );
  assert( fabs(rowStdevfedTable2_2_5[4] - 0.4523443f ) / fabs ( rowStdevfedTable2_2_5[4] ) <  1e-6 );
  return 0;
}


static int cstdevfsTest(void) {
  floatComplex value1;
  floatComplex coef1 ;
  floatComplex value2;
  floatComplex coef2 ;

  value1 = FloatComplex(3.0f, 3.0f);
  coef1 = FloatComplex(3.0f, 0.0f);
  value2 = FloatComplex(1.123456789f, 1.123456789f);
  coef2 = FloatComplex(9.0f, 0.0f);

  printf("\n>>>> Mean Float Complex Scalar Test\n");
  assert(  creals(cstdevfs(value1,coef1)) ==  0.0f );
  assert(  cimags(cstdevfs(value1,coef1)) ==  0.0f );
  assert(  creals(cstdevfs(value2,coef2)) ==  0.0f );
  assert(  cimags(cstdevfs(value2,coef2)) ==  0.0f );

  return 0;
}



static int cstdevfaTest(void) {

  float tableR1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float tableI1[9] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  float coefR1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 
  float coefI1[9] = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 


  float tableR3[10] = {3.0f, 6.0f, 9.0f,10.0f, 5.0f,
                       6.0f,18.0f, 7.0f,14.0f, 2.0f};
  float tableI3[10] = {3.0f,12.0f,25.0f, 1.0f, 2.0f,
                       5.0f,18.0f, 7.0f, 4.0f, 1.0f};


  float coefR3 [10] = {3.0f, 8.0f,14.0f,13.0f, 2.0f,
                       5.0f, 8.0f, 2.0f, 6.0f, 8.0f};
  float coefI3 [10] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 


  floatComplex* table1 = FloatComplexMatrix (tableR1, tableI1, 9);
  floatComplex* coef1 = FloatComplexMatrix (coefR1, coefI1, 9);

  floatComplex* table3 = FloatComplexMatrix (tableR3, tableI3, 10);
  floatComplex* coef3 = FloatComplexMatrix (coefR3, coefI3, 10);

  floatComplex result =FloatComplex(0.0f , 0.0f); 
  
  printf("\n>>>> Mean Float Complex Array Test\n");
  result = cstdevfa(table1,1, 9, coef1);
  printf("\nresult_1_9 : %f \t+ %f i " ,creals(result) ,cimags(result)) ;
  assert(  creals(result) == 0.0f );
  assert(  cimags(result) == 0.0f );


  result = cstdevfa(table3,2, 5, coef3);
  printf("\nresult_2_5 : %f \t+ %f i " ,creals(result) ,cimags(result)) ;
  assert( fabs(creals(result)  - 1.7749350f ) / fabs ( cimags(result)  ) <  1e-6 );
  assert( fabs(cimags(result)  - 8.3811287f ) / fabs ( creals(result)  ) <  1e-6 );

  return 0;
}


static int crowstdevfaTest(void) {

  int i = 0 ;

  float tableR1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float tableI1[9] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  float coefR1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 
  float coefI1[9] = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 

  float tableR2[10] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f, 10.0f};
  float tableI2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 15.0f};
  float coefR2[10]  = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f, 19.0f}; 
  float coefI2[10]   = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 

  floatComplex* table1 = FloatComplexMatrix (tableR1, tableI1, 9);
  floatComplex* coef1 = FloatComplexMatrix (coefR1, coefI1, 9);

  floatComplex* table2 = FloatComplexMatrix (tableR2, tableI2, 10);
  floatComplex* coef2 = FloatComplexMatrix (coefR2, coefI2, 10);

  floatComplex rowStdevfedTable1_3_3[3];
  floatComplex rowStdevfedTable2_2_5[5];

  printf("\n>>>> Row stdevf Float Complex Array Test\n");

  rowStdevfedTable1_3_3[0] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable1_3_3[1] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable1_3_3[2] = FloatComplex(0.0f, 0.0f);

  rowStdevfedTable2_2_5[0] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable2_2_5[1] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable2_2_5[2] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable2_2_5[3] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable2_2_5[4] = FloatComplex(0.0f, 0.0f);


  crowstdevfa(table1 , 3 , 3 , coef1 , rowStdevfedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {
      printf("rowStdevfedTable_3_3[%d] = %e + %ei\n", i, creals(rowStdevfedTable1_3_3[i]), cimags(rowStdevfedTable1_3_3[i]));
    }
  assert( fabs(creals(rowStdevfedTable1_3_3[0])  - 2.8394542f ) / fabs ( cimags(rowStdevfedTable1_3_3[0])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable1_3_3[0])  - 0.9464847f ) / fabs ( creals(rowStdevfedTable1_3_3[0])  ) <  1e-6 );
  
  assert( fabs(creals(rowStdevfedTable1_3_3[1])  - 2.8003759f ) / fabs ( cimags(rowStdevfedTable1_3_3[1])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable1_3_3[1])  - 0.9334586f ) / fabs ( creals(rowStdevfedTable1_3_3[1])  ) <  1e-6 );

  assert( fabs(creals(rowStdevfedTable1_3_3[2])  - 2.7669196f ) / fabs ( cimags(rowStdevfedTable1_3_3[2])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable1_3_3[2])  - 0.9223065f ) / fabs ( creals(rowStdevfedTable1_3_3[2])  ) <  1e-6 );
/**/
  crowstdevfa(table2 , 2 , 5 , coef2 , rowStdevfedTable2_2_5);
  for (i = 0 ; i < 5 ; ++i )
    {
      printf("rowStdevfedTable_2_5[%d] = %e + %ei\n", i, creals(rowStdevfedTable2_2_5[i]), cimags(rowStdevfedTable2_2_5[i]));
    }
  assert( fabs(creals(rowStdevfedTable2_2_5[0])  - 0.9045340f ) / fabs ( cimags(rowStdevfedTable2_2_5[0])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable2_2_5[0])  - 0.3015113f ) / fabs ( creals(rowStdevfedTable2_2_5[0])  ) <  1e-6 );

  assert( fabs(creals(rowStdevfedTable2_2_5[1])  - 2.3935678f ) / fabs ( cimags(rowStdevfedTable2_2_5[1])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable2_2_5[1])  + 0.4787136f ) / fabs ( creals(rowStdevfedTable2_2_5[1])  ) <  1e-6 );

  assert( fabs(creals(rowStdevfedTable2_2_5[2])  - 1.3887301f ) / fabs ( cimags(rowStdevfedTable2_2_5[2])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable2_2_5[2])  - 0.4629100f ) / fabs ( creals(rowStdevfedTable2_2_5[2])  ) <  1e-6 );

  assert( fabs(creals(rowStdevfedTable2_2_5[3])  - 1.2421180f ) / fabs ( cimags(rowStdevfedTable2_2_5[3])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable2_2_5[3])  - 0.4140393f ) / fabs ( creals(rowStdevfedTable2_2_5[3])  ) <  1e-6 );

  assert( fabs(creals(rowStdevfedTable2_2_5[4])  - 0.4523443f ) / fabs ( cimags(rowStdevfedTable2_2_5[4])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable2_2_5[4])  - 2.7140659f ) / fabs ( creals(rowStdevfedTable2_2_5[4])  ) <  1e-6 );

  return 0;
}




static int ccolumnstdevfaTest(void) {

  int i = 0 ;

  float tableR1[9] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f};
  float tableI1[9] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
  float coefR1[9] = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f}; 
  float coefI1[9] = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 

  float tableR2[10] = {1.0f, 4.0f, 7.0f, 2.0f , 5.0f, 8.0f, 3.0f, 6.0f, 9.0f, 10.0f};
  float tableI2[10] = {1.0f, 2.0f, 3.0f, 4.0f , 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 15.0f};
  float coefR2[10]  = {10.0f, 1.0f, 5.0f,11.0f , 2.0f, 6.0f,12.0f, 3.0f, 7.0f, 19.0f}; 
  float coefI2[10]   = { 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; 

  floatComplex* table1 = FloatComplexMatrix (tableR1, tableI1, 9);
  floatComplex* coef1 = FloatComplexMatrix (coefR1, coefI1, 9);

  floatComplex* table2 = FloatComplexMatrix (tableR2, tableI2, 10);
  floatComplex* coef2 = FloatComplexMatrix (coefR2, coefI2, 10);

  floatComplex rowStdevfedTable1_3_3[3];
  floatComplex rowStdevfedTable2_2_5[2];
 
  rowStdevfedTable1_3_3[0] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable1_3_3[1] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable1_3_3[2] = FloatComplex(0.0f, 0.0f);

  rowStdevfedTable2_2_5[0] = FloatComplex(0.0f, 0.0f);
  rowStdevfedTable2_2_5[1] = FloatComplex(0.0f, 0.0f);



  printf("\n>>>> Column stdevf Float Complex Array Test\n");

  

  ccolumnstdevfa(table1 , 3 , 3 , coef1 , rowStdevfedTable1_3_3);

  for (i = 0 ; i < 3 ; ++i )
    {
      printf("columnStdevfedTable_3_3[%d] = %e + %ei\n", i, creals(rowStdevfedTable1_3_3[i]), cimags(rowStdevfedTable1_3_3[i]));
    }
  assert( fabs(creals(rowStdevfedTable1_3_3[0])  - 0.8268689f ) / fabs ( cimags(rowStdevfedTable1_3_3[0])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable1_3_3[0])  - 2.4806066f ) / fabs ( creals(rowStdevfedTable1_3_3[0])  ) <  1e-6 );
  
  assert( fabs(creals(rowStdevfedTable1_3_3[1])  - 0.8164966f ) / fabs ( cimags(rowStdevfedTable1_3_3[1])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable1_3_3[1])  - 2.4494897f ) / fabs ( creals(rowStdevfedTable1_3_3[1])  ) <  1e-6 );

  assert( fabs(creals(rowStdevfedTable1_3_3[2])  - 0.8323524f ) / fabs ( cimags(rowStdevfedTable1_3_3[2])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable1_3_3[2])  - 2.4970571f ) / fabs ( creals(rowStdevfedTable1_3_3[2])  ) <  1e-6 );
/**/
  ccolumnstdevfa(table2 , 2 , 5 , coef2 , rowStdevfedTable2_2_5);
  for (i = 0 ; i < 2 ; ++i )
    {
      printf("columnStdevfedTable_2_5[%d] = %e + %ei\n", i, creals(rowStdevfedTable2_2_5[i]), cimags(rowStdevfedTable2_2_5[i]));
    }
  assert( fabs(creals(rowStdevfedTable2_2_5[0])  - 2.3683811f ) / fabs ( cimags(rowStdevfedTable2_2_5[0])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable2_2_5[0])  - 2.4784193f ) / fabs ( creals(rowStdevfedTable2_2_5[0])  ) <  1e-6 );

  assert( fabs(creals(rowStdevfedTable2_2_5[1])  - 3.1890422f ) / fabs ( cimags(rowStdevfedTable2_2_5[1])  ) <  1e-6 );
  assert( fabs(cimags(rowStdevfedTable2_2_5[1])  - 4.9966774f ) / fabs ( creals(rowStdevfedTable2_2_5[1])  ) <  1e-6 );


  return 0;
}


static int teststdevf(void) {

  sstdevfsTest();
  sstdevfaTest();
  scolumnstdevfaTest();
  srowstdevfaTest();
  cstdevfsTest();
  cstdevfaTest();
  crowstdevfaTest();
  ccolumnstdevfaTest();

  return 0;
}

int main(void) {
  assert(teststdevf() == 0);
  return 0;
}

