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

double	dvariancefa(double *in1, int size , double * in2)
{
  int i = 0 ;
  double temp = 0.0;
  double accumulate = 0.0 ;
  double accumulateFre = 0.0 ;

  double meanf = dmeanfa (in1 , size , in2);

  for(i = 0 ; i < size ; ++i)
    {
     temp = dpows (  (in1[i]  - meanf ) ,2 );
     temp *= in2[i];

     accumulate += temp ;
     accumulateFre += in2[i];
    }

  return accumulate / (accumulateFre -1);
}

