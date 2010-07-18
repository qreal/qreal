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

#include "matrixMultiplication.h"

/*
** \brief Compute a multiplication for floats complex matrixes.
** \param in1 : input matrix.
** \param lines1 : lines of in1 matrix.
** \param columns1 : columns of in1 matrix.
** \param in2 : input arry.
** \param lines2 : lines of in2 matrix.
** \param columns2 : columns of in2 matrix.
** \param out : Matrix that contains the multiplication in1 * in2.
*/
void	cmulma(floatComplex *in1, int lines1, int columns1,
	       floatComplex *in2, int lines2, int columns2,
	       floatComplex *out)
{
  int i = 0;
  int k = 0;
  floatComplex accu = FloatComplex(0, 0);

  for (i = 0 ; i < lines1 * columns2 ; ++i)
    {
      accu = FloatComplex(0,0);
      for (k = 0; k < columns1 ; ++k)
	{
	  accu = cadds(accu,
		cmuls(in1[i % lines1 + k *lines1] ,
			in2[k + (i / lines1) *lines2] ));
	}
      out[i] = accu;
    }
}
