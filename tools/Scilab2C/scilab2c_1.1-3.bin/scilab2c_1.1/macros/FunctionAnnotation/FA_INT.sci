function opout = FA_INT(in1)
// function opout = FA_INT(in1)
// -----------------------------------------------------------------
// Int function for Function Annotations.
// When in1 is a number opout = int(in1); where int truncates in1
// to integer. If in1 is string opout = in1.
//
// Input data:
// in1: string specifying a number or a symbol.
//
// Output data:
// opout: string containing the computed result.
//
// Status:
// 26-Oct-2007 -- Raffaele Nutricato: Author.
// 26-Oct-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2008 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

// ------------------------
// --- Generate Output. ---
// ------------------------
if (SCI2Cisnum(in1))
   outnum = int(eval(in1));
   if isnan(outnum)
      opout  = '__SCI2CNANSIZE';
   else
      opout  = string(outnum);
   end
else
   opout  = in1;
end   
endfunction
