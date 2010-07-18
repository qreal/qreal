function opout = FA_SZ_SEL2(in1,in2)
// function opout = FA_SZ_SEL2(in1,in2)
// -----------------------------------------------------------------
// Determines the number of columns of the output arguments 
// according to the number of columns of the first input argument and
// the specifier in2 which can be 1,2 or 'r','c' and 'm'.
// In this release the 'm' specifier is not supported so when it is
// used SCI2C will issue an error.
//
// Input data:
// in1: string specifying a number or a symbol.
// in2: string specifying a number or a symbol.
//
// Output data:
// opout: string containing the computed result.
//
// Status:
// 16-Mar-2008 -- Raffaele Nutricato: Author.
// 16-Mar-2008 -- Alberto Morea: Test Ok.
//
// Copyright 2008 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);
ReportFileName = '';
in2 = string(in2);

if (in2 == '1'| in2 == '""rr""')
   opout = in1;
elseif (in2 == '2'| in2 == '""c""')
   opout = '1';
else   
   PrintStringInfo(' ',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: Cannot associate the second input argument to a known specifier.',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: Please rearrange your code by using one of the following specifiers:',ReportFileName,'both','y');
   PrintStringInfo('SCI2CERROR: 1 or 2.',ReportFileName,'both','y');
   PrintStringInfo(' ',ReportFileName,'both','y');
   SCI2Cerror(' ');
end
endfunction
