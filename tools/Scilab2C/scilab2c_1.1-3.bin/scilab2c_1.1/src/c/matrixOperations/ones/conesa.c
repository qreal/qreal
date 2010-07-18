/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ones.h"

void conesa ( floatComplex* in , int rows ,int cols )
{
    int i = 0 ;
    
    for ( i = 0 ; i < rows*cols ; i++)
    {
        in[i] = FloatComplex ( 1.0f , 0);
        
    }
    
}
