function opoutsize = FA_SZ_OPAPEX(in1size)
// function opoutsize = FA_SZ_OPAPEX(in1size)
// -----------------------------------------------------------------
// Returns the size of the output computed by OPAPEX operator.
// 
//
// Status:
// 08-Jan-2008 -- Raffaele Nutricato: Author.
// 08-Jan-2008 -- Alberto Morea: Test Ok.
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
opoutsize(1) = string(in1size(2));
opoutsize(2) = string(in1size(1));

endfunction
