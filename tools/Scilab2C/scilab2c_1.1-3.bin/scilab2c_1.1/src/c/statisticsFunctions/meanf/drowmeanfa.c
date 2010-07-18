/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "meanf.h"
#include "sum.h"

void drowmeanfa(double *in1, int lines, int columns, double *in2, double *out) {
  int i = 0;
  int j = 0;
  double tempCoefSum = 0.0;
  double tempMul = 0.0;
  /*we first multiply each cell of the input matrix by its coefficient*/
  for (j = 0; j < columns; ++j)
  {
    tempCoefSum = 0.0;
    out[j]= 0.0;
    for ( i = 0 ; i < lines; ++i )
      {

        tempMul = in1[lines*j + i] * in2[lines*j + i]; /* we times by the coefficient*/
        tempCoefSum +=in2[lines*j + i];
        out[j] +=tempMul ;
      } 

    out[j] /= tempCoefSum ;
  }
}
