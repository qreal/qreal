function ST_InsOutArg(OutArg,NOutArg,FileInfo,SharedInfo,MatchRule)
// function ST_InsOutArg(OutArg,NOutArg,FileInfo,SharedInfo,MatchRule)
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
SCI2CNInArgCheck(argn(2),5,5);

// -----------------------
// --- Initialization. ---
// -----------------------
nxtscifunname        = SharedInfo.NextSCIFunName;
nxtscifunnumber      = SharedInfo.NextSCIFunNumber;
ReportFileName       = FileInfo.Funct(nxtscifunnumber).ReportFileName;
CDeclarationFileName = FileInfo.Funct(nxtscifunnumber).CDeclarationFileName;
CGblDeclarFileName   = FileInfo.Funct(nxtscifunnumber).CGblDeclarFileName;

GlobalVarsFileName   = FileInfo.GlobalVarFileName;
LocalVarsFileName    = FileInfo.Funct(nxtscifunnumber).LocalVarFileName;
TempVarsFileName     = FileInfo.Funct(nxtscifunnumber).TempVarFileName;

CPass1FileName       = FileInfo.Funct(nxtscifunnumber).CPass1FileName;
CPass1FreeFileName   = FileInfo.Funct(nxtscifunnumber).CPass1FreeFileName;


// #RNU_RES_B
PrintStringInfo(' ',ReportFileName,'file','y');
PrintStringInfo('***Putting output arguments in the symbol table***',ReportFileName,'file','y','n');
// #RNU_RES_E
// ---------------------------
// --- End Initialization. ---
// ---------------------------


