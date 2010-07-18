function EM_ZeroSize(ReportFileName)
// function EM_ZeroSize(ReportFileName)
// -----------------------------------------------------------------
//
// Input data:
// //NUT: Add description here
//
// Output data:
// //NUT: Add description here
//
// Status:
// 13-Feb-2008 -- Raffaele Nutricato: Author.
//
// Copyright 2008 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

PrintStringInfo('SCI2CERROR: Cannot handle zero-size arrays.',ReportFileName,'both','y');
PrintStringInfo(' ',ReportFileName,'both','y');
SCI2Cerror(' ');

endfunction
