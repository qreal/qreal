function [TBFlagfound,TBFlagEqualSymbols] = ST_MatchSymbol(TBName,TBType,TBSize,TBValue,TBFindLike,TBDimension,SymbolTableFileName,MatchRule)
// function [TBFlagfound,TBFlagEqualSymbols] = ST_MatchSymbol(TBName,TBType,TBSize,TBValue,TBFindLike,TBDimension,SymbolTableFileName,MatchRule)
// -----------------------------------------------------------------
// Match function for the symbol table.
//
// Input data:
// MatchRule: can be 'all','type','size','none'
// //NUT: add description here
//
// Output data:
// TBFlagfound: 0 = if the symbol doesn't exits.
//              1 = the symbol exits.
//              2 = the symbol exists but it is a non-initialized global variable.
// TBFlagEqualSymbols: 0 if the two symbols don't have the same settings,
//                     1 if the two symbols have the same settings.  
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
SCI2CNInArgCheck(argn(2),8,8);

TBFlagfound = 0;
TBFlagEqualSymbols = 0;

// --- Find symbol (If exists). ---
[TBFlagfound,tmpType,tmpSize,tmpValue,tmpFindLike,tmpDimension] = ...
   ST_Get(TBName,SymbolTableFileName);

if (TBFlagfound == 1)
   if (tmpType == 'GBLToBeDefined')
      TBFlagfound = 2;
      TBFlagEqualSymbols = 0; // I don't want to force the error issue in ST_InsOutArg.sci
   else
      // Symbol already exists. Check that it has the same settings of the current output argument.
      TBFlagEqualSymbols = 1;
      if (MatchRule == 'type' | MatchRule == 'all')
         if (mtlb_strcmp(tmpType,TBType) == %F)
            TBFlagEqualSymbols = 0;
         end
      end
      if (MatchRule == 'size' | MatchRule == 'all')
         // First check the dimension.
         if (tmpDimension ~= TBDimension)
            TBFlagEqualSymbols = 0;
         end
         // Then if the size is a number also its value is compared.
         if (SCI2Cisnum(tmpSize(1))) & (SCI2Cisnum(TBSize(1)))
            if (mtlb_strcmp(tmpSize(1),TBSize(1)) == %F)
               TBFlagEqualSymbols = 0;
            end
         end
         if (SCI2Cisnum(tmpSize(2))) & (SCI2Cisnum(TBSize(2)))
            if (mtlb_strcmp(tmpSize(2),TBSize(2)) == %F)
               TBFlagEqualSymbols = 0;
            end
         end
      end
   end
end
   
endfunction
