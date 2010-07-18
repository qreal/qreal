function SharedInfo = AST_HandleEndFor(FileInfo,SharedInfo)
// function SharedInfo = AST_HandleEndFor(FileInfo,SharedInfo)
// -----------------------------------------------------------------
// #RNU_RES_B
// Handles the EndFor tag of the AST.
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
// #RNU_RES_E
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
SCI2CNInArgCheck(argn(2),2,2);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname    = SharedInfo.NextSCIFunName;
nxtscifunnumber  = SharedInfo.NextSCIFunNumber;

ReportFileName        = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CPass1FileName        = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
CPass1ForProlFileName = FileInfo.Funct(nxtscifunnumber).CPass1ForProlFileName(SharedInfo.For.Level);
CPass1ForEpilFileName = FileInfo.Funct(nxtscifunnumber).CPass1ForEpilFileName(SharedInfo.For.Level);

PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Handling EndFor***',ReportFileName,'file','y');
CCall ='';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// ----------------------------
// --- Generate the C code. ---
// ----------------------------
// --- Copy Epilogue into C code (Pass1) file. ---
[CLinesArray,N_Lines] = File2StringArray(CPass1ForEpilFileName);
CLinesArray = stripblanks(CLinesArray);

for tmpcnt = 1:N_Lines-1
   PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CLinesArray(tmpcnt),CPass1FileName,'file','y');
end
PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent-1)+CLinesArray(N_Lines),CPass1FileName,'file','y');

// --------------------------
// --- Update SharedInfo. ---
// --------------------------
SharedInfo.NIndent            = SharedInfo.NIndent - 1;

// -------------------------------
// --- Delete temporary files. ---
// -------------------------------
SCI2Cmdelete(FileInfo.Funct(nxtscifunnumber).CPass1ForEpilFileName(SharedInfo.For.Level));

endfunction
