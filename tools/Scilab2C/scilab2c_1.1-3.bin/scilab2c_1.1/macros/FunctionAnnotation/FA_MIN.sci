function opout = FA_MIN(in1,in2)
// function opout = FA_MIN(in1,in2)
// -----------------------------------------------------------------
// Minimum function for Function Annotations.
// When in1 and in2 are both symbols this function returns
// in1.
//
// Input data:
// in1: string specifying a number or a symbol.
// in2: string specifying a number or a symbol.
//
// Output data:
// opout: string containing the computed result.

// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);


// ------------------------
// --- Generate Output. ---
// ------------------------
if (SCI2Cisnum(in1))
   in1num = eval(in1);
   if (SCI2Cisnum(in2))
      in2num = eval(in2);
      outnum = min(in1num,in2num);
      if isnan(outnum)
         opout  = '__SCI2CNANSIZE';
      else
         opout  = string(outnum);
      end
   else
      if (in1num == 1)
         opout  = string(in2);
      end
   end
else
   opout  = string(in1);
end   

endfunction
