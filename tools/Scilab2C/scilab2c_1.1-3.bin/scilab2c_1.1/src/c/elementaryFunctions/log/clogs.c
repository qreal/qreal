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

floatComplex	clogs(floatComplex in) {
  static float sR2	= 1.41421356237309504f;

  float _RealIn = creals(in);
  float _ImgIn = cimags(in);

  float _RealOut = 0;
  float _ImgOut = 0;

  float RMax = (float) getOverflowThreshold();
  float LInf = sqrtf((float) getUnderflowThreshold());
  float LSup = sqrtf(0.5f * RMax);

  float AbsReal = fabsf(_RealIn);
  float AbsImg	= fabsf(_ImgIn);

  _ImgOut = atan2f(_ImgIn, _RealIn);

  if(_ImgIn > _RealIn)
    {/* switch Real part and Imaginary part */
      float Temp	= AbsReal;
      AbsReal		= AbsImg;
      AbsImg		= Temp;
    }

  if((0.5 <= AbsReal) && (AbsReal <= sR2))
    _RealOut = 0.5f * slog1ps((AbsReal - 1.0f) * (AbsReal + 1.0f) + AbsImg * AbsImg);
  else if(LInf < AbsImg && AbsReal < LSup)
    _RealOut = 0.5f * slogs(AbsReal * AbsReal + AbsImg * AbsImg);
  else if(AbsReal > RMax)
    _RealOut = AbsReal;
  else
    {
      float Temp = spythags(AbsReal, AbsImg);
      if(Temp <= RMax)
	{
	  _RealOut = slogs(Temp);
	}
      else /* handle rare spurious overflow with : */
	{
	  float Temp2 = AbsImg/AbsReal;
	  _RealOut = slogs(AbsReal) + 0.5f * slog1ps(Temp2 * Temp2);
	}
    }
  return FloatComplex(_RealOut, _ImgOut);
}
