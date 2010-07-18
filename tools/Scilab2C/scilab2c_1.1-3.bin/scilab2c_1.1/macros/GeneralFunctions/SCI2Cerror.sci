function SCI2Cerror(errorstring)
// function SCI2Cerror(errorstring)
// -----------------------------------------------------------------
// It is the error function but before issuing the error, performs
// the mclose('all');
//
// Input data:
// errorstring: string which specifies the error message.
//
// Output data:
// ---
//
// Status:
// 02-May-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);


mclose('all')
error('###SCI2CERROR: '+errorstring);
endfunction
