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

double dstdevfa(double *in1, int lines, int columns, double* in2)
{
  int i = 0 ;
  double meanf = dmeanfa (in1 , lines*columns , in2);
  double temp = 0.0;
  double accumulate = 0.0 ;
  double accumulateFre = 0.0 ;
    /*printf ("\nmeanf %lf \n" ,meanf);*/

    /*equivalent to  (in1 - meanf(x , in2 )).^2 .*in2   */
    for(i = 0 ; i <  lines*columns; ++i)
      {
     
       temp = dpows (  (in1[i]  - meanf ) ,2 );
       temp *= in2[i];

       accumulate += temp ;
       accumulateFre += in2[i];
      }

    if (lines <= 1)
      {
        return 0.0 * accumulate ; 
      }
    else
      {
        return dsqrts(accumulate) / (accumulateFre - 1);
      }
}
