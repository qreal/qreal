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

#include "eye.h"

#define LEADDIM 10
#define ROWS    10
#define COLS    5

#define ROWS2    5
#define COLS2    10

static void deyeaTest ( void )
{
    int i, j = 0 ;

    double result = 0 ;
    double in [LEADDIM*LEADDIM] ;
    double in2 [ROWS * COLS];
    double in3 [ROWS2 * COLS2];

    deyea ( in , LEADDIM , LEADDIM ) ;
    for ( i = 0 ; i <  LEADDIM*LEADDIM ; i++ )
        {
            if ( i%(LEADDIM+1) == 0 )
                result = 1 ;
            else
                result = 0 ;

         printf ( "\t\t %d in : %e\tresult : %e\tassert : %e \n" , i, in[i] , result , fabs( in[i] - result) / fabs( in[i]) ) ;

         if ( in[i] < 1e-14 && result < 1e-14 )
            assert(1);
         else
            assert ( fabs ( in[i] - result) / fabs( in[i]) < 3e-16 ) ;
        }
    
    deyea ( in2, ROWS, COLS );
    for ( i = 0 ; i <  ROWS ; i++ )
        {
            for ( j = 0 ; j < COLS ; ++j )
            {
                if ( i == j )
                    result = 1 ;
                else
                    result = 0 ;

                printf ( "\t\t in[%d, %d] : %e\tresult : %e\tassert : %e \n" , i, j,
                        in[i * ROWS + j] , result ,
                        fabs( in[i * ROWS + j] - result) / fabs( in[i * ROWS + j]) ) ;

                if ( in[i * ROWS + j] < 1e-14 && result < 1e-14 )
                    assert(1);
                else
                    assert ( fabs ( in[i * ROWS + j] - result) / fabs( in[i * ROWS + j]) < 3e-16 ) ;
            }
        }

    deyea ( in3, ROWS2, COLS2);
    for ( j = 0 ; j < COLS2 ; ++j )
        {
            for ( i = 0 ; i <  ROWS2 ; i++ )
            {
                if ( i == j )
                    result = 1 ;
                else
                    result = 0 ;

                printf ( "\t\t in[%d, %d] : %e\tresult : %e\tassert : %e \n" , i, j,
                        in3[j * ROWS2 + i] , result ,
                        fabs( in3[j * ROWS2 + i] - result) / fabs( in3[j * ROWS2 + i]) ) ;

                if ( in3[j * ROWS2 + i] < 1e-14 && result < 1e-14 )
                    assert(1);
                else
                    assert ( fabs ( in3[j * ROWS2 + i] - result) / fabs( in3[j * ROWS2 + i]) < 3e-16 ) ;
            }
        }
    
}


static void seyeaTest ( void )
{
    int i, j = 0 ;

    float result = 0 ;

    float in[LEADDIM*LEADDIM] ;
    float in2 [ROWS * COLS];
    float in3 [ROWS2 * COLS2];
    seyea ( in , LEADDIM , LEADDIM ) ;

    for ( i = 0 ; i <  LEADDIM*LEADDIM ; i++)
        {
            if ( i%(LEADDIM+1) == 0 )
                result = 1.0f ;
            else
                result = 0 ;

         printf ( "\t\t %d in : %e\tresult : %e\tassert : %e \n" , i, in[i] , result , fabs( in[i] - result) / fabs( in[i]) ) ;
         if ( in[i] < 1e-6 && result < 1e-6 )
            assert(1);
         else
		    assert ( fabs ( in[i] - result) / fabs( in[i]) < 1e-6 ) ;
        }
    
    seyea ( in2, ROWS, COLS );
    for ( i = 0 ; i <  ROWS ; i++ )
        {
            for ( j = 0 ; j < COLS ; ++j )
            {
                if ( i == j )
                    result = 1 ;
                else
                    result = 0 ;

                printf ( "\t\t in[%d, %d] : %e\tresult : %e\tassert : %e \n" , i, j,
                        in[i * ROWS + j] , result ,
                        fabs( in[i * ROWS + j] - result) / fabs( in[i * ROWS + j]) ) ;

                if ( in[i * ROWS + j] < 1e-8 && result < 1e-8 )
                    assert(1);
                else
                    assert ( fabs ( in[i * ROWS + j] - result) / fabs( in[i * ROWS + j]) < 1e-8 ) ;
            }
        }

    seyea ( in3, ROWS2, COLS2);
    for ( j = 0 ; j < COLS2 ; ++j )
        {
            for ( i = 0 ; i <  ROWS2 ; i++ )
            {
                if ( i == j )
                    result = 1 ;
                else
                    result = 0 ;

                printf ( "\t\t in[%d, %d] : %e\tresult : %e\tassert : %e \n" , i, j,
                        in3[j * ROWS2 + i] , result ,
                        fabs( in3[j * ROWS2 + i] - result) / fabs( in3[j * ROWS2 + i]) ) ;

                if ( in3[j * ROWS2 + i] < 1e-14 && result < 1e-14 )
                    assert(1);
                else
                    assert ( fabs ( in3[j * ROWS2 + i] - result) / fabs( in3[j * ROWS2 + i]) < 1e-14 ) ;
            }
        }

}