for counteroutput = 1:NOutArg
   // #RNU_RES_B
   PrintStringInfo('   Symbol ""'+OutArg(counteroutput).Name+'""',ReportFileName,'file','y','n');
   PrintStringInfo('   Type:      '+OutArg(counteroutput).Type,ReportFileName,'file','y','n');
   PrintStringInfo('   Size(1):   '+string(OutArg(counteroutput).Size(1)),ReportFileName,'file','y','n');
   PrintStringInfo('   Size(2):   '+string(OutArg(counteroutput).Size(2)),ReportFileName,'file','y','n');
   PrintStringInfo('   Value:     '+string(OutArg(counteroutput).Value),ReportFileName,'file','y','n');
   PrintStringInfo('   FindLike:  '+string(OutArg(counteroutput).FindLike),ReportFileName,'file','y','n');
   PrintStringInfo('   Dimension: '+string(OutArg(counteroutput).Dimension),ReportFileName,'file','y','n');
   PrintStringInfo('   Scope:     '+string(OutArg(counteroutput).Scope),ReportFileName,'file','y','n');
   PrintStringInfo(' ',ReportFileName,'file','y','n');
   // #RNU_RES_E
   
   if mtlb_strcmp(OutArg(counteroutput).Scope,'Temp')
      SymbTableFileName = TempVarsFileName;
   elseif mtlb_strcmp(OutArg(counteroutput).Scope,'Local')
      SymbTableFileName = LocalVarsFileName;
   elseif mtlb_strcmp(OutArg(counteroutput).Scope,'Global')
      SymbTableFileName = GlobalVarsFileName;
   else
      SCI2Cerror('Unknown scope ""'+OutArg(counteroutput).Scope+'"" for symbol: '+OutArg(counteroutput).Name);
   end
   // #RNU_RES_B
   PrintStringInfo('   Setting symbol ""'+OutArg(counteroutput).Name+'"" in '+SymbTableFileName+'.',ReportFileName,'file','y');
   // #RNU_RES_E
   
   // #RNU_RES_B
   // Check existence and conflicts in the symbol table.
   // Here we have four possibilities:
   // 1. the symbol is a global variable not initialized yet -> we have to initialize it.
   // 2. the symbol already exists with different settings -> we have to issue an error.
   // 3. the symbol already exists with the same settings -> ok, we don't have to do nothing.
   // 4. the symbol doesn't exist -> we have to insert it into the table.
   // #RNU_RES_E
   [TBFlagfound,TBFlagEqualSymbols] = ...
      ST_MatchSymbol(OutArg(counteroutput).Name,...
      OutArg(counteroutput).Type,...
      OutArg(counteroutput).Size,...
      OutArg(counteroutput).Value,...
      OutArg(counteroutput).FindLike,...
      OutArg(counteroutput).Dimension,...
      SymbTableFileName,MatchRule);
   
   if (TBFlagfound == 1)
      if (TBFlagEqualSymbols == 0)
         PrintStringInfo(' ',ReportFileName,'both','y');
         PrintStringInfo('SCI2CERROR: Symbol Table Conflict. Trying to insert again symbol ""'+...
                         OutArg(counteroutput).Name+'"" with different settings',ReportFileName,'both','y');
         PrintStringInfo('SCI2CERROR: Please check that you are not using variable ""'+OutArg(counteroutput).Name+'""',ReportFileName,'both','y');
         PrintStringInfo('SCI2CERROR: with different sizes and/or types.',ReportFileName,'both','y');
         PrintStringInfo(' ',ReportFileName,'both','y');
         SCI2Cerror(' ');
      else
         // #RNU_RES_B
         // It's ok symbols do match.
         //NUT: forse occorre un altro check sulla size per capire se occore fare il malloc.
         //NUT: qui entro anche quando ho una variabile global gia' dichiarata tale in un altro
         //NUT: per cui devo dichiararala come external.
         //RNU qui ci puoi mettere una warning quando stai riallocando uno stesso simbolo con size simbolica.
         //RNU puoi anche aggiungere del codice in c o un semplice commento. per esempio una funzione c del tipo checksize che controlla il valore
         //RNU prima dopo delle size di una data variabile. Cosa succede se cambio la size anche nel caso di array e approccio
         //RNU di resize non attivo? L'unica cosa e' che molte size numeriche scompaiono e incomincio a creare numerose variabili
         //RNU con size simbolica.
         
         // If the symbol is scalar we update its value if it is an array we update its size
         // only in case we are using the 'REALLOC_ALL_RESIZE_ALL' resize approach
         // #RNU_RES_E
         if ((GetSymbolDimension(OutArg(counteroutput).Size)) == 0 | (SharedInfo.ResizeApproach=='REALLOC_ALL_RESIZE_ALL'))
            ST_Set(OutArg(counteroutput).Name,...
               OutArg(counteroutput).Type,...
               OutArg(counteroutput).Size,...
               OutArg(counteroutput).Value,...
               OutArg(counteroutput).FindLike,...
               OutArg(counteroutput).Dimension,...
               SymbTableFileName);
         end
      end
   elseif (TBFlagfound == 2)
      // #RNU_RES_B
      // We have a non-initialized global variable.
      // Set the non-initialized global variable.
      PrintStringInfo(' Found a non-initialized global variable.',ReportFileName,'file','y');
      // #RNU_RES_E
      ST_Set(OutArg(counteroutput).Name,...
         OutArg(counteroutput).Type,...
         OutArg(counteroutput).Size,...
         OutArg(counteroutput).Value,...
         OutArg(counteroutput).FindLike,...
         OutArg(counteroutput).Dimension,...
         SymbTableFileName);
      IndentLevel = 0; //NUT: forced always to 1
      FlagExt = 0;
      C_GenDeclarations(OutArg(counteroutput),CGblDeclarFileName,IndentLevel,ReportFileName,FlagExt,SharedInfo.ResizeApproach);
      IndentLevelMalloc      = SharedInfo.NIndent;
      // #RNU_RES_B
      //RNU da verificare bene qui. Cio' che si verifica e' che se la size della globale e' simbolica
      //RNU allora si assume che essa sia da allocare come puntatore e poi realloc.
      // #RNU_RES_E
      C_MemAllocOutTempVars(OutArg(counteroutput),1,CPass1FileName,CPass1FreeFileName,IndentLevelMalloc,ReportFileName,SharedInfo.ResizeApproach);
   else
      if (OutArg(counteroutput).FindLike == 1)
         // #RNU_RES_B
         // In presence of find-like functions the size must be always symbolic.
         // Don't change here the value of OutArg.Size because the first time
         // I need them to declare the OutArg variable with the values assumed by OutArg.Size.
         // #RNU_RES_E
         TmpOutArgSize(1) = '__'+OutArg(counteroutput).Name+'Size[0]';
         TmpOutArgSize(2) = '__'+OutArg(counteroutput).Name+'Size[1]';
      else
         TmpOutArgSize(1) = OutArg(counteroutput).Size(1);
         TmpOutArgSize(2) = OutArg(counteroutput).Size(2);
      end
      // #RNU_RES_B
      // Set a new symbol.
      // #RNU_RES_E
      ST_Set(OutArg(counteroutput).Name,...
         OutArg(counteroutput).Type,...
         TmpOutArgSize,...
         OutArg(counteroutput).Value,...
         OutArg(counteroutput).FindLike,...
         OutArg(counteroutput).Dimension,...
         SymbTableFileName);
      IndentLevelDeclaration = 1; //NUT: per ora lo forzo sempre a 1
      IndentLevelMalloc      = SharedInfo.NIndent;
      FlagExt = 0;
      C_GenDeclarations(OutArg(counteroutput),CDeclarationFileName,IndentLevelDeclaration,ReportFileName,FlagExt,SharedInfo.ResizeApproach);
      // #RNU_RES_B
      //RNU aggiunta qui in modo che le malloc saranno fatte una sola volta:
      //RNU verifica che tutto funzioni e chi altro usa la C_MemAlloc per capire se si puo' ottimizzare per questo stadio.
      // #RNU_RES_E
      C_MemAllocOutTempVars(OutArg(counteroutput),1,CPass1FileName,CPass1FreeFileName,IndentLevelMalloc,ReportFileName,SharedInfo.ResizeApproach);
   end
   
end

endfunction
