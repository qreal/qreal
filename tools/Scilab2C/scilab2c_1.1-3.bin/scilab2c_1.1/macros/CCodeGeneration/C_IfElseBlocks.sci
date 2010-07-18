function SharedInfo = C_IfElseBlocks(FileInfo,SharedInfo,InOutStatements)
// function SharedInfo = C_IfElseBlocks(FileInfo,SharedInfo,InOutStatements)
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
SCI2CNInArgCheck(argn(2),3,3);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname    = SharedInfo.NextSCIFunName;
nxtscifunnumber  = SharedInfo.NextSCIFunNumber;

ReportFileName   = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CPass1FileName = FileInfo.Funct(nxtscifunnumber).CPass1FileName;

IndentLevel      = SharedInfo.NIndent;

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('   Generate ""{"" or ""}"" code for if/else statement',ReportFileName,'file','y');
// #RNU_RES_E
CCall = '';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// #RNU_RES_B
// -----------------------------------------------------
// --- Generate the C call/Update indentation level. ---
// -----------------------------------------------------
// #RNU_RES_E
if (InOutStatements=='in')
   CCall = CCall+'{';
   PrintStringInfo('   '+CCall,ReportFileName,'file','y');
   PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
   IndentLevel = IndentLevel + 1;
elseif (InOutStatements=='out')
   CCall = CCall+'}';
   IndentLevel = IndentLevel - 1;
   PrintStringInfo('   '+CCall,ReportFileName,'file','y');
   PrintStringInfo(C_IndentBlanks(IndentLevel)+CCall,CPass1FileName,'file','y');
else
   SCI2CerrorFile('Unknown setting for InOutStatements: '+InOutStatements'.',ReportFileName);
end

// #RNU_RES_B
PrintStringInfo('   Updating indentation level to:'+string(IndentLevel),ReportFileName,'file','y');
// #RNU_RES_E
SharedInfo.NIndent = IndentLevel;

endfunction
