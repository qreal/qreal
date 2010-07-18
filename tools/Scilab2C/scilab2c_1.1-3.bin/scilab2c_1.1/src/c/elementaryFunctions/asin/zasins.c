/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*
 *     REFERENCE
 *        This is a Fortran-77 translation of an algorithm by
 *        T.E. Hull, T. F. Fairgrieve and P.T.P. Tang which
 *        appears in their article :
 *          "Implementing the Complex Arcsine and Arccosine
 *           Functions Using Exception Handling", ACM, TOMS,
 *           Vol 23, No. 3, Sept 1997, p. 299-335
 */

#include "lapack.h"
#include "asin.h"
#include "atan.h"
#include "sqrt.h"
#include "abs.h"
#include "log.h"
#include "log1p.h"
#include "min.h"
#include "max.h"

doubleComplex		zasins(doubleComplex z) {
  static double sdblPi_2	= 1.5707963267948966192313216;
  static double sdblLn2		= 0.6931471805599453094172321;
  static double sdblAcross	= 1.5;
  static double sdblBcross	= 0.6417;

  double dblLsup = dsqrts(getOverflowThreshold())/8.0;
  double dblLinf = 4 * dsqrts(getUnderflowThreshold());
  double dblEpsm = dsqrts(getRelativeMachinePrecision());

  double _dblReal	= zreals(z);
  double _dblImg	= zimags(z);

  double dblAbsReal	= dabss(_dblReal);
  double dblAbsImg	= dabss(_dblImg);
  int iSignReal		= _dblReal < 0 ? -1 : 1;
  int iSignImg		= _dblImg < 0 ? -1 : 1;

  double dblR = 0, dblS = 0, dblA = 0, dblB = 0;

  double dblTemp = 0;

  double _pdblReal = 0;
  double _pdblImg = 0;

  if( min(dblAbsReal, dblAbsImg) > dblLinf && max(dblAbsReal, dblAbsImg) <= dblLsup)
    {
      /* we are in the safe region */
      dblR = dsqrts( (dblAbsReal + 1) * (dblAbsReal + 1) + dblAbsImg * dblAbsImg);
      dblS = dsqrts( (dblAbsReal - 1) * (dblAbsReal - 1) + dblAbsImg * dblAbsImg);
      dblA = 0.5 * ( dblR + dblS );
      dblB = dblAbsReal / dblA;


      /* compute the real part */
      if(dblB <= sdblBcross)
	_pdblReal = dasins(dblB);
      else if( dblAbsReal <= 1)
	_pdblReal = datans(dblAbsReal / dsqrts( 0.5 * (dblA + dblAbsReal) * ( (dblAbsImg * dblAbsImg) / (dblR + (dblAbsReal + 1)) + (dblS + (1 - dblAbsReal)))));
      else
	_pdblReal = datans(dblAbsReal / (dblAbsImg * dsqrts(0.5 * ((dblA + dblAbsReal) / (dblR + (dblAbsReal + 1)) + (dblA + dblAbsReal) / (dblS + (dblAbsReal-1))))));

      /* compute the imaginary part */
      if(dblA <= sdblAcross)
	{
	  double dblImg1 = 0;

	  if(dblAbsReal < 1)
	    /* Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(y**2)/(S+(1.d0-x))) */
	    dblImg1 = 0.5 * (dblAbsImg * dblAbsImg / (dblR + (dblAbsReal + 1)) + dblAbsImg * dblAbsImg / (dblS + (1 - dblAbsReal)));
	  else
	    /* Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(S+(x-1.d0))) */
	    dblImg1 = 0.5 * (dblAbsImg * dblAbsImg / (dblR + (dblAbsReal + 1)) + (dblS + (dblAbsReal - 1)));
	  /* ai = logp1(Am1 + sqrt(Am1*(A+1.d0))) */
	  dblTemp = dblImg1 + dsqrts(dblImg1 * (dblA + 1));
	  _pdblImg = dlog1ps(dblTemp);
	}
      else
	/* ai = log(A + sqrt(A**2 - 1.d0)) */
	_pdblImg = dlogs(dblA + dsqrts(dblA * dblA - 1));
    }
  else
    {
      /* evaluation in the special regions ... */
      if(dblAbsImg <= dblEpsm * dabss(dblAbsReal - 1))
	{
	  if(dblAbsReal < 1)
	    {
	      _pdblReal	= dasins(dblAbsReal);
	      _pdblImg	= dblAbsImg / dsqrts((1 + dblAbsReal) * (1 - dblAbsReal));
	    }
	  else
	    {
	      _pdblReal = sdblPi_2;
	      if(dblAbsReal <= dblLsup)
		{
		  dblTemp		= (dblAbsReal - 1) + dsqrts((dblAbsReal - 1) * (dblAbsReal + 1));
		  _pdblImg	= dlog1ps(dblTemp);
		}
	      else
		_pdblImg	= sdblLn2 + dlogs(dblAbsReal);
	    }
	}
      else if(dblAbsImg < dblLinf)
	{
	  _pdblReal	= sdblPi_2 - dsqrts(dblAbsImg);
	  _pdblImg	= dsqrts(dblAbsImg);
	}
      else if((dblEpsm * dblAbsImg - 1 >= dblAbsReal))
	{
	  _pdblReal	= dblAbsReal * dblAbsImg;
	  _pdblImg	= sdblLn2 + dlogs(dblAbsReal);
	}
      else if(dblAbsReal > 1)
	{
	  _pdblReal	= datans(dblAbsReal / dblAbsImg);
	  dblTemp		= (dblAbsReal / dblAbsImg) * (dblAbsReal / dblAbsImg);
	  _pdblImg	= sdblLn2 + dlogs(dblAbsReal) + 0.5 * dlog1ps(dblTemp);
	}
      else
	{
	  double dblTemp2 = dsqrts(1 + dblAbsImg * dblAbsImg);
	  _pdblReal	= dblAbsReal / dblTemp2;
	  dblTemp		= 2 * dblAbsImg * (dblAbsImg + dblTemp2);
	  _pdblImg	= 0.5 * dlog1ps(dblTemp);
	}
    }
  _pdblReal *= iSignReal;
  _pdblImg *= iSignImg;

  return (DoubleComplex(_pdblReal, _pdblImg));
}
