/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Allan Simon
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "string.h"

#define SIZE 10000

/*
** \floating scalars test
*/
static void sstringaTest (void) {
   float tabF[SIZE];
   float tabF2[SIZE];
   char**  tabC = NULL ;
   int i = 0 ;

	printf("\n>>>> sstringaTest Tests\n");

   tabC = (char**)malloc( SIZE *sizeof(char*));

   for ( i=0;i< SIZE ; ++i )
      {
       tabC[i] = (char *)malloc(  26 * sizeof(char));
	  }

   srand( 1 ) ;

   for ( i = 0 ; i < SIZE ; ++i)
      {
         tabF[i] = (float) rand () ;
      }

/* call to the function to test */
   sstringa ( tabF , SIZE , tabC ) ;

/* reverse convertion*/
   for ( i = 0  ; i < SIZE ; ++i)
      {
         sscanf  ( tabC[i] , "%e" ,  &(tabF2[i]) ) ;
      }

/* and we compare if the initial and final floats are equal */

   for ( i = 0  ; i < SIZE ; ++i)
     {
      assert ( fabs( tabF[i]  - tabF2[i]) / ( fabs ( tabF[i] ))  < 1e-07 )  ;
     }



}


/*
** \double scalars test
*/

static void dstringaTest (void) {
   double tabD[SIZE];
   double tabD2[SIZE];
   char**  tabC = NULL ;
   int i = 0 ;

   srand( 1 ) ;

	printf("\n>>>> dstringaTest Tests\n");

   tabC = (char**)malloc( SIZE *sizeof(char*));

   for ( i=0;i< SIZE ; ++i )
      {
       tabC[i] = (char *)malloc(  26 * sizeof(char));
	  }


   for ( i = 0 ; i < SIZE ; ++i)
      {
         tabD[i] = (double) rand () ;
      }

/* call to the function to test */

   dstringa ( tabD , SIZE , tabC ) ;
/* reverse convertion*/
   for ( i = 0  ; i < SIZE ; ++i)
      {

         tabD2[i] =  strtod (tabC[i]  , NULL);

      }

/* and we compare if the initial and final doubles are equal */

   for ( i = 0  ; i < SIZE ; ++i)
     {
      assert ( fabs( tabD[i]  - tabD2[i]) / ( fabs ( tabD[i] ))  < 1e-16 )  ;
     }



}


/*
** \complex floats test
*/

static void cstringaTest (void) {
  floatComplex tabF[SIZE];
  floatComplex tabF2[SIZE] ;
  float img ;
  float real ;
  char**  tabC = NULL ;

  int i = 0 ;

   srand( 1 ) ;

	printf("\n>>>> cstringaTest Tests\n");
   tabC = (char**)malloc( SIZE *sizeof(char*));

   for ( i=0;i< SIZE ; ++i )
      {
       tabC[i] = (char *)malloc(  53 * sizeof(char));
	  }

   for ( i = 0 ; i < SIZE ; ++i)
      {
         tabF[i] = FloatComplex ((float) rand (), (float) rand());
      }


   cstringa ( tabF , SIZE , tabC ) ;

/* reverse convertion*/

   for ( i = 0  ; i < SIZE ; ++i)
      {
		sscanf (tabC[i]  , "%e + %e" , &real , &img );


        tabF2[i] = FloatComplex (real ,img );
      }


/* and we compare if the initial and final  floats are equal */


   for ( i = 0  ; i < SIZE ; ++i)
     {
      assert ( fabs(  creals(tabF2[i]) -  creals (tabF[i]) ) / fabs (creals (tabF[i]))  < 1e-07 );
	  assert ( fabs(  cimags(tabF2[i]) -  cimags (tabF[i]) ) / fabs (cimags (tabF[i]))  < 1e-07 ) ;
     }



}


/*
** \double 07 test
*/
static void zstringaTest (void) {
  doubleComplex tabD[SIZE];
  doubleComplex tabD2[SIZE] ;
  double img ;
  double real ;
  char* temp = NULL ;
  char**  tabC  = NULL ;

  int i = 0 ;

   srand( 1 ) ;

	printf("\n>>>> zstringaTest Tests\n");

   tabC = (char**)malloc( SIZE *sizeof(char*));

   for ( i=0;i< SIZE ; ++i )
      {
       tabC[i] = (char *)malloc(  53 * sizeof(char));
	  }


   for ( i = 0 ; i < SIZE ; ++i)
      {
         tabD[i] = DoubleComplex ((double) rand (), (double) rand());
      }

   zstringa ( tabD , SIZE , tabC ) ;

/* reverse convertion*/
   for ( i = 0  ; i < SIZE ; ++i)
      {
		real = (double) strtod (tabC[i]  ,&temp );
		img  = (double) strtod (temp + 3 ,NULL );

        tabD2[i] = DoubleComplex (real ,img );
      }
/* and we compare if the initial and final doubles are equal */

   for ( i = 0  ; i < SIZE ; ++i)
     {
      assert ( fabs(  zreals(tabD2[i]) -  zreals (tabD[i]) ) / fabs (zreals (tabD[i])) < 1e-16 );
	  assert ( fabs(  zimags(tabD2[i]) -  zimags (tabD[i]) ) / fabs (zimags (tabD[i])) < 1e-16 );
     }


}

static int testString    (void) {

  printf("\n>>>> String Tests\n");
  sstringaTest();
  dstringaTest();
  cstringaTest();
  zstringaTest();

  return 0;
}

int main(void) {
  assert(testString() == 0);
  return 0;
}
