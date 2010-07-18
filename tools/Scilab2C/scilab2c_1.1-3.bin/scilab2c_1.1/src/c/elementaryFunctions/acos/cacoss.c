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
 * This fonction is a translation of fortran wacos write by Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
 *     REFERENCE
 *        This is a Fortran-77 translation of an algorithm by
 *        T.E. Hull, T. F. Fairgrieve and P.T.P. Tang which
 *        appears in their article :
 *          "Implementing the Complex Arcsine and Arccosine
 *           Functions Using Exception Handling", ACM, TOMS,
 *           Vol 23, No. 3, Sept 1997, p. 299-335
 */

#include "acos.h"
#include "atan.h"
#include "log.h"
#include "log1p.h"
#include "sqrt.h"
#include "abs.h"
#include "lapack.h"
#include "min.h"
#include "max.h"

#define localSign(x) (x>0 ? 1.0f : -1.0f)

floatComplex		cacoss(floatComplex z) {
	static float sfltPi		= 3.1415926535897932384626433f;
	static float sfltPi_2		= 1.5707963267948966192313216f;
	static float sfltLn2		= 0.6931471805599453094172321f;
	static float sfltAcross	= 1.5f;
	static float sfltBcross	= 0.6417f;

	float fltLsup = ssqrts((float) getOverflowThreshold())/8.0f;
	float fltLinf = 4.0f * ssqrts((float) getUnderflowThreshold());
	float fltEpsm = ssqrts((float) getRelativeMachinePrecision());

	float fltAbsReal	= sabss(creals(z));
	float fltAbsImg		= sabss(cimags(z));
	float fltSignReal	= localSign(creals(z));
	float fltSignImg	= localSign(cimags(z));

	float fltR = 0, fltS = 0, fltA = 0, fltB = 0;

	float fltTemp = 0;

	float _pfltReal = 0;
	float _pfltImg = 0;

	if( min(fltAbsReal, fltAbsImg) > fltLinf && max(fltAbsReal, fltAbsImg) <= fltLsup)
	  {/* we are in the safe region */
		fltR = ssqrts( (fltAbsReal + 1 )*(fltAbsReal + 1 ) + fltAbsImg*fltAbsImg);
		fltS = ssqrts( (fltAbsReal - 1 )*(fltAbsReal - 1 ) + fltAbsImg*fltAbsImg);
		fltA = 0.5f * ( fltR + fltS );
		fltB = fltAbsReal / fltA;


		/* compute the real part */
		if(fltB <= sfltBcross)
			_pfltReal = sacoss(fltB);
		else if( fltAbsReal <= 1)
			_pfltReal = satans(ssqrts(0.5f * (fltA + fltAbsReal) * (fltAbsImg*fltAbsImg / (fltR + (fltAbsReal + 1)) + (fltS + (1 - fltAbsReal)))) / fltAbsReal);
		else
			_pfltReal = satans((fltAbsImg * ssqrts(0.5f * ((fltA + fltAbsReal) / (fltR + (fltAbsReal + 1)) + (fltA + fltAbsReal) / (fltS + (fltAbsReal - 1))))) / fltAbsReal);

		/* compute the imaginary part */
		if(fltA <= sfltAcross)
		{
			float fltImg1 = 0;

			if(fltAbsReal < 1)
			  /* Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(y**2)/(S+(1.d0-x))) */
				fltImg1 = 0.5f * (fltAbsImg*fltAbsImg / (fltR + (fltAbsReal + 1)) + fltAbsImg*fltAbsImg / (fltS + (1 - fltAbsReal)));
			else
			  /* Am1 = 0.5d0*((y**2)/(R+(x+1.d0))+(S+(x-1.d0))) */
				fltImg1 = 0.5f * (fltAbsImg*fltAbsImg / (fltR + (fltAbsReal + 1)) + (fltS + (fltAbsReal - 1)));
			/* ai = logp1(Am1 + sqrt(Am1*(A+1.d0))) */
			fltTemp = fltImg1 + ssqrts(fltImg1 *( fltA + 1));
			_pfltImg = slog1ps(fltTemp);
		}
		else
		  /* ai = log(A + sqrt(A**2 - 1.d0)) */
			_pfltImg = slogs(fltA + ssqrts(fltA*fltA - 1));
	}
	else
	  {/* evaluation in the special regions ... */
		if(fltAbsImg <= fltEpsm * sabss(fltAbsReal - 1))
		{
			if(fltAbsReal < 1)
			{
				_pfltReal	= sacoss(fltAbsReal);
				_pfltImg	= fltAbsImg / ssqrts((1 + fltAbsReal) * (1 - fltAbsReal));
			}
			else
			{
				_pfltReal = 0;
				if(fltAbsReal <= fltLsup)
				{
					fltTemp		= (fltAbsReal - 1) + ssqrts((fltAbsReal - 1) * (fltAbsReal + 1));
					_pfltImg	= slog1ps(fltTemp);
				}
				else
					_pfltImg	= sfltLn2 + slogs(fltAbsReal);
			}
		}
		else if(fltAbsImg < fltLinf)
		{
			_pfltReal	= ssqrts(fltAbsImg);
			_pfltImg	= _pfltReal;
		}
		else if((fltEpsm * fltAbsImg - 1 >= fltAbsReal))
		{
			_pfltReal	= sfltPi_2;
			_pfltImg	= sfltLn2 + slogs(fltAbsImg);
		}
		else if(fltAbsReal > 1)
		{
			_pfltReal	= satans(fltAbsImg / fltAbsReal);
			fltTemp		= (fltAbsReal / fltAbsImg)*(fltAbsReal / fltAbsImg);
			_pfltImg	= sfltLn2 + slogs(fltAbsImg) + 0.5f * slog1ps(fltTemp);
		}
		else
		{
			float fltTemp2 = ssqrts(1 + fltAbsImg*fltAbsImg);
			_pfltReal	= sfltPi_2;
			fltTemp		= 2 * fltAbsImg * (fltAbsImg + fltTemp2);
			_pfltImg	= 0.5f * slog1ps(fltTemp);
		}
	}
	if(fltSignReal < 0)
		_pfltReal = sfltPi - _pfltReal;

	if(fltAbsImg != 0 || fltSignReal < 0)
		_pfltImg = - fltSignImg * _pfltImg;

	return FloatComplex(_pfltReal, _pfltImg);
}
