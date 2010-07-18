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

#include "zeros.h"

#define ROWS 20
#define COLS 50

static void dzerosaTest ( void )
{
    int i = 0 ;

    double result = 0 ;

    double in [ROWS*COLS] ;

    dzerosa ( in , ROWS , COLS ) ;

    for ( i = 0 ; i <  ROWS*COLS ; i++ )
        {
         result = 0 ;

         printf ( "\t\t %d in : %e\tresult : %e\tassert : %e \n" , i, in[i] , result , fabs( in[i] - result) / fabs( in[i]) ) ;

         assert ( fabs ( in[i] - result) == 0 ) ;
        }
}


static void szerosaTest ( void )
{
    int i = 0 ;

    float result = 0 ;

    float in[ROWS*COLS] ;

    szerosa ( in , ROWS , COLS ) ;

    for ( i = 0 ; i <  ROWS*COLS ; i++)
        {
         result = 0.0f ;


         assert ( fabs ( in[i] - result) == 0 ) ;
        }
}


static void zzerosaTest ( void )
{
    int i = 0 ;

    doubleComplex result = DoubleComplex ( 0 , 0) ;

    doubleComplex in[ROWS*COLS] ;

    zzerosa ( in , ROWS , COLS ) ;

    for ( i = 0 ; i <  ROWS*COLS ; i++ )
        {
         result = DoubleComplex ( 0 , 0 ) ;


         assert ( fabs ( zreals(in[i]) - zreals(result)) == 0 ) ;
         assert ( fabs ( zimags(in[i]) - zimags(result)) == 0 ) ;

        }
}





static void czerosaTest ( void )
{
    int i = 0 ;

    floatComplex result = FloatComplex ( 0 , 0  ) ;

    floatComplex in[ROWS*COLS] ;

    czerosa ( in , ROWS , COLS ) ;

    for ( i = 0 ; i <  ROWS*COLS ; i++)
        {
        result = FloatComplex ( 0.0f , 0 ) ;


         assert ( fabs ( creals(in[i]) - creals(result)) == 0 ) ;
         assert ( fabs ( cimags(in[i]) - cimags(result)) == 0 ) ;

        }
}



static int testZeros(void) {

  printf("\n>>>> Matrix ZEROS Tests\n");
  printf("\t>>>> Matrix Double Real Tests\n");
  dzerosaTest();

  printf("\n\n\t>>>> Matrix Float Real Tests\n");
  szerosaTest();

  printf("\n\n\n");
  printf("\t>>>> Matrix Float Complex Tests\n");
  czerosaTest();

  printf("\n\n\n");
  printf("\t>>>> Matrix Double Complex Tests\n");
  zzerosaTest();

  return 0;
}



int main(void) {
  assert(testZeros() == 0);
  return 0;
}

