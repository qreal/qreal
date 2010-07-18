function typeout = FA_TP_COMPLEX(in1)
// function typeout = FA_TP_COMPLEX(in1)
// -----------------------------------------------------------------
// Converts into complex data type the input argument, by preserving
// the precision of the input argument.
// See following examples:
// FA_TP_COMPLEX('s') = 'c'
// FA_TP_COMPLEX('d') = 'z'
// FA_TP_COMPLEX('c') = 'c'
// FA_TP_COMPLEX('z') = 'z'
//
// Input data:
// in1: string specifying the data type number 1.
//
// Output data:
// typeout: string containing the type specifier.
//
// Status:
// 26-Jan-2008 -- Raffaele Nutricato: Author.
// 26-Jan-2008 -- Alberto Morea: Test Ok.
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
if (in1 == 's')
   typeout = 'c';
elseif (in1 == 'd')
   typeout = 'z';
else
   typeout = in1;
end
endfunction
