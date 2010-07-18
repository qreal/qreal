function OutC_Type = C_Type(ArgType)
// function OutC_Type = C_Type(ArgType)
// -----------------------------------------------------------------
// //NUT: add description here
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 27-Oct-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),1,1);

if (ArgType == 's')
   OutC_Type = 'float';
elseif (ArgType == 'd')
   OutC_Type = 'double';
elseif (ArgType == 'c')
   OutC_Type = 'floatComplex';
elseif (ArgType == 'z')
   OutC_Type = 'doubleComplex';
elseif (ArgType == 'i')
   OutC_Type = 'SCI2Cint';
elseif (ArgType == 'g')
   OutC_Type = 'char';
elseif (ArgType == 'f')
   OutC_Type = 'SCI2CFILEID';
else
   SCI2Cerror('Unknown Argument Type: ""'+ArgType+'"".');
end
endfunction
