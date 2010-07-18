function OutArg = ST_AnalyzeScope(OldOutArg,NOutArg,FileInfo,SharedInfo);
// function OutArg = ST_AnalyzeScope(OldOutArg,NOutArg,FileInfo,SharedInfo);
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
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),4,4);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname   = SharedInfo.NextSCIFunName;
nxtscifunnumber = SharedInfo.NextSCIFunNumber;
ReportFileName  = FileInfo.Funct(nxtscifunnumber).ReportFileName;

// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Getting output arguments info from the symbol table***',ReportFileName,'file','y');
// #RNU_RES_E

OutArg = OldOutArg;
GlobalVarsFileName = FileInfo.GlobalVarFileName;
LocalVarsFileName  = FileInfo.Funct(nxtscifunnumber).LocalVarFileName;
TempVarsFileName   = FileInfo.Funct(nxtscifunnumber).TempVarFileName;
// ---------------------------
// --- End Initialization. ---
// ---------------------------

// #RNU_RES_B
// ------------------------------------------------------------------
// --- Check if the out variables already exist in symbol tables. ---
// ------------------------------------------------------------------
// #RNU_RES_E
for cntout = 1:NOutArg
   // #RNU_RES_B
   PrintStringInfo('   Symbol ""'+OutArg(cntout).Name+'""',ReportFileName,'file','y');
   // #RNU_RES_E
   TBName = OutArg(cntout).Name;
   
   // #RNU_RES_B
   // --- Check in temporary symbol table. ---
   // #RNU_RES_E
   SymbolTableFileName = TempVarsFileName;
   [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension] = ...
      ST_Get(TBName,SymbolTableFileName);
   if (TBFlagfound == 0)
      // #RNU_RES_B
      PrintStringInfo('   ...not found in: '+SymbolTableFileName+'.',ReportFileName,'file','y');
      // #RNU_RES_E
   else
      SCI2CerrorFile('Found a temp symbol in '+SymbolTableFileName+...
         ' with the same name of the equal output argument ""'+TBName+'"".',ReportFileName);
   end
   
   // #RNU_RES_B
   // --- Check in local symbol table. ---
   // #RNU_RES_E
   SymbolTableFileName = LocalVarsFileName;
   [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension] = ...
      ST_Get(TBName,SymbolTableFileName);
   if (TBFlagfound == 0)
      // #RNU_RES_B
      PrintStringInfo('   ...not found in: '+SymbolTableFileName+'.',ReportFileName,'file','y');
      // #RNU_RES_E
   else
      // #RNU_RES_B
      PrintStringInfo('   ...found in: '+SymbolTableFileName+'.',ReportFileName,'file','y');
      // #RNU_RES_E
      OutArg(cntout).Scope     = 'Local';
   end
   
   // #RNU_RES_B
   // --- Check in global symbol table. ---
   // #RNU_RES_E
   if (TBFlagfound == 0)
      // Local wins over global.
      SymbolTableFileName = GlobalVarsFileName;
      [TBFlagfound2,TBType,TBSize,TBValue,TBFindLike,TBDimension] = ...
         ST_Get(TBName,SymbolTableFileName);
      if (TBFlagfound2 == 0)
         // #RNU_RES_B
         PrintStringInfo('   ...not found in: '+SymbolTableFileName+'.',ReportFileName,'file','y');
         // #RNU_RES_E
         if SCI2Cstrncmps1size(SharedInfo.ASTReader.TempVarsName,OutArg(cntout).Name)
            OutArg(cntout).Scope = 'Temp';
         else
            OutArg(cntout).Scope = 'Local';
         end
      else
         // #RNU_RES_B
         PrintStringInfo('   ...found in: '+SymbolTableFileName+'.',ReportFileName,'file','y');
         // #RNU_RES_E
         OutArg(cntout).Scope     = 'Global';
      end
   end
   
   // #RNU_RES_B
   PrintStringInfo('   Type:      '+OutArg(cntout).Type,ReportFileName,'file','y');
   PrintStringInfo('   Size(1):   '+string(OutArg(cntout).Size(1)),ReportFileName,'file','y');
   PrintStringInfo('   Size(2):   '+string(OutArg(cntout).Size(2)),ReportFileName,'file','y');
   PrintStringInfo('   Value:     '+string(OutArg(cntout).Value),ReportFileName,'file','y');
   PrintStringInfo('   FindLike:  '+string(OutArg(cntout).FindLike),ReportFileName,'file','y');
   PrintStringInfo('   Dimension: '+string(OutArg(cntout).Dimension),ReportFileName,'file','y');
   PrintStringInfo('   Scope:     '+string(OutArg(cntout).Scope),ReportFileName,'file','y');
   PrintStringInfo(' ',ReportFileName,'file','y');
   // #RNU_RES_E
end

endfunction
