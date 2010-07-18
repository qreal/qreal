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



/* get binary log of integer argument; exact if n a power of 2 */
static int fastlog2( int n)
{
  int log = -1;
  while(n) {
    log++;
    n >>= 1;
  }
  return(log);
}



void fft842 (doubleComplex* b, int size , int in)
{
  double fn;
  doubleComplex temp ;

  int L[16],L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11,L12,L13,L14,L15;
  int j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,j11,j12,j13,j14;
  int i = 0, j, ij, ji, ij1, ji1;
  int  n2pow, n8pow, nthpo, ipass, nxtlt, lengt;

  n2pow = fastlog2( size );
  nthpo = size ;
  fn = 1.0 / (double)nthpo;


  if(in==FORWARD)
    /* take conjugate  */
    for(i=0;i< size ;i++)
        {
        b[i] = DoubleComplex ( zreals( b[i]) , - zimags (b[i]));
        }



  n8pow = n2pow/3;

  if(n8pow)
    {
      /* if the size if a factor of a power of 8 we call r8tx */
      /* radix 8 iterations */
      for(ipass=1;ipass<=n8pow;ipass++)
	{
	  nxtlt = 0x1 << (n2pow - 3*ipass);
	  lengt = 8*nxtlt;



	  	  r8tx(nxtlt,nthpo,lengt,
	       b,b+nxtlt,b+2*nxtlt,
	       b+3*nxtlt,b+4*nxtlt,b+5*nxtlt,
	       b+6*nxtlt,b+7*nxtlt);


	}
    }

/* if the size can be written this way 2^(3*n + 1) , then we call the  radix 2 function
   if can be written this way 2^(3*n + 1) the we call the radix 4 function */

  if(n2pow%3 == 1)
    {
      /* radix 2 iteration needed */
	r2tx(nthpo,b,b+1);
    }

  if(n2pow%3 == 2)
    {
      /* radix 4 iteration needed */

      r4tx(nthpo,b,b+1,b+2,b+3);
    }



  for(j=1;j<=15;j++)
    {
      L[j] = 1;
      if(j-n2pow <= 0) L[j] = 0x1 << (n2pow + 1 - j);
    }
  /* this part can maybe be improved */

  L15=L[1];L14=L[2];L13=L[3];L12=L[4];L11=L[5];L10=L[6];L9=L[7];
  L8=L[8];L7=L[9];L6=L[10];L5=L[11];L4=L[12];L3=L[13];L2=L[14];L1=L[15];

  ij = 1;

/* all the following instruction is to unscramble the output */
  for(j1=1;j1<=L1;j1++)
  for(j2=j1;j2<=L2;j2+=L1)
  for(j3=j2;j3<=L3;j3+=L2)
  for(j4=j3;j4<=L4;j4+=L3)
  for(j5=j4;j5<=L5;j5+=L4)
  for(j6=j5;j6<=L6;j6+=L5)
  for(j7=j6;j7<=L7;j7+=L6)
  for(j8=j7;j8<=L8;j8+=L7)
  for(j9=j8;j9<=L9;j9+=L8)
  for(j10=j9;j10<=L10;j10+=L9)
  for(j11=j10;j11<=L11;j11+=L10)
  for(j12=j11;j12<=L12;j12+=L11)
  for(j13=j12;j13<=L13;j13+=L12)
  for(j14=j13;j14<=L14;j14+=L13)
  for(ji=j14;ji<=L15;ji+=L14)

    {
      ij1 = ij-1;
      ji1 = ji-1;

      if(ij-ji<0)
	{
	  temp   = b[ij1];
	  b[ij1] = b[ji1];
	  b[ji1] = temp;

	/*
	  r = b[ij1].re;
	  b[ij1].re = b[ji1].re;
	  b[ji1].re = r;
	  fi = b[ij1].im;
	  b[ij1].im = b[ji1].im;
	  b[ji1].im = fi;
	*/
	}
      ij++;
    }

  if(in==FORWARD)  /* take conjugates & unscramble outputs */
    for(i=0,j=size/2;j<size;i++,j++)
	{
	temp = DoubleComplex ( zreals ( b[j] ) ,- zimags( b[j] ));
	b[j] = DoubleComplex ( zreals ( b[i] ) ,- zimags( b[i] ));
	b[i] = DoubleComplex ( zreals ( temp ) , zimags( temp ));


	/*  r = b[j].re; fi = b[j].im;
	  b[j].re = b[i].re; b[j].im = -b[i].im;
	  b[i].re = r; b[i].im = -fi;
	*/
	}



/* this code is two fix a problem of result order which was different from what scilab  give */
       for ( i = 0 ; i < size /2 ; i++)
        {
          temp =  b[i] ;
          b[i] =  b[i+(size/2)];
          b[i+(size/2)]= temp ;



        }


}
