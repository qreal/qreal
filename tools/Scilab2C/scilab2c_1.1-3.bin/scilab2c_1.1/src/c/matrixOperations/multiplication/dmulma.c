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

#ifndef WITHOUT_BLAS
#include "lapack.h"
#endif
#include "matrixMultiplication.h"

/*
** \brief Compute a multiplication for doubles matrixes.
** \param in1 : input matrix.
** \param lines1 : lines of in1 matrix.
** \param columns1 : columns of in1 matrix.
** \param in2 : input arry.
** \param lines2 : lines of in2 matrix.
** \param columns2 : columns of in2 matrix.
** \param out : Matrix that contains the multiplication in1 * in2.
*/
void	dmulma(double *in1, int lines1, int columns1,
	       double *in2, int lines2, int columns2,
	       double *out)
{
#ifndef WITHOUT_BLAS
  /*
  ** USES BLAS DGEMM FUNCTION.
  */
  double One		= 1;
  double Zero		= 0;

  /* Cr <-  1*Ar*Br + 0*Cr */
  dgemm_("N","N", &lines1, &columns2, &columns1, &One,
	 in1 , &lines1, in2, &lines2, &Zero, out, &lines1);
#else
  /*
  ** DO NOT USE ANY BLAS FUNCTION.
  */
  int i = 0;
  int k = 0;
  double accu = 0;

  /*
  **  How to convert 2 index matrixes to one.
  ** #define in1(a, b)	in1[a+b*lines1]
  ** #define in2(c, d)	in2[c+d*lines2]
  */

  for (i = 0 ; i < lines1 * columns2 ; ++i)
    {
      accu = 0;
      for (k = 0; k < columns1 ; ++k)
	{
	  accu += in1[i % lines1 + k * lines1]
	    * in2[k + (i / lines1) * lines2];
	}
      out[i] = accu;
    }
#endif
}
