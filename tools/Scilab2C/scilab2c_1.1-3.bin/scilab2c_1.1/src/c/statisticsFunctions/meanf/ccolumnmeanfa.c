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
#include "multiplication.h"

void ccolumnmeanfa(floatComplex *in1, int lines, int columns,floatComplex *in2, floatComplex *out) {
  int i = 0;
  int j = 0;
  floatComplex tempCoefSum = FloatComplex(0.0f,0.0f);
  floatComplex tempMul = FloatComplex(0.0f,0.0f);
  /*we first multiply each cell of the input matrix by its coefficient*/
  for (j = 0; j < lines ; ++j)
  {
    tempCoefSum = FloatComplex(0.0f,0.0f);
    out[j]=  FloatComplex(0.0f,0.0f);
    for ( i = 0 ; i < columns; ++i )
      {

        tempMul = cmuls ( in1[lines*i + j] , in2[lines*i + j]); /* we times by the coefficient*/
        tempCoefSum = cadds ( in2[lines*i + j] ,tempCoefSum ) ;
        out[j] = cadds (tempMul, out[j]) ;
      } 

    out[j] = crdivs(out[j] ,tempCoefSum);
  }
}


