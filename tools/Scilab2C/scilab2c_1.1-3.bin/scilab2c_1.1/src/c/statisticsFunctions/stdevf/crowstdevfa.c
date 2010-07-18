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


#include "stdevf.h"
#include "meanf.h"
#include "abs.h"


void crowstdevfa(floatComplex *in1, int lines, int columns, floatComplex*in2, floatComplex* out){
  int i = 0;
  int j = 0;
  floatComplex temp = FloatComplex(0.0f,0.0f);
  floatComplex accumulate = FloatComplex(0.0f,0.0f);
  floatComplex accumulateFre = FloatComplex(0.0f,0.0f);
    
  crowmeanfa(in1, lines, columns, in2, out );

  /*we first multiply each cell of the input matrix by its coefficient*/
  for (j = 0; j < columns; ++j)
    {
      accumulate = FloatComplex(0.0f,0.0f);
      accumulateFre =FloatComplex(0.0f,0.0f);
      temp = FloatComplex(0.0f,0.0f);

      for ( i = 0 ; i < lines; ++i )
        {

         temp = cpows (  cdiffs (in1[lines*j + i] ,out[j] ) ,FloatComplex (2.0f, 0.0f ) );
         temp = cmuls( in2[lines*j + i] , temp);

         accumulate = cadds( temp , accumulate);
         accumulateFre =  cadds (in2[lines*j + i] ,accumulateFre );

        } 

      if (lines <= 1)
        {
          out[j] = cmuls (FloatComplex(0.0f,0.0f) , accumulate ) ; 
        }
      else
        {
	  if( sabss (creals(accumulate)) <= 3e-6 ) accumulate = FloatComplex(sabss(creals(accumulate)) ,cimags(accumulate));
	  if( sabss (cimags(accumulate)) <= 3e-6 ) accumulate = FloatComplex(creals(accumulate) ,sabss(cimags(accumulate)));
          accumulate =   crdivs (accumulate , cdiffs (accumulateFre ,FloatComplex(1.0f,0.0f))   );
          out[j] =csqrts(accumulate);
        }
    }

}
