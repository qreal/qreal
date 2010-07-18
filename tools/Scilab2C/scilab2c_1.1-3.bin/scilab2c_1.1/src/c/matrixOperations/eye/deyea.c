/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "eye.h"

void deyea(double *in, int _iRows, int _iCols)
{
    int i, j  ;

    for ( i = 0 ; i < _iCols ; i++ )
        for ( j = 0 ; j < _iRows ; j++ )
                in[i*_iRows + j] =  (i == j)?1:0  ;
}

