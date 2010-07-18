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

#include "variancef.h" 

/*
s_moyennepond = meanf(x, fre)
s_ecartmoyenne = x - s_moyennepond
s_ecartcarre  = s_ecartmoyenne.^2 
s_ecartcarrepondere = s_ecartcarre .* fre
s = sum( s_ecartcarrepondere )/(sumfre - 1), 
*/
float svariancefa(float *in1, int size, float* in2)
{
  int i = 0 ;
  float temp = 0.0f;
  float accumulate = 0.0f ;
  float accumulateFre = 0.0f ;

  float meanf = smeanfa (in1 , size , in2);
  /*printf ("\nmeanf %lf \n" ,meanf);*/
  for(i = 0 ; i < size ; ++i)
    {
     
     temp = spows (  (in1[i]  - meanf ) ,2 );
     temp *= in2[i];

     accumulate += temp ;
     accumulateFre += in2[i];


    }

  return accumulate / (accumulateFre - 1) ;
}


