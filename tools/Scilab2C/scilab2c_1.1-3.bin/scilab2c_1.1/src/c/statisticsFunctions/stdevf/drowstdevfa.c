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


#include "stdevf.h"


void drowstdevfa(double *in1, int lines, int columns, double *in2, double* out){
  int i = 0;
  int j = 0;
  double temp = 0.0f;
  double accumulate = 0.0f;
  double accumulateFre = 0.0f ;
    
  drowmeanfa(in1, lines, columns, in2, out );


  /*we first multiply each cell of the input matrix by its coefficient*/
  for (j = 0; j < columns; ++j)
    {
      accumulate = 0.0f;
      accumulateFre= 0.0f;
      temp = 0.0f;

      for ( i = 0 ; i < lines; ++i )
        {
         temp = dpows (  (in1[lines*j + i]  - out[j] ) ,2 );
         temp *= in2[lines*j + i];

         accumulate += temp ;
         accumulateFre += in2[lines*j + i];
        } 

      if (lines <= 1)
       {
        out[j] =  0.0 * accumulate ; 
       }
      else
       {
        out[j] = dsqrts(accumulate / (accumulateFre - 1));
       }
    }

}
