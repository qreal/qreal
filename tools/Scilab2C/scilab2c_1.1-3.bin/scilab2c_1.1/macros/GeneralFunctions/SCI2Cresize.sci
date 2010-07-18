function out = SCI2Cresize(in)
// function out = SCI2Cresize(in)
// -----------------------------------------------------------------
// #RNU_RES_B
// It is a dummy function used by the programmer to specify at a given
// point that a variable is changing its size. This will be translated
// into C code by re-assigning the size array.
// Next releases of this function will include check to avoid
// increment of the size outside the limits specified by the first
// initialization of the variable.
//
// Input data:
// in: input variable to be resized
//
// Output data:
// out: resized variable
//
// #RNU_RES_E
// Status:
// 10-Jun-2008 -- Nutricato Raffaele: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

out = in;

endfunction
