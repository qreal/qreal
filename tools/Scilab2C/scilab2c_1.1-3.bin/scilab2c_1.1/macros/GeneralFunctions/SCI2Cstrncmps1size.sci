function res = SCI2Cstrncmps1size(s1,s2);
// function res = SCI2Cstrncmps1size(s1,s2);
// -----------------------------------------------------------------
// #RNU_RES_B
// This function compares first n characters of strings s1 and s2.
// n is the size of the string s1.
// SCI2Cstrncmps1size returns logical T (true) if the first n characters of
// the strings s1 and s2 are the same and logical 0 (false) otherwise.
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// #RNU_RES_E
// Status:
// 16-Apr-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

n = length(s1);
res = (part(s1,1:n) == part(s2,1:n));

endfunction
