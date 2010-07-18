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
#include "zeros.h"
#include <malloc.h>

floatComplex cmeanfcs (floatComplex* in1 ,int lines , int columns , float* in2)
  {
    float* ZEROS;
    ZEROS=malloc((unsigned int)(lines*columns*sizeof(float)));
    szerosa(ZEROS,lines,columns);
    
    return cmeanfa(in1, lines*columns , FloatComplexMatrix(in2,ZEROS,lines*columns));
  }
