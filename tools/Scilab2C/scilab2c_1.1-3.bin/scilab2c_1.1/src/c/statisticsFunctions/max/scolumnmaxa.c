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

#include "statMax.h"

void scolumnmaxa(float *in, int rows, int columns, float* out) {
  int i = 0, j = 0;

  for (i = 0; i < rows; i++) {
	  out[i]=in[i];
	  for (j=0;j<columns;j++)
      		if (in[i+j*rows]>out[i]) out[i] = in[i+j*rows];
    }

}
