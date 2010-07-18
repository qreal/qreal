function SharedInfo = C_WhileExpression(FileInfo,SharedInfo)
// function SharedInfo = C_WhileExpression(FileInfo,SharedInfo)
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
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;

ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CPass1FileName  = FileInfo.Funct(nxtscifunnumber).CPass1FileName;

CPass1WhileProlFileName = FileInfo.Funct(nxtscifunnumber).CPass1WhileProlFileName(SharedInfo.While.Level);
CPass1WhileEpilFileName = FileInfo.Funct(nxtscifunnumber).CPass1WhileEpilFileName(SharedInfo.While.Level);
CDeclarationFileName    = FileInfo.Funct(nxtscifunnumber).CDeclarationFileName;

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Generating C code***',ReportFileName,'file','y');
// #RNU_RES_E
CCall ='';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// ----------------------------
// --- Generate the C call. ---
// ----------------------------

// -------------------------
// --- Manage all cases. ---
// -------------------------
PrintStringInfo('   Handling While Expression with OpColon.',ReportFileName,'file','y'); //NUT: sistema il commento.
   
// -------------------------------------------------------------------------------------
// --- Generate Prologue and Epilogue -> Copy the first N-1 lines of the for.c code. ---
// -------------------------------------------------------------------------------------
[C_Strings,NumCStrings] = File2StringArray(CPass1WhileProlFileName);
C_Strings = stripblanks(C_Strings);
for cntstr = 1:NumCStrings
   // Prologue
   PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+C_Strings(cntstr),CPass1FileName,'file','y','n');
   // Epilogue
   if (length(C_Strings(cntstr)) == 0)
      C_Strings(cntstr) = ' '; // RNU for Bruno: If I don't do that I get a PrintStringInfo error related to mputstr.
      // Function not defined for given argument type(s),
      // check arguments or define function %0_mputstr for overloading.
   end
   PrintStringInfo(C_Strings(cntstr),CPass1WhileEpilFileName ,'file','y','n');
end
// ----------------------------------------
// --- Insert "}" in the epilogue file. ---
// ----------------------------------------
PrintStringInfo('}',CPass1WhileEpilFileName ,'file','y');
   
// ------------------------------
// --- Insert for expression. ---
// ------------------------------
CCall = 'while('+SharedInfo.WhileExpr.CondVar+')';
PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CCall,CPass1FileName,'file','y');
   
// -------------------
// --- Insert "{". ---
// -------------------
CCall = '{';
PrintStringInfo(C_IndentBlanks(SharedInfo.NIndent)+CCall,CPass1FileName,'file','y');
   
// ---------------------------------
// --- Update Indentation Level. ---
// ---------------------------------
SharedInfo.NIndent = SharedInfo.NIndent + 1;   

endfunction
