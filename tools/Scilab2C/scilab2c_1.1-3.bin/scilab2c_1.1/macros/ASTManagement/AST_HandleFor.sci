function FileInfo = AST_HandleFor(FileInfo,SharedInfo)
// function FileInfo = AST_HandleFor(FileInfo,SharedInfo)
// -----------------------------------------------------------------
//#RNU_RES_B
// Handles the For tag of the AST.
//
// overloading function for "for" type tlist string function
// this is a node of the AST
// fields:  
//    expression : "expression" type tlist (the loop expression)
//    statements : list of "equal" type tlist and list('EOL') (the
//                           for instructions list)
//   txt=['For'
//         '  Expression:'
//         '    '+string(F.expression)
//         '  Statements:'
//         '    '+objectlist2string(F.statements)
//         'EndFor']
//
//#RNU_RES_E
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 10-Nov-2007 -- Raffaele Nutricato: Author.
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
nxtscifunname        = SharedInfo.NextSCIFunName;
nxtscifunnumber      = SharedInfo.NextSCIFunNumber;
ReportFileName       = FileInfo.Funct(nxtscifunnumber).ReportFileName;
PfxP1ForProlFileName = FileInfo.Funct(nxtscifunnumber).PfxP1ForProlFileName;
PfxP1ForEpilFileName = FileInfo.Funct(nxtscifunnumber).PfxP1ForEpilFileName;
PrintStepInfo('Handling For',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
// ---------------------------
// --- End Initialization. ---
// ---------------------------

//#RNU_RES_B
// --- Signal the entrance in a for expression. ---
//#RNU_RES_E
SharedInfo.ForExpr.OnExec = SharedInfo.ForExpr.OnExec + 1;

//#RNU_RES_B
// --- Generate the file names for the prologue and epilogue files. ---
//#RNU_RES_E
FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level) = ...
   PfxP1ForProlFileName+string(SharedInfo.For.Level)+'.c';
FileInfo.Funct(nxtscifunnumber).CPass1ForEpilFileName(SharedInfo.For.Level) = ...
   PfxP1ForEpilFileName+string(SharedInfo.For.Level)+'.c';

//#RNU_RES_B
// ---------------------------------------------------------
// --- Create a copy of the For Prologue/Epilogue Files. ---
// ---------------------------------------------------------
//#RNU_RES_E
PrintStringInfo(' ',FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level),'file');
PrintStringInfo(' ',FileInfo.Funct(nxtscifunnumber).CPass1ForEpilFileName(SharedInfo.For.Level),'file');

//#RNU_RES_B
// ------------------------------------------------------
// --- Replace the CPass1V1 file with a temp ForFile. ---
// ------------------------------------------------------
// From now up to Expression: all the C code will be written in a for temporary file.
//#RNU_RES_E
tmpfilename = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
FileInfo.Funct(nxtscifunnumber).CPass1FileName = FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level);
FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level) = tmpfilename;
PrintStringInfo('Redirecting C code to: '+FileInfo.Funct(nxtscifunnumber).CPass1FileName,FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');

endfunction
