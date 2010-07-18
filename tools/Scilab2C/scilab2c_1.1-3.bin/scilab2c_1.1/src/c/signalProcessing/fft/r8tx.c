
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
#include <math.h>



/*
** radix 8 iteration subroutine
*/

/* this function do in one turn the same computation that do radix 2 in three turns  */

void r8tx ( int nxtlt,int nthpo,int lengt,
            doubleComplex* cc0,doubleComplex* cc1,doubleComplex* cc2,doubleComplex* cc3,
            doubleComplex* cc4,doubleComplex* cc5,doubleComplex* cc6,doubleComplex* cc7)

{


  int j , kk;
  double dblP7  = 1 / sqrt(2.0) ;
  double dblPi2 = 8 * atan(1.0);

  double scale, arg;
  double c1,c2,c3,c4,c5,c6,c7;
  double s1,s2,s3,s4,s5,s6,s7;

  doubleComplex Atemp0,Atemp1,Atemp2,Atemp3,Atemp4,Atemp5,Atemp6,Atemp7;
  doubleComplex Btemp0,Btemp1,Btemp2,Btemp3,Btemp4,Btemp5,Btemp6,Btemp7;

  doubleComplex temp ;


  scale = dblPi2/lengt;


  for(j=0;j<nxtlt;j++)
    {

      arg = j*scale;
      c1 = cos(arg);
      s1 = sin(arg);
      c2 = c1*c1 - s1*s1;
      s2 = c1*s1 + c1*s1;
      c3 = c1*c2 - s1*s2;
      s3 = c2*s1 + s2*c1;
      c4 = c2*c2 - s2*s2;
      s4 = c2*s2 + c2*s2;
      c5 = c2*c3 - s2*s3;
      s5 = c3*s2 + s3*c2;
      c6 = c3*c3 - s3*s3;
      s6 = c3*s3 + c3*s3;
      c7 = c3*c4 - s3*s4;
      s7 = c4*s3 + s4*c3;

      for(kk=j;kk<nthpo;kk+=lengt)
	{
	   /* (k-1)*2*/ /* index by twos; re & im alternate */


       /*  first turn the same as calling radix 2 with the input vector */
       /* but radix2 will have do it in three turn , radix8 do it in one */
		Atemp0 =  zadds ( cc0[kk] , cc4[kk] ) ;
		Atemp1 =  zadds ( cc1[kk] , cc5[kk] ) ;
		Atemp2 =  zadds ( cc2[kk] , cc6[kk] ) ;
		Atemp3 =  zadds ( cc3[kk] , cc7[kk] ) ;


		Atemp4 =  zdiffs ( cc0[kk] , cc4[kk] ) ;
		Atemp5 =  zdiffs ( cc1[kk] , cc5[kk] ) ;
		Atemp6 =  zdiffs ( cc2[kk] , cc6[kk] ) ;
		Atemp7 =  zdiffs ( cc3[kk] , cc7[kk] ) ;

        /* second turn the same as  calling radix 2 with the vector transformed by a previous call of radix2 */
        /* the same here , three turns in one */
		Btemp0 =  zadds  ( Atemp0 , Atemp2 ) ;
		Btemp1 =  zadds  ( Atemp1 , Atemp3 ) ;
		Btemp2 =  zdiffs ( Atemp0 , Atemp2 ) ;
		Btemp3 =  zdiffs ( Atemp1 , Atemp3 ) ;

        Btemp4 = DoubleComplex ( zreals ( Atemp4 ) - zimags( Atemp6 ) , zimags ( Atemp4 ) + zreals( Atemp6 ) );
        Btemp5 = DoubleComplex ( zreals ( Atemp5 ) - zimags( Atemp7 ) , zimags ( Atemp5 ) + zreals( Atemp7 ) );
        Btemp6 = DoubleComplex ( zreals ( Atemp4 ) + zimags( Atemp6 ) , zimags ( Atemp4 ) - zreals( Atemp6 ) );
        Btemp7 = DoubleComplex ( zreals ( Atemp5 ) + zimags( Atemp7 ) , zimags ( Atemp5 ) - zreals( Atemp7 ) );

        /*third turn the same as calling radix 2 with the vector transformed by two previous call of radix2 */
	    cc0[kk] = zadds ( Btemp0 , Btemp1 );



        /* if we are not in the first turn */

	  if(j>0)
	    {
		cc1[kk] = DoubleComplex ( 	(c4 * (zreals(Btemp0) - zreals(Btemp1))) - (s4 * (zimags(Btemp0) - zimags(Btemp1))),
                                    c4 * (zimags(Btemp0) - zimags(Btemp1)) + s4 * (zreals(Btemp0) - zreals(Btemp1)));

		cc2[kk] = DoubleComplex ( 	c2 *  (zreals(Btemp2) - zimags(Btemp3)) - s2 * (zimags(Btemp2) + zreals(Btemp3)) ,
                                    c2 *  (zimags(Btemp2) + zreals(Btemp3)) + s2 * (zreals(Btemp2) - zimags(Btemp3)));

		cc3[kk] = DoubleComplex ( 	c6 * (zreals(Btemp2)  + zimags(Btemp3)) - s6 * (zimags(Btemp2) - zreals(Btemp3)) ,
                                    c6 * (zimags(Btemp2)  - zreals(Btemp3)) + s6 * (zreals(Btemp2) + zimags(Btemp3)));


            temp = DoubleComplex ( 	dblP7*(zreals ( Btemp5 ) - zimags( Btemp5 )) ,
                                    dblP7*(zreals ( Btemp5 ) + zimags( Btemp5 )) );

		cc4[kk] = DoubleComplex ( 	c1 * (zreals (Btemp4) + zreals(temp)) - s1 * (zimags (Btemp4) + zimags(temp)) ,
                                    c1 * (zimags (Btemp4) + zimags(temp)) + s1 * (zreals (Btemp4) + zreals(temp)));
		cc5[kk] = DoubleComplex ( 	c5 * (zreals (Btemp4) - zreals(temp)) - s5 * (zimags (Btemp4) - zimags(temp)) ,
                                    c5 * (zimags (Btemp4) - zimags(temp)) + s5 * (zreals (Btemp4) - zreals(temp)));


            temp = DoubleComplex ( - dblP7*(zreals ( Btemp7 ) + zimags( Btemp7 )) ,
                                     dblP7*(zreals ( Btemp7 ) - zimags( Btemp7 )) );

		cc6[kk] = DoubleComplex ( 	c3 * (zreals (Btemp6) + zreals(temp)) - s3 * (zimags (Btemp6) + zimags(temp)) ,
                                    c3 * (zimags (Btemp6) + zimags(temp)) + s3 * (zreals (Btemp6) + zreals(temp)));
		cc7[kk] = DoubleComplex ( 	c7 * (zreals (Btemp6) - zreals(temp)) - s7 * (zimags (Btemp6) - zimags(temp)) ,
                                    c7 * (zimags (Btemp6) - zimags(temp)) + s7 * (zreals (Btemp6) - zreals(temp)));




	    }
    else
        {
            cc1[kk] = zdiffs ( Btemp0 , Btemp1 );

            cc2[kk] = DoubleComplex ( zreals ( Btemp2 ) - zimags( Btemp3 ) ,
                                      zimags ( Btemp2 ) + zreals( Btemp3 ) );


            cc3[kk] = DoubleComplex ( zreals ( Btemp2 ) + zimags( Btemp3 ) ,
                                      zimags ( Btemp2 ) - zreals( Btemp3 ) );


            temp = DoubleComplex ( 	dblP7*(zreals ( Btemp5 ) - zimags( Btemp5 )) ,
                                    dblP7*(zreals ( Btemp5 ) + zimags( Btemp5 )) );

            cc4[kk] = zadds  ( Btemp4 , temp );
            cc5[kk] = zdiffs ( Btemp4 , temp );

            temp = DoubleComplex ( - dblP7*(zreals ( Btemp7 ) + zimags( Btemp7 )) ,
                                     dblP7*(zreals ( Btemp7 ) - zimags( Btemp7 )) );

            cc6[kk] = zadds  ( Btemp6 , temp );
            cc7[kk] = zdiffs ( Btemp6 , temp );


        }

	}
    }


}
