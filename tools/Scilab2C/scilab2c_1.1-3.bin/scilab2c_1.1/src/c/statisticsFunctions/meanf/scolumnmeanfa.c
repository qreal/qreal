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

#include "meanf.h"

void scolumnmeanfa(float *in1, int lines, int columns, float *in2, float *out) {
  int i = 0;
  int j = 0;
  float tempCoefSum = 0.0f;
  float tempMul = 0.0f ;



  /*we first multiply each cell of the input matrix by its coefficient*/
for (j = 0; j < lines; ++j)
  {
    tempCoefSum = 0.0f;
    out[j]= 0.0f;
    for ( i = 0 ; i < columns; ++i )
      {

        tempMul = in1[lines*i + j] * in2[lines*i + j]; /* we times by the coefficient*/
        tempCoefSum +=in2[lines*i + j];
        out[j] +=tempMul ;
      } 

    out[j] /= tempCoefSum ;
  }
  
}

