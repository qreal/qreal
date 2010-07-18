function outbool = IsNanSize(instring)
// function outbool = IsNanSize(instring)
// -----------------------------------------------------------------
// #RNU_RES_B
// It searches for __SCI2CNANSIZE string in the string which specifies the
// size of the argument. Useful to find if a given size contains
// a nan value. In this case an error is issued.
// IsNanSize = '__SCI2CNANSIZE'   -> True
// IsNanSize = 'c*__SCI2CNANSIZE' -> True
// IsNanSize = 'c+b'   -> False
// #RNU_RES_E
//
// Input data:
// instring: string to analyze.
//
// Output data:
// outbool: %T if nan string has been found.
//
// Status:
// 11-Feb-2008 -- Nutricato Raffaele: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);


outbool = %F;
indexval = strindex(instring,'__SCI2CNANSIZE');

if(length(indexval)>=1)
   outbool = %T;
end

endfunction
