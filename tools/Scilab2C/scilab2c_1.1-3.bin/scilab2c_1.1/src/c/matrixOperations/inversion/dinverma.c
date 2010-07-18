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
#include <malloc.h>
#include "matrixInversion.h" 
#include "lapack.h"
#include <stdio.h>


void dinverma ( double* in, double* out, int leadDimIn )
{
   int info = 0 ;
   int* vectPivot = (int*) malloc ( sizeof(int) * (unsigned int)( leadDimIn*leadDimIn) );
   double* work  =  (double*) malloc ( sizeof(double) * (unsigned int) (leadDimIn*leadDimIn) );

   int i = 0 ;

   for ( i = 0 ; i < leadDimIn*leadDimIn ; i ++)
      {
         out[i] = in[i] ;
      }

    dgetrf_ ( &leadDimIn, &leadDimIn, out, &leadDimIn, vectPivot, &info );   
    dgetri_ ( &leadDimIn, out, &leadDimIn , vectPivot,  work , &leadDimIn , &info ); 
    
    free(vectPivot);
    free(work);


}

