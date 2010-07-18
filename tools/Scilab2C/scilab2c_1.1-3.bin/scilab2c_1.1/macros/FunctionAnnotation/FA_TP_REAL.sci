function opout = FA_TP_REAL(in1)
// function opout = FA_TP_REAL(in1)
// -----------------------------------------------------------------
// Type-Real function for Function Annotations.
// Returns the real precision corresponding to the precision of
// the input operand.
//
// Input data:
// in1: string specifying the data type number 1.
//
// Output data:
// opout: string containing the computed result.
//
// Status:
// 26-Mar-2008 -- Raffaele Nutricato: Author.
// 26-Mar-2008 -- Alberto Morea: Test Ok.
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
opout = in1;

if (in1 == 'c')
   opout = 's';
elseif (in1 == 'z')
   opout = 'd';
end

endfunction
