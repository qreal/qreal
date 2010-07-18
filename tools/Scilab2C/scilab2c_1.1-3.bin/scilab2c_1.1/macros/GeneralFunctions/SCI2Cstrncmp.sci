function res = SCI2Cstrncmp(s1,s2,n)
// function res = SCI2Cstrncmp(s1,s2,n)
// -----------------------------------------------------------------
// This function compares first n characters of strings s1 and s2.
// SCI2Cstrncmp(s1,s2,n) returns 1logical T (true) if the first n characters of
// the strings s1 and s2 are the same and logical 0 (false) otherwise.
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 16-Apr-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),3,3);

res = (part(s1,1:n) == part(s2,1:n));
endfunction
