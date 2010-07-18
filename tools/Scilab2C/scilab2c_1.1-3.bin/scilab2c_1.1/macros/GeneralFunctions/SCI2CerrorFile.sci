function SCI2CerrorFile(errorstring,filename);
// function SCI2CerrorFile(errorstring,filename);
// -----------------------------------------------------------------
// It is the error function but before issuing the error, performs
// the mclose('all'); It also write the error string into the
// file specified by filename.
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 02-May-2006 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

mclose('all')
PrintStringInfo('Error: '+errorstring,filename,'both');
error('####SCI2C_ERROR -> Read File: '+filename+'.');
endfunction
