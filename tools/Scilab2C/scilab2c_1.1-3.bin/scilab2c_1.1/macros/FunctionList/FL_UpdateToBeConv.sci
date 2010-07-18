function SharedInfo = FL_UpdateToBeConv(ASTFunName,CFunName,FunPrecSpecifier,FunTypeAnnot,FunSizeAnnot,InArg,NInArg,OutArg,NOutArg,FileInfo,SharedInfo)
// function SharedInfo = FL_UpdateToBeConv(ASTFunName,CFunName,FunPrecSpecifier,FunTypeAnnot,FunSizeAnnot,InArg,NInArg,OutArg,NOutArg,FileInfo,SharedInfo)
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
SCI2CNInArgCheck(argn(2),11,11);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;

ReportFileName      = FileInfo.Funct(nxtscifunnumber).ReportFileName;
SCI2CAvailableCDat  = FileInfo.FunctionList.SCI2CAvailableCDat;
USER2CAvailableCDat = FileInfo.FunctionList.USER2CAvailableCDat;
ConvertedDat        = FileInfo.FunctionList.ConvertedDat; 
ToBeConvertedDat    = FileInfo.FunctionList.ToBeConvertedDat; 
FunInfoDatDir       = FileInfo.FunctionList.FunInfoDatDir;

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Updating C Function List***',ReportFileName,'file','y');
PrintStringInfo('   C Function Name: '+CFunName,ReportFileName,'file','y');
// #RNU_RES_E
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// #RNU_RES_B
// --------------------------------------------------
// --- Manage anticipated exit from the function. ---
// --------------------------------------------------
//NUT: questo codice e' identico quasi a quello della CFunCall, si pu0 pensare di 
//NUT: di fare un'unica funzione.
// #RNU_RES_E

if (SharedInfo.SkipNextFun > 0)
   // #RNU_RES_B
   PrintStringInfo('   Current function will not be inserted in the Function List.',ReportFileName,'file','y');
   // #RNU_RES_E
   return;
end

// #RNU_RES_B
// Exit if the function is a precision specifier and the corresponding flag is 1.
// #RNU_RES_E
if ((sum(mtlb_strcmp(ASTFunName,SharedInfo.Annotations.DataPrec)) > 0) & ...
    (SharedInfo.SkipNextPrec == 1))
   // #RNU_RES_B
   PrintStringInfo('   Current function will not be inserted in the Function List.',ReportFileName,'file','y');
   // #RNU_RES_E
   return;
end

// #RNU_RES_B
// Exit if the function is OpEqual and the corresponding skip flag is enabled.
// #RNU_RES_E
if ((mtlb_strcmp(ASTFunName,'OpEqual')) & ...
    (SharedInfo.SkipNextEqual == 1))
   // #RNU_RES_B
   PrintStringInfo('   Current function will not be inserted in the Function List.',ReportFileName,'file','y');
   // #RNU_RES_E
   return;
end

// #RNU_RES_B
// ---------------------------------------
// --- If the function is not skipped. ---
// ---------------------------------------
// --- Check existence of the C function. ---
// #RNU_RES_E
flagexist = FL_ExistCFunction(CFunName,USER2CAvailableCDat,SCI2CAvailableCDat,ConvertedDat,ToBeConvertedDat,ReportFileName);

// #RNU_RES_B
// --- Update C function list and dat files. ---
// #RNU_RES_E
if (flagexist  == %F)

   // #RNU_RES_B
   // --- Add C function to the "ToBeConverted" function list. ---
   // #RNU_RES_E
   load(ToBeConvertedDat,'ToBeConverted');
   
   NToConvP1 = size(ToBeConverted,1)+1;
   ToBeConverted(NToConvP1).SCIFunctionName = ASTFunName;
   ToBeConverted(NToConvP1).CFunctionName   = CFunName;
   
   save(ToBeConvertedDat,ToBeConverted);
   SharedInfo.NFilesToTranslate = SharedInfo.NFilesToTranslate + 1;
   
   // #RNU_RES_B
   // --- Generate C Function dat file. ---
   PrintStringInfo('   Add C Function ""'+CFunName+'"" to: '+ToBeConvertedDat,ReportFileName,'file','y');
   // #RNU_RES_E
end

endfunction
