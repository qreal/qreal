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


#include "matrixDivision.h"

floatComplex crdivv(floatComplex *in1, floatComplex *in2, int size){

  floatComplex out[1] ={ FloatComplex (0.0f , 0.0f)};
  crdivma ( in1,1 ,size ,in2 , 1 , size , out );

 return out[0] ;
}


