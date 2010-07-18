function opout = FA_TP_MAX(in1,in2)
// function opout = FA_TP_MAX(in1,in2)
// -----------------------------------------------------------------
// Type-Maximum function for Function Annotations.
// Returns the maximum between the two data types in input according
// to a predefined priority. For example z(double complex) is 
// greater that c(single complex).
//
// Input data:
// in1: string specifying the data type number 1.
// in2: string specifying the data type number 2.
//
// Output data:
// opout: string containing the computed result.
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
SCI2CNInArgCheck(argn(2),2,2);

// ------------------------
// --- Generate Output. ---
// ------------------------
in1Pin2 = in1+in2;
opout = in1;

if (in2 == 'z')
   opout = 'z';
elseif (in1Pin2 == 'sd')
   opout = 'd';
elseif (in1Pin2 == 'sc')
   opout = 'c';
elseif (in1Pin2 == 'dc')
   opout = 'z';
end

endfunction
