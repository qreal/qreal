function SharedInfo = AST_HandleEndWhile(FileInfo,SharedInfo)
// function SharedInfo = AST_HandleEndWhile(FileInfo,SharedInfo)
// -----------------------------------------------------------------
//#RNU_RES_B
// Handles the EndWhile tag of the AST.
//
//   txt=['While'
//        '  WhileExpression:'
//        '    '+string(W.expression)
//        '  WhileStatements:'
//        '    '+objectlist2string(W.statements)
//        'EndWhile']
//#RNU_RES_E
//
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

ReportFileName          = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CPass1FileName          = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
CPass1WhileEpilFileName = FileInfo.Funct(nxtscifunnumber).CPass1WhileEpilFileName(SharedInfo.While.Level);

PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Handling EndWhile***',ReportFileName,'file','y');
CCall ='';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

//#RNU_RES_B
// ----------------------------
// --- Generate the C code. ---
// ----------------------------
// --- Copy Epilogue into C code (Pass1) file. ---
//#RNU_RES_E
[CLinesArray,N_Lines] = File2StringArray(CPass1WhileEpilFileName);
CLinesArray = stripblanks(CLinesArray);

for tmpcnt = 1:N_Lines-1
   PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CLinesArray(tmpcnt),CPass1FileName,'file','y');
end
PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent-1)+CLinesArray(N_Lines),CPass1FileName,'file','y');

//#RNU_RES_B
// --------------------------
// --- Update SharedInfo. ---
// --------------------------
//#RNU_RES_E
SharedInfo.NIndent = SharedInfo.NIndent - 1;

// -------------------------------
// --- Delete temporary files. ---
// -------------------------------
SCI2Cmdelete(CPass1WhileEpilFileName);

endfunction
