function [RhsNames,RhsScope,NRhs] = AST_ReadEqualRhsNames(FileInfo,SharedInfo)
// function [RhsNames,RhsScope,NRhs] = AST_ReadEqualRhsNames(FileInfo,SharedInfo)
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
// 11-Apr-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;

global SCI2CSTACK 
global StackPosition;
global STACKDEDUG

//#RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Reading Equal Rhs Names***',ReportFileName,'file','y');
//#RNU_RES_E

// -------------------------------
// --- Read Output parameters. ---
// -------------------------------
cntpop = 1;
NRhs = 0;
RhsField(cntpop) = AST_PopASTStack();
RhsNames = [];
while (RhsField(cntpop) ~= 'Expression:')
   NRhs = NRhs + 1;
   [RhsNames(NRhs),RhsScope(NRhs)] = AST_ExtractNameAndScope(RhsField(cntpop));
   cntpop = cntpop + 1;
   RhsField(cntpop) = AST_PopASTStack();
end
RhsNames = SCI2Cflipud(RhsNames);
RhsScope = SCI2Cflipud(RhsScope);

// --- Repush everything into the stack. ---
for cntpush = cntpop:-1:1
   AST_PushASTStack(RhsField(cntpush));
end

endfunction
