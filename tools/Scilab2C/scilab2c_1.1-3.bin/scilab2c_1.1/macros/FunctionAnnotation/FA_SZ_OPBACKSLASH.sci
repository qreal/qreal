function opoutsize = FA_SZ_OPBACKSLASH(in1size,in2size)
// function opoutsize = FA_SZ_OPBACKSLASH(in1size,in2size)
// -----------------------------------------------------------------
// Returns the size of the output computed by OPBACKSLASH operator.
// 
//
// Status:
// 08-Mar-2008 -- Alberto Morea: Author.
// 08-Mar-2008 -- Raffaele Nutricato: Test Ok.
//
// Copyright 2008 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

// ------------------------
// --- Generate Output. ---
// ------------------------
in1size=string(in1size);
in2size=string(in2size);

// --- Get dimensions of input arguments. ---
in1dim = GetSymbolDimension(in1size);
in2dim = GetSymbolDimension(in2size);

if (in1dim == 0)
   opoutsize = in2size;
elseif (in2dim == 0)
   opoutsize = in1size;
else
   opoutsize(1) = in1size(2);
   opoutsize(2) = in2size(2);
end

endfunction
