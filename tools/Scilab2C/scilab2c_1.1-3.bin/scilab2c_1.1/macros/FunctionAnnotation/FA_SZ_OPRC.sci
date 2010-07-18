function opoutsize = FA_SZ_OPRC(in1size,in2size)
// function opoutsize = FA_SZ_OPRC(in1size,in2size)
// -----------------------------------------------------------------
// Returns the size of the output computed by OPRC operator.
// 
//
// Status:
// 08-Mar-2008 -- Raffaele Nutricato: Author.
// 08-Mar-2008 -- Alberto Morea: Test Ok.
//
// Copyright 2008 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

in1size = string(in1size);
in2size = string(in2size);

// ------------------------
// --- Generate Output. ---
// ------------------------
// --- Get dimensions of input arguments. ---
in1dim = GetSymbolDimension(in1size);
in2dim = GetSymbolDimension(in2size);

opoutsize(1) = in1size(1);

if (SCI2Cisnum(in1size(2)) & SCI2Cisnum(in2size(2)))
   in1num = eval(in1size(2));
   in2num = eval(in2size(2));
   opoutsize(2) = string(in1num+in2num);
else  
   opoutsize(2) = '('+string(in1size(2))+'+'+string(in2size(2))+')';
end

endfunction
