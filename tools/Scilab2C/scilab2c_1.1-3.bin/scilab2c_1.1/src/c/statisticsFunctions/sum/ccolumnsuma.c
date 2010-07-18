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

#include "sum.h"

void ccolumnsuma(floatComplex *in, int lines, int columns, floatComplex * out) {
  int i = 0;

  /*
  ** First assign first row, just in case
  ** out contains non-zero's elements.
  */
  for (i = 0 ; i < lines; ++i)
    {
      out[i] = in[i];
    }
  /*
  ** Then accumulate in each row.
  */
  for (i = lines ; i < lines * columns ; ++i)
    {
      out[i % lines] = cadds(out[i % lines] , in[i]);
    }
}
