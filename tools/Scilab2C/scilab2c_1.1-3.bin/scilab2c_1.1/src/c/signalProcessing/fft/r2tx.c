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

#include "fft_internal.h"
/*
** radix 2 iteration subroutine
*/
void r2tx(int nthpo, doubleComplex* c0, doubleComplex* c1)
{
  int kk;
 /* double *cr0, *ci0, *cr1, *ci1, r1, fi1;*/
 doubleComplex temp ;

/*  cr0 = &(c0[0].re);
  ci0 = &(c0[0].im);
  cr1 = &(c1[0].re);
  ci1 = &(c1[0].im);*/

  for(kk=0;kk<nthpo;kk+=2)
    {


    temp   = zadds ( c0[kk] , c1[kk] );
	c1[kk] = zdiffs( c0[kk] , c1[kk] );
	c0[kk] = DoubleComplex ( zreals ( temp ) , zimags( temp ));
/*
      r1 = cr0[kk] + cr1[kk];
      cr1[kk] = cr0[kk] - cr1[kk];
      cr0[kk] = r1;


      fi1 = ci0[kk] + ci1[kk];
      ci1[kk] = ci0[kk] - ci1[kk];
      ci0[kk] = fi1;
*/
    }
}
