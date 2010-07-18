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

#include "statMin.h"

void drowmina(double *in, int rows, int columns, double* out) {
  int i = 0, j = 0;

  for (i = 0; i < columns; i++) {
	  out[i]=in[i*rows];
	  for (j = 1 ; j < rows ; j++)
     		 if (in[i*rows+j]<out[i]) out[i] = in[i*rows+j];
    }
}
