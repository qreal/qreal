function [FunctionName,InArg,NInArg,OutArg,NOutArg] = ...
   AST_GetFuncallPrm(FileInfo,SharedInfo,ASTFunType)
// function [FunctionName,InArg,NInArg,NOutArg] = ...
//    AST_GetFuncallPrm(FileInfo,SharedInfo,ASTFunType)
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
SCI2CNInArgCheck(argn(2),3,3);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;
//#RNU_RES_B
PrintStringInfo('***Retrieving '+ASTFunType+' Parameters from AST***',ReportFileName,'file','y');
//#RNU_RES_E
OutArg  = [];
NOutArg = 0;
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// ------------------------------------------------------
// --- Get Parameters from the AST Funcall structure. ---
// ------------------------------------------------------
if (ASTFunType=='Funcall')
   [FunctionName,InArg,NInArg,NOutArg] = AST_ParseFuncallStruct(FileInfo,SharedInfo);
elseif (ASTFunType=='Operation')
   [FunctionName,InArg,NInArg,NOutArg] = AST_ParseOperStruct(FileInfo,SharedInfo);
elseif (ASTFunType=='Equal')
   [FunctionName,InArg,NInArg,OutArg,NOutArg] = AST_ParseEqualStruct(FileInfo,SharedInfo);
else
   SCI2CerrorFile('Unknown Function type: '+ASTFunType+'.',ReportFileName);
end

endfunction
