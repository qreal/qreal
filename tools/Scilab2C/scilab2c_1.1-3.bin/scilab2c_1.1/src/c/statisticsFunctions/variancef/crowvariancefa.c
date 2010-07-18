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

#include "variancef.h"

void crowvariancefa(floatComplex *in1, int lines, int columns,floatComplex *in2, floatComplex* out){
    
    int i = 0 ;
    
    for ( i = 0; i < columns ; i++)
    out[i] = cvariancefa ( in1 + i*lines , lines , in2) ;
}

