function [FileInfo,SharedInfo] = AST_HandleWhileExpr(FileInfo,SharedInfo)
// function [FileInfo,SharedInfo] = AST_HandleWhileExpr(FileInfo,SharedInfo)
// -----------------------------------------------------------------
//#RNU_RES_B
// Handles the WhileExpression tag of the AST.
//
//   txt=['While'
//        '  WhileExpression:'
//        '    '+string(W.expression)
//        '  WhileStatements:'
//        '    '+objectlist2string(W.statements)
//        'EndWhile']
//
//#RNU_RES_E
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 29-Dec-2007 -- Raffaele Nutricato: Author.
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
PfxP1WhileProlFileName = FileInfo.Funct(nxtscifunnumber).PfxP1WhileProlFileName;
PfxP1WhileEpilFileName = FileInfo.Funct(nxtscifunnumber).PfxP1WhileEpilFileName;
PrintStepInfo('Handling While',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
// ---------------------------
// --- End Initialization. ---
// ---------------------------

//#RNU_RES_B
// --- Signal the entrance in a while expression. ---
//#RNU_RES_E
SharedInfo.WhileExpr.OnExec = SharedInfo.WhileExpr.OnExec + 1;

//#RNU_RES_B
// --- Generate the file names for the prologue and epilogue files. ---
//#RNU_RES_E
FileInfo.Funct(nxtscifunnumber).CPass1WhileProlFileName(SharedInfo.While.Level) = ...
   PfxP1WhileProlFileName+string(SharedInfo.While.Level)+'.c';
FileInfo.Funct(nxtscifunnumber).CPass1WhileEpilFileName(SharedInfo.While.Level) = ...
   PfxP1WhileEpilFileName+string(SharedInfo.While.Level)+'.c';

//#RNU_RES_B
// -----------------------------------------------------------
// --- Create a copy of the While Prologue/Epilogue Files. ---
// -----------------------------------------------------------
//#RNU_RES_E
PrintStringInfo(' ',FileInfo.Funct(nxtscifunnumber).CPass1WhileProlFileName(SharedInfo.While.Level),'file');
PrintStringInfo(' ',FileInfo.Funct(nxtscifunnumber).CPass1WhileEpilFileName(SharedInfo.While.Level),'file');

//#RNU_RES_B
// --------------------------------------------------------
// --- Replace the CPass1V1 file with a temp WhileFile. ---
// --------------------------------------------------------
// From now up to Expression: all the C code will be written in a while temporary file.
//#RNU_RES_E
tmpfilename = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
FileInfo.Funct(nxtscifunnumber).CPass1FileName = FileInfo.Funct(nxtscifunnumber).CPass1WhileProlFileName(SharedInfo.While.Level);
FileInfo.Funct(nxtscifunnumber).CPass1WhileProlFileName(SharedInfo.While.Level) = tmpfilename;
//#RNU_RES_B
PrintStringInfo('Redirecting C code to: '+FileInfo.Funct(nxtscifunnumber).CPass1FileName,FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
//#RNU_RES_E

endfunction
