function typeout = FA_TP_C()
// function typeout = FA_TP_C()
// -----------------------------------------------------------------
// Returns the "single complex" type specifier 
// for Function Annotations.
//
// Input data:
// ---
//
// Output data:
// typeout: string containing the type specifier.
//
// Status:
// 26-Oct-2007 -- Raffaele Nutricato: Author.
// 26-Oct-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2007 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),0,0);


// ------------------------
// --- Generate Output. ---
// ------------------------
typeout = 'c';

endfunction
