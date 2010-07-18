function outstring = SCI2Cstring(innum)
// function outstring = SCI2Cstring(innum)
// -----------------------------------------------------------------
// #RNU_RES_B
// It fixes the bug of string function when applied to
// exponential formats: 
// Example:
// -->string(10e-10)
// ans  =
// 1.000D-09 
// Note how the "D" is syntactically wrong.
//
// Input data:
// innnum: input number to be converted into string.
//
// Output data:
// outstring: string containing the conversion.
// #RNU_RES_E
//
// Status:
// 07-May-2008 -- Nutricato Raffaele: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

outstring=strsubst(string(innum),'D','e');

endfunction
