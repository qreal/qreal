/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "find.h"

void dfinda(double* x, int size ,double *out, int *indiceOut , int max ) {
  int i = 0;
  indiceOut[1]=0;
  
  /* if out is empty */
  out[0]=-1;

  
  for (i = 0; i < size ; ++i) {
    /*to avoid useless search if we only want to find the  max first founded value   */
    if (indiceOut[1] == max  ) return ;
    if (x[i] != 0) {
	out[indiceOut[1]] = (double)(i+1);
	indiceOut[1]++;
    }
  }
}

