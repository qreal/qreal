function SCI2Ccopyfile(InFileName,OutFileName,CopyMode)
// function SCI2Ccopyfile(InFileName,OutFileName,CopyMode)
// -----------------------------------------------------------------
// #RNU_RES_B
// Copy the contents of infile into outfile. Append mode is used.
//
// Input data:
// InFileName: path+filename of the input file.
// OutFileName: path+filename of the input file.
// CopyMode: 'append' or 'overwrite'
// #RNU_RES_E
//
// Output data:
// ---
//
// Status:
// 23-Nov-2007 -- Raffaele Nutricato: Author.
//
// Copyright 2007 Raffaele Nutricato
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),3,3);

if (CopyMode == 'append')
   // ------------------------
   // --- Open Input file. ---
   // ------------------------
   fidIn = SCI2COpenFileRead(InFileName);
   
   // -------------------
   // --- Read lines. ---
   // -------------------
   tmpline = mgetl(fidIn,1);
   while (meof(fidIn) == 0)
      PrintStringInfo(tmpline, OutFileName, 'file', 'y');
      tmpline = mgetl(fidIn,1);
   end
   mclose(fidIn);
elseif (CopyMode == 'overwrite')
   PrintStringInfo(' ', OutFileName, 'file', 'y'); // Cannot use scilab copyfile when the directory is empty!.
   copyfile(InFileName,OutFileName);
else
   SCI2Cerror('Unknown CopyMode: ""'+CopyMode+'""');
end

endfunction
