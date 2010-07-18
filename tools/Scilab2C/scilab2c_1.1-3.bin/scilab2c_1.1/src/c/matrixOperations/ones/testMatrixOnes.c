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

#include "ones.h"

#define ROWS 20
#define COLS 50

static void donesaTest ( void )
{
    int i = 0 ;

    double result = 0 ;

    double in [ROWS*COLS] ;

    donesa ( in , ROWS , COLS ) ;

    for ( i = 0 ; i <  ROWS*COLS ; i++ )
        {
         result = 1 ;

         printf ( "\t\t %d in : %e\tresult : %e\tassert : %e \n" , i, in[i] , result , fabs( in[i] - result) / fabs( in[i]) ) ;

         if ( in[i] < 1e-14 && result < 1e-14 )
            assert(1);
         else
            assert ( fabs ( in[i] - result) / fabs( in[i]) < 3e-16 ) ;
        }
}


static void sonesaTest ( void )
{
    int i = 0 ;

    float result = 0 ;

    float in[ROWS*COLS] ;

    sonesa ( in , ROWS , COLS ) ;

    for ( i = 0 ; i <  ROWS*COLS ; i++)
        {
         result = 1.0f ;

         printf ( "\t\t %d in : %e\tresult : %e\tassert : %e \n" , i, in[i] , result , fabs( in[i] - result) / fabs( in[i]) ) ;
         if ( in[i] < 1e-6 && result < 1e-6 )
            assert(1);
         else
		    assert ( fabs ( in[i] - result) / fabs( in[i]) < 1e-6 ) ;
        }
}


static void zonesaTest ( void )
{
    int i = 0 ;

    doubleComplex result = DoubleComplex ( 0 , 0) ;

    doubleComplex in[ROWS*COLS] ;

    zonesa ( in , ROWS , COLS ) ;

    for ( i = 0 ; i <  ROWS*COLS ; i++ )
        {
         result = DoubleComplex ( 1 , 0 ) ;


         printf ( "\t\t %d in : %e %e*i\tresult : %e %e*i\n" , i, zreals( in[i]) , zimags(in[i]), zreals ( result ),zimags(result) ) ;

         if ( zreals( in[i]) < 1e-14 && zreals( result) < 1e-14 )
            assert(1);
         else
		    assert ( fabs ( zreals(in[i]) - zreals(result)) / fabs( zreals(in[i])) < 1e-16 ) ;

        if ( zimags ( in[i]) < 1e-14 )
            assert (1);
        else
            assert (0);

        }
}





static void conesaTest ( void )
{
    int i = 0 ;

    floatComplex result = FloatComplex ( 0 , 0  ) ;

    floatComplex in[ROWS*COLS] ;

    conesa ( in , ROWS , COLS ) ;

    for ( i = 0 ; i <  ROWS*COLS ; i++)
        {
        result = FloatComplex ( 1.0f , 0 ) ;


         printf ( "\t\t %d in : %e %e*i\tresult : %e %e*i\n" , i, creals( in[i]) , cimags(in[i]), creals ( result ),cimags(result) ) ;
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



static int testEye(void) {

  printf("\n>>>> Matrix ONES Tests\n");
  printf("\t>>>> Matrix Double Realt Tests\n");
  donesaTest();

  printf("\n\n\t>>>> Matrix Float Realt Tests\n");
  sonesaTest();

  printf("\n\n\n");
  printf("\t>>>> Matrix Float Complex Tests\n");
  conesaTest();

  printf("\n\n\n");
  printf("\t>>>> Matrix Double Complex Tests\n");
  zonesaTest();

  return 0;
}



int main(void) {
  assert(testEye() == 0);
  return 0;
}

