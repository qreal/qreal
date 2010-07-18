//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
function opout = FA_REAL(in1, in2)
// function opout = FA_REAL(in1, in2)
// -----------------------------------------------------------------
// Real function for Function Annotations.
// When in1 is a number opout = real(in1); where real returns in1
// real part. If in1 is string opout = in1.
//
// Input data:
// in1: string specifying a number or a symbol.
// in2: string specifying in1 type
//
// Output data:
// opout: string containing the computed result.
//
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

// ------------------------
// --- Generate Output. ---
// ------------------------
if (SCI2Cisnum(in1))
   outnum = real(eval(in1));
   if isnan(outnum)
      opout  = '__SCI2CNANSIZE';
   else
      opout  = string(outnum);
   end
else
   if (in1 == "%i")
      opout  = " 0 ";
   else   
      opout  = in2+"0real"+FA_TP_REAL(in2)+"0"+"("+in1+")";
   end
end
endfunction
