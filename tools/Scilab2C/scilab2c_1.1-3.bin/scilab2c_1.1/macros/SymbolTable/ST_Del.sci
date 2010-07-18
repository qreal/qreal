function ST_Del(TBName,SymbolTableFileName)
// function ST_Del(TBName,SymbolTableFileName)
// -----------------------------------------------------------------
// Delete function for the symbol table.
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
SCI2CNInArgCheck(argn(2),2,2);

// --- Load symbol table. ---
SCI2CSymbolTable = ST_Load(SymbolTableFileName);

// --- Find symbol position. ---
[TBFlagfound,TBPosition] = ST_FindPos(TBName,SymbolTableFileName);

if (TBFlagfound == 0)
   SCI2Cerror('Missing symbol: trying to del a non existing symbol ""'+TBName+'"".');
elseif (TBFlagfound == 1)
   // --- Update symbol table. ---
   SCI2CSymbolTable(TBPosition) = [];
   
   // --- Save symbol table. ---
   ST_Save(SymbolTableFileName,SCI2CSymbolTable);
end

endfunction
