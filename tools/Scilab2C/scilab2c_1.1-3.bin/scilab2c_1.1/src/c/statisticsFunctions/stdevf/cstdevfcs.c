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

floatComplex cstdevfcs(floatComplex *in1, int lines, int columns, float* in2)
{
  int i = 0 ;
  floatComplex temp = FloatComplex(0.0f,0.0f);
  floatComplex accumulate = FloatComplex(0.0f,0.0f);
  float accumulateFre = 0.0f ;
  floatComplex meanf = cmeanfcs (in1 , lines ,columns , in2);
/*equivalent to  (in1 - meanf(x , in2 )).^2 .*in2   */
  for(i = 0 ; i < lines*columns ; ++i)
    {
     temp = cpows (  cdiffs (in1[i] , meanf ) ,FloatComplex (2.0f, 0.0f ) );
     temp = cmuls( FloatComplex (in2[i],0.0f) , temp);

     accumulate = cadds( temp , accumulate);
     accumulateFre +=  in2[i];
    }

    if (lines <= 1)
      {
        return cmuls (FloatComplex(0.0f,0.0f) , accumulate ) ; 
      }
    else
      {
        accumulate = FloatComplex(  creals(accumulate ) / (accumulateFre - 1) , cimags(accumulate)  / (accumulateFre - 1));
        return csqrts(accumulate);
      }
}
