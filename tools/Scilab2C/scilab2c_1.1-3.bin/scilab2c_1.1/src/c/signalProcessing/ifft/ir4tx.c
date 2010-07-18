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

#include "ifft_internal.h"



/*
** radix 4 iteration subroutine
*/
/* this function do in one turn the same computation that do radix 2 in two turns  */
void ir4tx( int nthpo, doubleComplex* c0, doubleComplex* c1, doubleComplex* c2, doubleComplex* c3)
{
  int kk;
  doubleComplex temp1 , temp2 , temp3 , temp4 ;

  for(kk=0;kk<nthpo;kk+=4)
    {
       /* real and imag parts alternate */

    /* this first step is strictly equivalent than calling radix 2
       except that radix would have needed 2 turns to compute what radix4 do in one */
	temp1 = zadds ( c0[kk] , c2[kk] ) ;
	temp2 = zdiffs( c0[kk] , c2[kk] ) ;
	temp3 = zadds ( c1[kk] , c3[kk] ) ;
	temp4 = zdiffs( c1[kk] , c3[kk] ) ;


    /* strictly equivalent than calling radix2 with the temporary vector , but here also , radix4 do it in one turn
    instead of two */
	c0[kk] = zadds ( temp1 , temp3 );
	c1[kk] = zdiffs( temp1 , temp3 );


	c2[kk] = DoubleComplex ( zreals ( temp2 ) - zimags( temp4 ) , zimags ( temp2 ) + zreals( temp4 ) );
	c3[kk] = DoubleComplex ( zreals ( temp2 ) + zimags( temp4 ) , zimags ( temp2 ) - zreals( temp4 ) );


    }
}
