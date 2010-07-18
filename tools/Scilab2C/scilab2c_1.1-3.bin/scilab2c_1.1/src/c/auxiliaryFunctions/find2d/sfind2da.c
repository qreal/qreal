/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Arnaud TORSET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "find2d.h"
#include <malloc.h>

void sfind2da(float* x, int rows, int columns, float* out1,int* indiceOut1, float* out2,int* indiceOut2,int max) {
  int i = 0, j=0;
  indiceOut1[1] = 0;
  
  /* if out are empties */
  out1[0]=-1;
  out2[0]=-1;
  
  for	(j = 0; j < columns ; ++j){
  	 for (i = 0; i < rows ; ++i) {
            /*to avoid useless search if we only want to find the  max first founded value   */
            if (indiceOut1[1] == max  ) return ;
	    if (x[j*rows+i] != 0) {
	    	
		out1[indiceOut1[1]] = (float)(i+1);
		out2[indiceOut1[1]] = (float)(j+1);
		indiceOut1[1]++;
	    }
	}
  }
  indiceOut2[1]=indiceOut1[1];
}
