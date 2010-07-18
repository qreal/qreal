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

#include <math.h>
#include "log.h"
#include "lapack.h"
#include "log1p.h"
#include "pythag.h"

doubleComplex	zlogs(doubleComplex in) {
  static double sR2	= 1.41421356237309504;

  double _RealIn = zreals(in);
  double _ImgIn = zimags(in);

  double _RealOut = 0;
  double _ImgOut = 0;

  double RMax = getOverflowThreshold();
  double LInf = sqrt(getUnderflowThreshold());
  double LSup = sqrt(0.5 * RMax);

  double AbsReal = fabs(_RealIn);
  double AbsImg	= fabs(_ImgIn);

  _ImgOut = atan2(_ImgIn, _RealIn);

  if(_ImgIn > _RealIn)
    {/* switch Real part and Imaginary part */
      double Temp	= AbsReal;
      AbsReal		= AbsImg;
      AbsImg		= Temp;
    }

  if((0.5 <= AbsReal) && (AbsReal <= sR2))
    _RealOut = 0.5 * dlog1ps((AbsReal - 1) * (AbsReal + 1) + AbsImg * AbsImg);
  else if(LInf < AbsImg && AbsReal < LSup)
    _RealOut = 0.5 * dlogs(AbsReal * AbsReal + AbsImg * AbsImg);
  else if(AbsReal > RMax)
    _RealOut = AbsReal;
  else
    {
      double Temp = dpythags(AbsReal, AbsImg);
      if(Temp <= RMax)
	{
	  _RealOut = dlogs(Temp);
	}
      else /* handle rare spurious overflow with : */
	{
	  double Temp2 = AbsImg/AbsReal;
	  _RealOut = dlogs(AbsReal) + 0.5 * dlog1ps(Temp2 * Temp2);
	}
    }
  return DoubleComplex(_RealOut, _ImgOut);
}
