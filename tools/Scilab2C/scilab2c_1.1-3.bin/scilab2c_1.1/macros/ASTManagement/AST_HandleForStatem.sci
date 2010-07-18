function [FileInfo,SharedInfo] = AST_HandleForStatem(FileInfo,SharedInfo)
// -----------------------------------------------------------------
//#RNU_RES_B
// Handles the ForStatements tag of the AST.
// overloading function for "for" type tlist string function
// this is a node of the AST
// fields:  
//    expression : "expression" type tlist (the loop expression)
//    statements : list of "equal" type tlist and list('EOL') (the
//                           for instructions list)
//   txt=['For'
//         '  ForExpression:'
//         '    '+string(F.expression)
//         '  ForStatements:'
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
// 15-Nov-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(2,2,2);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;
PrintStepInfo('Handling ForStatements',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
// ---------------------------
// --- End Initialization. ---
// ---------------------------

//#RNU_RES_B
// ---------------------------------------------
// --- Resume the correct name for CPass1V1. ---
// ---------------------------------------------
//#RNU_RES_E
tmpfilename = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
FileInfo.Funct(nxtscifunnumber).CPass1FileName = FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level);
FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level) = tmpfilename;
PrintStringInfo('   Redirecting C code to: '+FileInfo.Funct(nxtscifunnumber).CPass1FileName,FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');

//#RNU_RES_B
// ------------------------
// --- Generate C code. ---
// ------------------------
//#RNU_RES_E
SharedInfo = C_ForExpression(FileInfo,SharedInfo);

//#RNU_RES_B
// --------------------------
// --- Update SharedInfo. ---
// --------------------------
// Signal the exit from a for expression.
//#RNU_RES_E
SharedInfo.ForExpr.OnExec = SharedInfo.ForExpr.OnExec - 1;
SharedInfo.ForExpr.IntCntArg     = [];
SharedInfo.ForExpr.MtxValCntArg  = [];
SharedInfo.ForExpr.SclValCntArg  = [];
SharedInfo.ForExpr.OpColonInfoIn1 = '';
SharedInfo.ForExpr.OpColonInfoIn2 = '';
SharedInfo.ForExpr.OpColonInfoIn3 = '';


SharedInfo.ForExpr.AssignmentFun = 0;

// -------------------------------
// --- Delete temporary files. ---
// -------------------------------
SCI2Cmdelete(FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level));

endfunction
