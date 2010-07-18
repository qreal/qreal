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

#include <assert.h>
#include <stdlib.h>
#include "disp.h"


#define SIZE 10000

/*
** \float scalars displaying test
*/
static void sdispaTest (void) {
   
   int i = 0 ;
   float tabF[SIZE] ;
	
   for ( i = 0 ; i < SIZE ; ++i)
      {
         tabF[i] = (float) rand () ;
      }
	
	sdispa ( tabF, 1, SIZE ); 
}

/*
** \double scalars displaying test
*/

static void ddispaTest (void) {
   
   int i = 0 ;
   double tabD[SIZE] ;
	
   for ( i = 0 ; i < SIZE ; ++i)
      {
         tabD[i] = (double) rand () ;
      }
	
	ddispa ( tabD, 1, SIZE ) ;
	
}


/*
** \float complexes displaying test
*/
static void cdispaTest (void) {

   int i = 0 ;
   floatComplex tabF[SIZE] ;

   for ( i = 0 ; i < SIZE ; ++i)
      {
         tabF[i] = FloatComplex ((float) rand (), (float) rand());
      }
	
   	cdispa ( tabF, 1, SIZE ) ;
}



/*
** \double complexes displaying test
*/
static void zdispaTest (void) {
	
   int i = 0 ;
   doubleComplex tabD[SIZE] ;

   for ( i = 0 ; i < SIZE ; ++i)
      {
         tabD[i] = DoubleComplex ((double) rand (), (double) rand());
      }
	
   	zdispa ( tabD, 1, SIZE ) ;
	
}



static int testDisp   (void) {

  printf("\n>>>> Disp Tests\n");
  sdispaTest();
  ddispaTest();
  cdispaTest();
  zdispaTest();

  return 0;
}


int main(void) {
  assert(testDisp() == 0);
  return 0;
}
