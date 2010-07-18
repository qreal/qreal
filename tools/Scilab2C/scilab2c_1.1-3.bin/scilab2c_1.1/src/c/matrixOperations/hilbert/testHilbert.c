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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "hilbert.h"

#define SIZE 6

static void shilbaTest ( void ){
    
    int i = 0 ;
    int j = 0 ; 
    

    float Result[] =    
    {36.0f,-630.0f,3360.0f,-7560.0f,7560.0f,-2772.0f,
-630.0f,14700.0f,-88200.0f,211680.0f,-220500.0f,83160.0f,
3360.0f,-88200.0f,564480.0f,-1411200.0f,1512000.0f,-582120.0f,
-7560.0f,211680.0f,-1411200.0f,3628800.0f,-3969000.0f,1552320.0f,
7560.0f,-220500.0f,1512000.0f,-3969000.0f,4410000.0f,-1746360.0f,
-2772.0f,83160.0f,-582120.0f,1552320.0f,-1746360.0f,698544.0f};

    
    
    float out[SIZE*SIZE] ;
    shilba ( out , SIZE ) ;
    
    for (i  = 0 ; i < SIZE ; i++)
    {
       printf ("\n\t" ) ; 
       for ( j = 0 ; j < SIZE; j++)
       printf ("%e\t" , out[i + j*SIZE] ); 

    }

    for ( i = 0 ; i < SIZE*SIZE  ; i++ )
	{
		assert ( fabs ( out[i] - Result[i] ) / fabs( out[i]) < 1e-06 ) ;
	}

      printf ("\n\n" ) ; 
}

static void dhilbaTest ( void ){
    
    int i = 0 ;
    int j = 0 ; 
    
    double Result[] =   
{36,-630,3360,-7560,7560,-2772,
-630,14700,-88200,211680,-220500,83160,
3360,-88200,564480,-1411200,1512000,-582120,
-7560,211680,-1411200,3628800,-3969000,1552320,
7560,-220500,1512000,-3969000,4410000,-1746360,
-2772,83160,-582120,1552320,-1746360,698544}
;
    
    
    double out[SIZE*SIZE] ;
    
    dhilba ( out , SIZE ) ;
    
    for (i  = 0 ; i < SIZE ; i++)
    {
       printf ("\n\t" ) ; 
       for ( j = 0 ; j < SIZE; j++)
       printf ("%e\t" , out[i + j*SIZE] ); 
        
    }
    
    
    for ( i = 0 ; i < SIZE*SIZE  ; i++ )
	{
			assert ( fabs ( out[i] - Result[i] ) / fabs( out[i]) < 1e-16 ) ;
	}
    
    printf ("\n\n" ) ;
}



static int testHilbert(void) {

  printf("\n>>>>Hilbert's Matrix Tests\n");
    
  printf("\n\t>>> Float Test\n");
  shilbaTest();
    
  printf("\n\t>>> Double Test\n");  
  dhilbaTest();

  return 0;
}



int main(void) {
  assert(testHilbert() == 0);
  return 0;
}