static void zeyeaTest ( void )
{
    int i = 0 ;

    doubleComplex result = DoubleComplex ( 0 , 0) ;

    doubleComplex in[LEADDIM*LEADDIM] ;

    zeyea ( in , LEADDIM , LEADDIM ) ;

    for ( i = 0 ; i <  LEADDIM*LEADDIM ; i++ )
        {
            if ( i%(LEADDIM+1) == 0 )
                result = DoubleComplex ( 1 , 0 ) ;
            else
                result = DoubleComplex ( 0 , 0 ) ;

         printf ( "\t\t %d in : %e\tresult : %e \n" , i, zreals( in[i]) , zreals ( result ) ) ;

         if ( zreals( in[i]) < 1e-14 && zreals( result) < 1e-14 )
            assert(1);
         else
		    assert ( fabs ( zreals(in[i]) - zreals(result)) / fabs( zreals(in[i])) < 3e-16) ;

        if ( zimags ( in[i]) < 1e-14 )
            assert (1);
        else
            assert (0);

        }
}





static void ceyeaTest ( void )
{
    int i = 0 ;

    floatComplex result = FloatComplex ( 0 , 0  ) ;

    floatComplex in[LEADDIM*LEADDIM] ;

    ceyea ( in , LEADDIM , LEADDIM ) ;

    for ( i = 0 ; i <  LEADDIM*LEADDIM ; i++)
        {
            if ( i%(LEADDIM+1) == 0 )
                result = FloatComplex ( 1.0f , 0 ) ;
            else
                result = FloatComplex ( 0    , 0 ) ;

         printf ( "\t\t %d in : %e\tresult : %e \n" , i, creals( in[i]) , creals ( result ) ) ;
         if ( creals( in[i]) < 1e-6 && creals( result) < 1e-6 )
            assert(1);
         else
		    assert ( fabs ( creals(in[i]) - creals(result)) / fabs( creals(in[i])) < 1e-6 ) ;

        if ( cimags ( in[i]) < 1e-6 )
            assert (1);
        else
            assert (0);

        }
}

/*  EYES TEST  */

static void ceyesTest ( void )
{
  

    floatComplex result = FloatComplex ( 1.0f , 0  ) ;

    floatComplex in = FloatComplex ( LEADDIM, LEADDIM ) ;

    in = ceyes ( in  ) ;


   
     assert (  (creals(in) - creals(result)) / fabs( creals(in)) < 1e-6 ) ;
    assert  (  cimags (in) == 0) ;
}





static void zeyesTest ( void )
{

    doubleComplex result = DoubleComplex ( 1 , 0  ) ;

    doubleComplex in = DoubleComplex ( LEADDIM , LEADDIM ) ;

    in = zeyes ( in  ) ;


     assert (  (zreals(in) - zreals(result)) / fabs( zreals(in)) < 1e-16 ) ;
    assert  (  zimags (in) == 0) ;
}



static void deyesTest ( void )
{


    double result =  1  ;

    double in = LEADDIM ;

    in = deyes ( in  ) ;


     assert ( (in - result) / in < 1e-16 ) ; ;
}



static void seyesTest ( void )
{


    float result =  1.0f  ;

    float in = 154.0f ;

    in = seyes ( in  ) ;


     assert ( (in - result) / in < 1e-8 ) ;

}



static int testEye(void) {

  printf("\n>>>> Matrix Eye Tests\n");
  printf("\t>>>> Matrix Double Realt Tests\n");
  deyeaTest();

  printf("\n\n\t>>>> Matrix Float Realt Tests\n");
  seyeaTest();

  printf("\n\n\n");
  printf("\t>>>> Matrix Float Complex Tests\n");
  ceyeaTest();

  printf("\n\n\n");
  printf("\t>>>> Matrix Double Complex Tests\n");
  zeyeaTest();
    
    
    
    
  printf("\t>>>> Matrix Double Realt Tests\n");
  deyesTest();

  printf("\n\n\t>>>> Matrix Float Realt Tests\n");
  seyesTest();

  printf("\n\n\n");
  printf("\t>>>> Matrix Float Complex Tests\n");
  ceyesTest();

  printf("\n\n\n");
  printf("\t>>>> Matrix Double Complex Tests\n");
  zeyesTest();

  return 0;
}



int main(void) {
  assert(testEye() == 0);
  return 0;
}


