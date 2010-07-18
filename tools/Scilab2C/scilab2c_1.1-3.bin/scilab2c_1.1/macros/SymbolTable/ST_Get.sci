function [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension] = ...
   ST_Get(Field_Name,SymbolTableFileName)
// function [TBFlagfound,TBType,TBSize,TBValue,TBFindLike,TBDimension] = ...
//    ST_Get(Field_Name,SymbolTableFileName)
// -----------------------------------------------------------------
// #RNU_RES_B
// Get function for the symbol table.
//
// #RNU_RES_E
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
SCI2CNInArgCheck(argn(2),2,2);

// --------------------------
// --- Load Symbol Table. ---
// --------------------------
[tmpnams,tmptyps,tmpdims,tmpvols]=listvarinfile(SymbolTableFileName);
if (max(size(tmpnams)) > 1)
    SCI2Cerror('More than one variable found in ""'+SymbolTableFileName+'"".');
end
load(SymbolTableFileName,tmpnams);
SCI2CSymbolTable = eval(tmpnams);
// ------------------------------
// --- End Load Symbol Table. ---
// ------------------------------

TBFlagfound = 0;
TBType      = '';
TBSize(1)   = '';
TBSize(2)   = '';
TBValue     = %nan;
TBFindLike  = %nan;
TBDimension = %nan;
if (TBFlagfound == 0)
   NEntries = max(size(SCI2CSymbolTable));
   for countertable = 1:NEntries
      if (mtlb_strcmp(Field_Name,SCI2CSymbolTable(countertable).Name))
         TBFlagfound = TBFlagfound + 1;
         TBType      = SCI2CSymbolTable(countertable).Type; // String
         TBSize      = SCI2CSymbolTable(countertable).Size; // String
         TBValue     = SCI2CSymbolTable(countertable).Value; 
         TBFindLike  = SCI2CSymbolTable(countertable).FindLike; // Number: 0 or 1.
         TBDimension = SCI2CSymbolTable(countertable).Dimension; // Number: 0 or 1 or 2.
      end
   end
end

if (TBFlagfound > 1)
   SCI2Cerror('Symbol table conflict: found two symbols with the same name ""'+TBName+'"".');
end
endfunction
