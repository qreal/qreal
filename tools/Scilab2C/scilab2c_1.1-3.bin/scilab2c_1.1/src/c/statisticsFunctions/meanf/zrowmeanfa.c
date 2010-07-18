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
#include "sum.h"
#include "multiplication.h"
void zrowmeanfa(doubleComplex *in1, int lines, int columns, doubleComplex *in2, doubleComplex *out) {
  int i = 0;
  int j = 0;
  doubleComplex tempCoefSum = DoubleComplex(0.0,0.0);
  doubleComplex tempMul = DoubleComplex(0.0,0.0);
  /*we first multiply each cell of the input matrix by its coefficient*/
  for (j = 0; j < columns; ++j)
  {
    tempCoefSum =  DoubleComplex(0.0,0.0);
    out[j]=  DoubleComplex(0.0,0.0);
    for ( i = 0 ; i < lines; ++i )
      {

        tempMul = zmuls ( in1[lines*j + i] , in2[lines*j + i]); /* we times by the coefficient*/
        tempCoefSum = zadds ( in2[lines*j + i] ,tempCoefSum ) ;
        out[j] = zadds (tempMul, out[j]) ;
      } 

    out[j] = zrdivs(out[j] ,tempCoefSum) ;
  }
}
