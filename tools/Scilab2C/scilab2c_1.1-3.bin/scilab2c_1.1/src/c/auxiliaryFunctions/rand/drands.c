/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA - Arnaud TORSET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
 
/*
   URAND, A UNIVERSAL RANDOM NUMBER GENERATOR
   BY, MICHAEL A. MALCOLM, CLEVE B. MOLER,
   STAN-CS-73-334, JANUARY 1973,
   COMPUTER SCIENCE  DEPARTMENT,
   School of Humanities and Sciences, STANFORD UNIVERSITY,
   ftp://reports.stanford.edu/pub/cstr/reports/cs/tr/73/334/CS-TR-73-334.pdf

*/



#include "rand.h"
#include <stdio.h>

double	drands(void) {
	int m=1;
	const int itwo=2;
	static int m2=0,halfm=0,ia=0, ic=0, mic=0, iy=0;
	static double s=0.0;
  
      if (m2==0){
	/*  if first entry, compute machine integer word length */
		while (m>m2){
		m2=m;
		m=itwo*m2;
		}
		halfm = m2;
	
		/* compute multiplier and increment for linear congruential method */
		ia = 8*(int)(halfm*atan(1.0)/8.0) + 5;
		ic = 2*(int)(halfm*(0.5-sqrt(3.0)/6.0)) + 1;
		mic = (m2 - ic) + m2;
	
		/* s is the scale factor for converting to floating point */
		s = 0.5/halfm;
	}
	
	/* compute next random number	*/
	iy = iy*ia;

	/* the following statement is for computers which do not allow
	   integer overflow on addition  */
      if (iy > mic) iy = (iy - m2) - m2;

      iy = iy + ic;

	/* the following statement is for computers where the
	   word length for addition is greater than for multiplication */
      if (iy/2 > m2) iy = (iy - m2) - m2;

	/* the following statement is for computers where integer
	   overflow affects the sign bit */
      if (iy < 0) iy = (iy + m2) + m2;
      return (double)iy*s;
}


