function opoutsize = FA_SZ_OPPLUSA(in1size,in2size)
// function opoutsize = FA_SZ_OPPLUSA(in1size,in2size)
// -----------------------------------------------------------------
// Returns the size of the output computed by OPPLUS operator 
// restricted to arithmetic operations (string operations not supported.)
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
//  M     M    in1r   in1c
//
// Where S means that the input is a scalar
// and M means that the input is a matrix.
// There is also the case related to the string catenation!
// This is the main difference between - and + operators.
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
// 18-Mar-2008 -- Raffaele Nutricato: Author.
// 18-Mar-2008 -- Alberto Morea: Test Ok.
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
// --- Get dimensions of input arguments. ---
in1size = string(in1size);
in2size = string(in2size);
in1dim = GetSymbolDimension(in1size);

if (in1dim == 0)
   opoutsize = in2size;
else
   opoutsize = in1size;
end

endfunction
