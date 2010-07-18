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
#include <stdlib.h>
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
void	zmulma(doubleComplex *in1, int lines1, int columns1,
	       doubleComplex *in2, int lines2, int columns2,
	       doubleComplex *out)
{
#ifndef WITHOUT_BLAS
  /*
  ** USES BLAS DGEMM FUNCTION.
  */
  int i = 0;
  double One		= 1;
  double MinusOne	= -1;
  double Zero		= 0;

  double *in1Real = (double*)malloc((unsigned int) lines1 * (unsigned int) columns1 * sizeof(double));
  double *in1Imag = (double*)malloc((unsigned int) lines1 * (unsigned int) columns1 * sizeof(double));
  double *in2Real = (double*)malloc((unsigned int) lines2 * (unsigned int) columns2 * sizeof(double));
  double *in2Imag = (double*)malloc((unsigned int) lines2 * (unsigned int) columns2 * sizeof(double));

  double *RealOut = (double*)malloc((unsigned int) lines1 * (unsigned int) columns2 * sizeof(double));
  double *ImagOut = (double*)malloc((unsigned int) lines1 * (unsigned int) columns2 * sizeof(double));

  zreala(in1, lines1 * columns1, in1Real);
  zreala(in2, lines2 * columns2, in2Real);
  zimaga(in1, lines1 * columns1, in1Imag);
  zimaga(in2, lines2 * columns2, in2Imag);

  /* Cr <-  1*Ar*Br + 0*Cr */
  dgemm_("N","N", &lines1, &columns2, &columns1, &One,
	 in1Real, &lines1, in2Real, &lines2, &Zero, RealOut, &lines1);
  

  /* Cr <- -1*Ai*Bi + 1*Cr */
  dgemm_("N","N", &lines1, &columns2, &columns1, &MinusOne,
	 in1Imag, &lines1, in2Imag, &lines2, &One, RealOut, &lines1);

  /* Ci <-  1*Ar*Bi + 0*Ci */
  dgemm_("N","N", &lines1, &columns2, &columns1, &One,
	 in1Real, &lines1, in2Imag, &lines2, &Zero, ImagOut, &lines1);

  /* Ci <-  1*Ai*Br + 1*Ci */
  dgemm_("N","N", &lines1, &columns2, &columns1, &One,
	 in1Imag, &lines1, in2Real, &lines2, &One, ImagOut, &lines1);

  /* Now fill output matrix */
  for(i = 0 ; i < lines1 * columns2 ; ++i)
    {
      out[i] = DoubleComplex(RealOut[i], ImagOut[i]);
    }
  /* FREE allocated variguru_dim_structables */
  free(in1Real);
  free(in2Real);
  free(in1Imag);
  free(in2Imag);
  free(RealOut);
  free(ImagOut);
#else
  /*
  ** DO NOT USE ANY BLAS FUNCTION.
  */
  int i = 0;
  int k = 0;
  doubleComplex accu = DoubleComplex(0, 0);

  for (i = 0 ; i < lines1 * columns2 ; ++i)
    {
      accu = DoubleComplex(0,0);
      for (k = 0; k < columns1 ; ++k)
	{
	  accu = zadds(accu,
		zmuls(in1[i % lines1 + k *lines1] ,
			in2[k + (i / lines1) *lines2] ));
	}
      out[i] = accu;
    }
#endif
}
