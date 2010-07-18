/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2009 - INRIA - Allan SIMON
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include <malloc.h>
#include "zeros.h"
#include "matrixDivision.h"

doubleComplex zrdivzdv (doubleComplex* in1,double* in2,  int size)
  {
    double* ZEROS;
    ZEROS=malloc((unsigned int)(size*sizeof(double)));
    dzerosa(ZEROS,size  , 1);
    
    return zrdivv(in1 , DoubleComplexMatrix(in2,ZEROS,size),size );
  }
