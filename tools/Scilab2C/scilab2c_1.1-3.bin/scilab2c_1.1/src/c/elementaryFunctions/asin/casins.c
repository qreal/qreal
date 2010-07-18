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

floatComplex		casins(floatComplex z) {
  static float sdblPi_2		= 1.5707963267948966192313216f;
  static float sdblLn2		= 0.6931471805599453094172321f;
  static float sdblAcross	= 1.5f;
  static float sdblBcross	= 0.6417f;

  float dblLsup = ssqrts((float) getOverflowThreshold())/ 8.0f;
  float dblLinf = 4.0f * ssqrts((float) getUnderflowThreshold());
  float dblEpsm = ssqrts((float) getRelativeMachinePrecision());

  float _dblReal	= creals(z);
  float _dblImg		= cimags(z);

  float dblAbsReal	= sabss(_dblReal);
  float dblAbsImg	= sabss(_dblImg);
  float iSignReal	= _dblReal < 0 ? -1.0f : 1.0f;
  float iSignImg	= _dblImg < 0 ? -1.0f : 1.0f;

  float dblR = 0, dblS = 0, dblA = 0, dblB = 0;

  float dblTemp = 0;

  float _pdblReal = 0;
  float _pdblImg = 0;

  if( min(dblAbsReal, dblAbsImg) > dblLinf && max(dblAbsReal, dblAbsImg) <= dblLsup)
    {
      /* we are in the safe region */
      dblR = ssqrts( (dblAbsReal + 1) * (dblAbsReal + 1) + dblAbsImg * dblAbsImg);
      dblS = ssqrts( (dblAbsReal - 1) * (dblAbsReal - 1) + dblAbsImg * dblAbsImg);
      dblA = (float) 0.5 * ( dblR + dblS );
      dblB = dblAbsReal / dblA;


      /* compute the real part */
      if(dblB <= sdblBcross)
	_pdblReal = sasins(dblB);
      else if( dblAbsReal <= 1)
	_pdblReal = satans(dblAbsReal / ssqrts( 0.5f * (dblA + dblAbsReal) * ( (dblAbsImg * dblAbsImg) / (dblR + (dblAbsReal + 1)) + (dblS + (1 - dblAbsReal)))));
      else
	_pdblReal = satans(dblAbsReal / (dblAbsImg * ssqrts( 0.5f * ((dblA + dblAbsReal) / (dblR + (dblAbsReal + 1)) + (dblA + dblAbsReal) / (dblS + (dblAbsReal-1))))));

      /* compute the imaginary part */
      if(dblA <= sdblAcross)
	{
	  float dblImg1 = 0;

	  if(dblAbsReal < 1)
	    /* Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(y**2)/(S+(1.d0-x))) */
	    dblImg1 = 0.5f * (dblAbsImg * dblAbsImg / (dblR + (dblAbsReal + 1)) + dblAbsImg * dblAbsImg / (dblS + (1 - dblAbsReal)));
	  else
	    /* Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(S+(x-1.d0))) */
	    dblImg1 = 0.5f * (dblAbsImg * dblAbsImg / (dblR + (dblAbsReal + 1)) + (dblS + (dblAbsReal - 1)));
	  /* ai = logp1(Am1 + sqrt(Am1*(A+1.d0))) */
	  dblTemp = dblImg1 + ssqrts(dblImg1 * (dblA + 1));
	  _pdblImg = slog1ps(dblTemp);
	}
      else
	/* ai = log(A + sqrt(A**2 - 1.d0)) */
	_pdblImg = slogs(dblA + ssqrts(dblA * dblA - (float) 1.0));
    }
  else
    {
      /* evaluation in the special regions ... */
      if(dblAbsImg <= dblEpsm * dabss(dblAbsReal - 1))
	{
	  if(dblAbsReal < 1)
	    {
	      _pdblReal	= sasins(dblAbsReal);
	      _pdblImg	= dblAbsImg / ssqrts((1 + dblAbsReal) * (1 - dblAbsReal));
	    }
	  else
	    {
	      _pdblReal = sdblPi_2;
	      if(dblAbsReal <= dblLsup)
		{
		  dblTemp		= (dblAbsReal - 1) + ssqrts((dblAbsReal - 1) * (dblAbsReal + 1));
		  _pdblImg	= slog1ps(dblTemp);
		}
	      else
		_pdblImg	= sdblLn2 + slogs(dblAbsReal);
	    }
	}
      else if(dblAbsImg < dblLinf)
	{
	  _pdblReal	= sdblPi_2 - ssqrts(dblAbsImg);
	  _pdblImg	= ssqrts(dblAbsImg);
	}
      else if((dblEpsm * dblAbsImg - 1 >= dblAbsReal))
	{
	  _pdblReal	= dblAbsReal * dblAbsImg;
	  _pdblImg	= sdblLn2 + slogs(dblAbsReal);
	}
      else if(dblAbsReal > 1)
	{
	  _pdblReal	= satans(dblAbsReal / dblAbsImg);
	  dblTemp	= (dblAbsReal / dblAbsImg) * (dblAbsReal / dblAbsImg);
	  _pdblImg	= sdblLn2 + slogs(dblAbsReal) + 0.5f * slog1ps(dblTemp);
	}
      else
	{
	  float dblTemp2 = ssqrts(1 + dblAbsImg * dblAbsImg);
	  _pdblReal	= dblAbsReal / dblTemp2;
	  dblTemp	= 2.0f * dblAbsImg * (dblAbsImg + dblTemp2);
	  _pdblImg	= 0.5f * slog1ps(dblTemp);
	}
    }
  _pdblReal *= iSignReal;
  _pdblImg *= iSignImg;

  return (FloatComplex(_pdblReal, _pdblImg));
}
