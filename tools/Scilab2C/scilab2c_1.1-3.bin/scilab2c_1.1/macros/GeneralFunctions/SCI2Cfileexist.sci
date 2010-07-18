function ExistTest = SCI2Cfileexist(InDir,FileName)
// function ExistTest = SCI2Cfileexist(InDir,FileName)
// -----------------------------------------------------------------
// Searches for the file FileName in the directory InDir.
// Return %F if it doesn't exist.
//
// Input data:
// //NUT: add description here
//
// Output data:
// //NUT: add description here
//
// Status:
// 12-Jun-2007 -- Nutricato Raffaele: Author.
//
// Copyright 2007 Raffaele Nutricato.
// Contact: raffaele.nutricato@tiscali.it
// -----------------------------------------------------------------

// ------------------------------
// --- Check input arguments. ---
// ------------------------------
SCI2CNInArgCheck(argn(2),2,2);

tmppwd = pwd();
cd(InDir);
allfiles = ls(FileName);
cd(tmppwd);

if (size(allfiles,1) == 0)
   ExistTest = %F;
elseif (size(allfiles,1) == 1)
   ExistTest = %T;
else
   SCI2Cerror('Very Strange! Found more than one file with the same name.');
end

endfunction
