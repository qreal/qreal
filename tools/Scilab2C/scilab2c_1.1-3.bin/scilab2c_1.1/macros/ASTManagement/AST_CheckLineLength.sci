function AST_CheckLineLength(instring)
// function AST_CheckLineLength(instring)
// -----------------------------------------------------------------
// "Fixes" the AST generator bug. When a line of code is greater
// than 80 chars the generated AST is wrong.
//
// Input data:
// instring: string read from the AST.
//
// Output data:
// ---
//
// Status:
// 15-May-2008 -- Raffaele Nutricato: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

//if length(instring) > 77
//   SCI2Cerror('Line too long: please reduce the length of the current line.');
//end

endfunction
