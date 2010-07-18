function [LhsArg,NLhsArg,PrecisionSpecifier,SharedInfo] = AST_GetPrecAndLhsArg(OutArg,NOutArg,FunctionName,FunTypeAnnot,FunSizeAnnot,ASTFunType,FileInfo,SharedInfo);
// function [LhsArg,NLhsArg,PrecisionSpecifier,SharedInfo] = AST_GetPrecAndLhsArg(OutArg,NOutArg,FunctionName,FunTypeAnnot,FunSizeAnnot,ASTFunType,FileInfo,SharedInfo);
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
SCI2CNInArgCheck(argn(2),8,8);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;
// #RNU_RES_B
PrintStringInfo('***Search for Equal Lhs and precision specifier to be applied to the current function.***',ReportFileName,'file','y');
// #RNU_RES_E
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// #RNU_RES_B
// ---------------------------------------
// --- Search for Precision Specifier. ---
// ---------------------------------------
// #RNU_RES_E
if (NOutArg == 1 & FunTypeAnnot == 'FA_TP_USER')
   PrecisionSpecifier = AST_CheckPrecSpecifier(FunctionName,FileInfo,SharedInfo);
   if (PrecisionSpecifier == 'default')
      SearchLevel = 0;
   else
      SearchLevel = 1;
      SharedInfo.SkipNextPrec = 1;
   end
else
   PrecisionSpecifier = '';
   SearchLevel = 0;
end

// #RNU_RES_B
// -------------------------------------------------------------
// --- Check Last Function Condition and update LhsArg info. ---
// -------------------------------------------------------------
// #RNU_RES_E
if (ASTFunType~='Equal')
   // #RNU_RES_B
   PrintStringInfo(' ',ReportFileName,'file','y');
   PrintStringInfo('   Checking presence of Equal after the current function...',ReportFileName,'file','y');
   // #RNU_RES_E
   [LhsArgNames,LhsArgScope,NLhsArg] = AST_CheckLastFunc(SharedInfo.ASTReader.fidAST,SearchLevel);
else
   LhsArgNames = '';
   LhsArgScope = '';
   NLhsArg = 0;
end

// --- Generate the LhsArg structure. ---
LhsArg = [];
for cntarg = 1:NLhsArg
   LhsArg(cntarg).Name = LhsArgNames(cntarg);
   LhsArg(cntarg).Scope = LhsArgScope(cntarg);
end

// #RNU_RES_B
// -------------------------
// --- Check on NLhsArg. ---
// -------------------------
// #RNU_RES_E
if (NLhsArg > 0)
   // #RNU_RES_B
   PrintStringInfo('...Found Equal.',ReportFileName,'file','y');
   PrintStringInfo('OutArg Names will be replaced with Lhs Names of the Equal.',ReportFileName,'file','y');
   // #RNU_RES_E
   SharedInfo.SkipNextEqual = 1; // 1 = the next equal in the AST will not produce C code.
   if (NLhsArg ~= NOutArg)
      SCI2CerrorFile('NLhsArg='+string(NLhsArg)+' must be equal to NOutArg='+string(NOutArg)+'.',ReportFileName);
   end
else
   // #RNU_RES_B
   PrintStringInfo('...Equal not found.',ReportFileName,'file','y');
   // #RNU_RES_E
end

endfunction
