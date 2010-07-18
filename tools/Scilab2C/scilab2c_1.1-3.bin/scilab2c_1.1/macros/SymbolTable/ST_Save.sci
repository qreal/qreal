function ST_Save(SymbolTableFileName,SCI2CSymbolTable)
// function ST_Save(SymbolTableFileName,SCI2CSymbolTable)
// -----------------------------------------------------------------
// Save into a .dat file a symbol table.
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

[tmpnams,tmptyps,tmpdims,tmpvols]=listvarinfile(SymbolTableFileName);
if (max(size(tmpnams)) > 1)
    SCI2Cerror('More than one variable found in ""'+SymbolTableFileName+'"".');
end

// Identifies the Table name and save it into the .dat file.
if (mtlb_strcmp(tmpnams,'GlobalVars'))
   GlobalVars = SCI2CSymbolTable;
   save(SymbolTableFileName,GlobalVars);
elseif (mtlb_strcmp(tmpnams,'LocalVars'))
   LocalVars = SCI2CSymbolTable;
   save(SymbolTableFileName,LocalVars);
elseif (mtlb_strcmp(tmpnams,'TempVars'))
   TempVars = SCI2CSymbolTable;
   save(SymbolTableFileName,TempVars);
else
   SCI2Cerror('Unknow table: ""'+tmpnams+'"".');
end

endfunction
