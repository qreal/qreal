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

float sstdevfa(float *in1, int lines, int columns, float* in2)
{
  int i = 0 ;
  float meanf = smeanfa (in1 , lines*columns , in2);
  float temp = 0.0f;
  float accumulate = 0.0f ;
  float accumulateFre = 0.0f ;
    

    /*equivalent to  (in1 - meanf(x , in2 )).^2 .*in2   */
    for(i = 0 ; i <  lines*columns; ++i)
      {
     
       temp = spows (  (in1[i]  - meanf ) ,2 );
       temp *= in2[i];

       accumulate += temp ;
       accumulateFre += in2[i];
      }

    if (lines <= 1)
      {
        return 0.0f * accumulate ; 
      }
    else
      {
        return ssqrts(  accumulate/(accumulateFre - 1) );
      }
}
/*
                               
!    o = '*'                                                                                        !

!  //remove the median                                                                              !
!                                                                                                   !
!  if o == '*' then                                                                                 !
!                                                                                                   !
!    y = x - meanf(x, fre)                                                                          !
!                                                                                                   !
!  elseif o == 'r' | o == 1 then                                                                    !
!                                                                                                   !
!    y = x - ones(size(x, o), 1) * meanf(x, fre, o)                                                 !
!                                                                                                   !
!  elseif o == 'c' | o == 2 then                                                                    !
!                                                                                                   !
!    y = x - meanf(x, fre, o) * ones(1, size(x, o))                                                 !
!                                                                                                   !

!  if size(x, 1) == 1 then                                                                          !                                                                                              !
!    s = 0 * sum(y.^2 .* fre, o)                                                                                                                                                                 !
!  else                                                                                             !                                                                                               !
!    s = sqrt(   sum(y.^2 .* fre, o) ./ (sum(fre, o) - 1)   );                                            !
                                                                                      !
*/           
