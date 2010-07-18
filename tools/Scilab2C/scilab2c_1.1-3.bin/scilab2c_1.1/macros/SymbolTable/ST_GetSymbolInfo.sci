function [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension,TBScope] = ST_GetSymbolInfo(TBName,FileInfo,SharedInfo)
// function [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension,TBScope] = ST_GetSymbolInfo(TBName,FileInfo,SharedInfo)
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
// 26-Oct-2007 -- Raffaele Nutricato: Author.
// 26-Oct-2007 -- Alberto Morea: Test Ok.
//
// Copyright 2007 Raffaele Nutricato & Alberto Morea.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),3,3);

// -----------------------
// --- Initialization. ---
// -----------------------
// --- Extraction of the function name and number. ---
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;

GlobalVarsFileName = FileInfo.GlobalVarFileName;
LocalVarsFileName  = FileInfo.Funct(nxtscifunnumber).LocalVarFileName;
TempVarsFileName   = FileInfo.Funct(nxtscifunnumber).TempVarFileName;

TBFlagfound = 0;
TBType      = '';
TBSize(1)   = '';
TBSize(2)   = '';
TBValue     = %nan
TBFindLike  = %nan
TBDimension = %nan;
TBScope     = '';
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// #RNU_RES_B
// ------------------------------------------------
// --- Search in the temporary variables table. ---
// ------------------------------------------------
PrintStringInfo('Searching ""'+TBName+'"" in '+FileInfo.Funct(nxtscifunnumber).TempVarFileName+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
// #RNU_RES_E
[TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension] = ...
   ST_Get(TBName,TempVarsFileName);
if (TBFlagfound == 1);
   // #RNU_RES_B
   PrintStringInfo('...Found in: ""'+FileInfo.Funct(nxtscifunnumber).TempVarFileName+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   // #RNU_RES_E
   TBScope = 'Temp';
end

// --------------------------------------------
// --- Search in the local variables table. ---
// --------------------------------------------
if (TBFlagfound == 0);
   // #RNU_RES_B
   PrintStringInfo('Searching ""'+TBName+'"" in '+FileInfo.Funct(nxtscifunnumber).LocalVarFileName+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   // #RNU_RES_E
   [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension] = ...
      ST_Get(TBName,LocalVarsFileName);
   if (TBFlagfound == 1);
      // #RNU_RES_B
      PrintStringInfo('...Found in: ""'+FileInfo.Funct(nxtscifunnumber).LocalVarFileName+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
      // #RNU_RES_E
      TBScope = 'Local';
   end
end

// #RNU_RES_B
// ---------------------------------------------
// --- Search in the global variables table. ---
// ---------------------------------------------
// #RNU_RES_E
if (TBFlagfound == 0);
   // #RNU_RES_B
   PrintStringInfo('Searching ""'+TBName+'"" in '+FileInfo.GlobalVarFileName+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
   // #RNU_RES_E
   [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension] = ...
      ST_Get(TBName,GlobalVarsFileName);
   if (TBFlagfound == 1);
      // #RNU_RES_B
      PrintStringInfo('...Found in: ""'+FileInfo.GlobalVarFileName+'.',FileInfo.Funct(nxtscifunnumber).ReportFileName,'file');
      // #RNU_RES_E
      TBScope = 'Global';
   end
end

endfunction
