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

void srowsuma(float *in, int lines, int columns, float * out) {
  int i = 0;

  /*
  ** First assign first row, just in case
  ** out contains non-zero's elements.
  */
  for (i = 0 ; i < columns; ++i)
    {
      out[i] = in[i * lines];
    }
  /*
  ** Then accumulate in each column.
  */
  for (i = 1 ; i <= (lines - 1) * columns ; ++i)
    {
      out[(i - 1) % columns] += in[((i - 1) % columns) * lines + 1 + (i - 1) / columns];
    }
}

