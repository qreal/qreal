function opoutsize = FA_SZ_OPSTAR(in1size,in2size)
// function opoutsize = FA_SZ_OPSTAR(in1size,in2size)
// -----------------------------------------------------------------
// Returns the size of the output computed by OPSTAR operator.
// 
// Assuming:
// size(in1) = [in1r,in1c]
// size(in2) = [in2r,in2c]
// size(out) = [outr,outc]
//
// we have the following combinations:
// in1   in2   outr   outc
// -----------------------
//  S     S    in2r   in2c    
//  S     M    in2r   in2c
//  M     S    in1r   in1c
//  M     M    in1r   in2c
//
// Where S means that the input is a scalar
// and M means that the input is a matrix.
//
// Input data:
// in1size: size of input number 1. It is an array of 2 strings. 
//          The first string specifies the number of rows.
//          The second string specifies the number of columns.
// 
// in2size: size of input number 2. It is an array of 2 strings. 
//          The first string specifies the number of rows.
//          The second string specifies the number of columns.
//
// Output data:
// opoutsize: size of output. It is an array of 2 strings. 
//            The first string specifies the number of rows.
//            The second string specifies the number of columns. 
//
// Status:
// 08-Dec-2007 -- Raffaele Nutricato: Author.
// 08-Dec-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2007 Raffaele Nutricato & Alberto Morea.
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

if (in1dim == 0)
   opoutsize = in2size;
elseif (in2dim == 0)
   opoutsize = in1size;
else
   opoutsize(1) = in1size(1);
   opoutsize(2) = in2size(2);
end

endfunction
